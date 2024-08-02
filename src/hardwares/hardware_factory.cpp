#include "hardware_factory.h"

HardwareFactory::HardwareFactory() {
}

HardwareFactory::~HardwareFactory() {
}

HardwareFactory *HardwareFactory::get_singleton() {
	static HardwareFactory self;
	return &self;
}

bool HardwareFactory::register_hardware(const HardwareModel &p_model, std::shared_ptr<BaseDevice> (*createFunc)(std::string p_com_port)) {
	// Device has already been registered.
	if (device_creators.find(p_model) != device_creators.end()) {
		return false;
	}

	device_creators[p_model] = createFunc;
	supported_hardwares.push_back(p_model);
	return true;
}

std::vector<HardwareModel> HardwareFactory::get_supported_hardwares() const {
	return supported_hardwares;
}

std::shared_ptr<BaseDevice> HardwareFactory::create_device(const HardwareModel &p_model, std::string p_com_port) {
	if (device_creators.find(p_model) != device_creators.end()) {
		return device_creators[p_model](p_com_port);
	}

	return nullptr; // Device not registered
}
