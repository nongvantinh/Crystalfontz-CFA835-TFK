#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <memory>
#include <vector>
#include <unordered_map>

class BaseDevice;

class DeviceManager {
public:
	static BaseDevice* QUEUE_FOR_TESTING;
	static DeviceManager *get_singleton();

	std::vector<std::shared_ptr<BaseDevice>> get_connected_devices();
	void detect_device_event();
private:
	DeviceManager();
	~DeviceManager();
	std::unordered_map<std::string, std::shared_ptr<BaseDevice>> connected_devices;
	std::vector<std::string> excluded_ports;
};

#endif // DEVICE_MANAGER_h