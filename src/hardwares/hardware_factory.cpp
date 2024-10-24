#include "hardware_factory.h"

#include "hardwares/base_device.h"
#include "hardwares/crystalfontz/cfa835.h"

HardwareFactory::HardwareFactory() {
	HardwareModel cfa835("Crystalfontz", "CFA835");
	register_hardware(cfa835, [](const std::string &p_com_port) -> std::shared_ptr<CFA835> { return std::make_shared<CFA835>(p_com_port); });
}

HardwareFactory *HardwareFactory::get_singleton() {
	static HardwareFactory self;
	return &self;
}

bool HardwareFactory::register_hardware(const HardwareModel &p_model, std::function<std::shared_ptr<BaseDevice>(const std::string &p_com_port)> p_create_func) {
	// Device has already been registered.
	if (device_creators.find(p_model) != device_creators.end()) {
		return false;
	}

	device_creators[p_model] = p_create_func;
	supported_hardwares.push_back(p_model);
	return true;
}

std::vector<HardwareModel> HardwareFactory::get_supported_hardwares() const {
	return supported_hardwares;
}

std::shared_ptr<BaseDevice> HardwareFactory::create_device(const HardwareModel &p_model, const std::string &p_com_port) {
	if (device_creators.find(p_model) != device_creators.end()) {
		return device_creators[p_model](p_com_port);
	}

	return nullptr; // Device not registered
}
