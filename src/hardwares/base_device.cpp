#include "base_device.h"

BaseDevice::BaseDevice(std::string p_device) :
		device(p_device) {
}

BaseDevice::~BaseDevice() {
}

std::string BaseDevice::get_device_path() const {
	return device;
}

Vector2i BaseDevice::get_screen_size() const {
	return screen_size;
}

void BaseDevice::set_screen_size(const Vector2i &p_size) {
	screen_size = p_size;
}