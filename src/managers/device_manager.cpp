#include "device_manager.h"

#include "hardwares/base_device.h"
#include "hardwares/hardware_factory.h"
#include "utils/helpers.h"

#include <algorithm>
#include <boost/asio.hpp>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

BaseDevice *DeviceManager::QUEUE_FOR_TESTING = nullptr;

DeviceManager::DeviceManager() :
		connected_devices(), excluded_ports() {
}

DeviceManager::~DeviceManager() {
}

DeviceManager *DeviceManager::get_singleton() {
	static DeviceManager self;
	return &self;
}

void DeviceManager::detect_device_event() {
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

// Devices can be plugged or unplugged anytime, so don't store
// a cache reference to the function's result. Use the returned value directly.
std::vector<std::shared_ptr<BaseDevice>> DeviceManager::get_connected_devices() {
	std::vector<std::shared_ptr<BaseDevice>> devices;
	for (const auto &pair : connected_devices) {
		devices.push_back(pair.second);
	}

	return devices;
}
