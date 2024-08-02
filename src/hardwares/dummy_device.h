#ifndef DUMMY_DEVICE_H
#define DUMMY_DEVICE_H
#include "base_device.h"

class DummyDevice : public BaseDevice {
public:
	bool initialize() override;
	DummyDevice(std::string p_device);
	~DummyDevice() override;

	void upload_file(std::string p_path) override;

};

#endif // DUMMY_DEVICE_H