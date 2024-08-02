#ifndef CRYSTALFONTZ_DEVICE_H
#define CRYSTALFONTZ_DEVICE_H

#include "hardwares/base_device.h"

class CrystalfontzDevice : public BaseDevice {
protected:
	CrystalfontzDevice(std::string p_device);
	~CrystalfontzDevice() override;
};

#endif // CRYSTALFONTZ_DEVICE_H