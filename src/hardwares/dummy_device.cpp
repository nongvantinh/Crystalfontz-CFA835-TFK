#include "dummy_device.h"

bool DummyDevice::initialize() {
	return false;
}

DummyDevice::DummyDevice(std::string p_device) :
		BaseDevice(p_device) {
}

DummyDevice::~DummyDevice() {
}

void DummyDevice::upload_file(std::string) {
}
