#include "application.h"

#include "hardwares/base_device.h"
#include "hardwares/hardware_factory.h"
#include "hardwares/input.h"
#include "utils/helpers.h"
#include "viewport.h"

#include <functional>
#include <regex>
#include <stack>

const float Application::DEFAULT_FPS = 30.0f;

Application *Application::get_singleton() {
	static Application self(Application::DEFAULT_FPS);
	return &self;
}

float Application::get_fps() const {
	return fps;
}

void Application::set_fps(float p_fps) {
	fps = p_fps;
}

std::shared_ptr<Viewport> Application::get_viewport() {
	return viewport;
}

void Application::detect_device_event() {
	std::string stream(Helpers::run_command("ls /dev"));
	// The CFA835 will appear under Linux as a Virtual COM port
	// as /dev/ttyACMx (where x is the next available device number).
	std::regex pattern("ttyACM\\d+");
	std::smatch matches;
	std::vector<std::string> com_ports;
	std::string::const_iterator search_start(stream.cbegin());
	while (std::regex_search(search_start, stream.cend(), matches, pattern)) {
		for (const auto &port : matches) {
			com_ports.push_back("/dev/" + port.str());
		}

		// Update the search start position for the next iteration
		search_start = matches.suffix().first;
	}

	// Added devices.
	for (const std::string &port : com_ports) {
		bool is_excluded = std::find(excluded_ports.begin(), excluded_ports.end(), port) != excluded_ports.end();
		if (!is_excluded && connected_devices.find(port) == connected_devices.end()) {
			std::cout << "===================================================" << std::endl;
			std::cout << "COM port: " << port << std::endl;
			bool success = false;
			for (HardwareModel model : HardwareFactory::get_singleton()->get_supported_hardwares()) {
				std::cout << "Attempting to map the COM port as a "
						  << "[" << model.manufacturer << ", "
						  << model.model << "] device." << std::endl;

				std::shared_ptr<BaseDevice> device = HardwareFactory::get_singleton()->create_device(model, port);

				success = device->initialize();
				if (success) {
					std::cout << "Successfully mapped the device as a "
							  << "[" << model.manufacturer << ", "
							  << model.model << "]" << std::endl;

					connected_devices.insert({ port, device });
					break;
				} else {
					std::cout << "Unable to map COM port: " << port << " for the "
							  << "[" << model.manufacturer << ", "
							  << model.model << "] device." << std::endl;
				}
			}

			if (!success) {
				std::cout << "The connected device is unknown. Did you forget to register this hardware?" << std::endl;
				excluded_ports.push_back(port);
			}
			std::cout << "===================================================" << std::endl;
		}
	}

	// Removed devices.
	{
		std::vector<std::string> keys;
		for (const auto &pair : connected_devices) {
			keys.push_back(pair.first);
		}

		for (const std::string &port : keys) {
			if (std::find(com_ports.begin(), com_ports.end(), port) == com_ports.end()) {
				std::cout << "- " << port << std::endl;
				connected_devices.erase(port);
			}
		}

		{
			std::vector<std::string> removed_ports;
			excluded_ports.erase(
					std::remove_if(excluded_ports.begin(), excluded_ports.end(), [&](std::string port) {
						if (std::find(com_ports.begin(), com_ports.end(), port) == com_ports.end()) {
							removed_ports.push_back(port);
							return true;
						}
						return false;
					}),
					excluded_ports.end());
			for (auto port : removed_ports) {
				std::cout << "- " << port << std::endl;
			}
		}
	}
}

std::vector<std::shared_ptr<BaseDevice>> Application::get_connected_devices() {
	std::vector<std::shared_ptr<BaseDevice>> devices;
	for (const auto &pair : connected_devices) {
		devices.push_back(pair.second);
	}

	return devices;
}

void Application::start() {
	display_server = DisplayServer::get_singleton();
	is_running = true;
	viewport = std::make_shared<Viewport>();
	Input::get_singleton()->process_input_event_callback = std::bind(&Application::process_input_event, this, std::placeholders::_1);
	run();
}

void Application::stop() {
	is_running = false;
}

void Application::process_input_event(std::shared_ptr<InputEvent> p_event) {
	Input *input = Input::get_singleton();

	viewport->traverse_post_order([&input, &p_event](Component *component) -> bool {
		// Input has been processed, stop broadcasting.
		if (!input->has_input()) {
			return true;
		}

		component->input_event(p_event);
		return false;
	});
}

void Application::run() {
	std::chrono::milliseconds frame_duration = std::chrono::milliseconds(int(1000 / fps));

	while (is_running) {
		auto start_time = std::chrono::high_resolution_clock::now();
		DisplayServer::get_singleton()->clear_display();
		update();
		DisplayServer::get_singleton()->flush_graphic_buffer();

		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

		if (elapsed_time < frame_duration) {
			std::this_thread::sleep_for(frame_duration - elapsed_time);
		}
	}
}

void Application::update() {
	{
		detect_device_event();
		std::vector<std::shared_ptr<BaseDevice>> connected_devices = get_connected_devices();
		bool is_disconnected = std::find(connected_devices.begin(), connected_devices.end(), display_server->get_main_display()) == connected_devices.end();
		if (is_disconnected) {
			if (connected_devices.size()) {
				display_server->set_main_display(connected_devices[0]);
				viewport->set_size(display_server->get_main_display()->get_screen_size() - Vector2i(1, 1));
				viewport->set_position(Vector2i(0, 0));
				viewport->initialize();
				// viewport->traverse_depth_first([this](Component *component) -> bool {
				// 	component->notification(Component::Notification::DRAW);
				// 	return false;
				// });

			} else {
				display_server->set_main_display(nullptr);
				viewport->shutdown();
			}
		}
	}

	viewport->traverse_depth_first([this](Component *component) -> bool {
		component->update(1.0f / get_fps());
		return false;
	});

	// viewport->traverse_depth_first([this](Component *component) -> bool {
	// 	if (component->should_perform_render()) {
	// 		component->render();
	// 		component->queue_render(false);
	// 	}
	// 	return false;
	// });
}
