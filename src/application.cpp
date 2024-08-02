#include "application.h"

#include "hardwares/base_device.h"
#include "hardwares/input.h"
#include "viewport.h"

#include <functional>
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

void Application::start() {
	device_manager = DeviceManager::get_singleton();
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
		device_manager->detect_device_event();
		std::vector<std::shared_ptr<BaseDevice>> connected_devices = device_manager->get_connected_devices();
		bool is_disconnected = std::find(connected_devices.begin(), connected_devices.end(), display_server->get_main_display()) == connected_devices.end();
		if (is_disconnected) {
			if (connected_devices.size()) {
				display_server->set_main_display(connected_devices[0]);
				viewport->set_size(display_server->get_main_display()->get_screen_size() - Vector2i(1, 1));
				viewport->set_position(Vector2i(0, 0));
				viewport->initialize();
				viewport->traverse_depth_first([this](Component *component) -> bool {
					component->notification(Component::Notification::DRAW);
					return false;
				});

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

	viewport->traverse_depth_first([this](Component *component) -> bool {
		if (component->should_perform_render()) {
			component->render();
			component->queue_render(false);
		}
		return false;
	});
}
