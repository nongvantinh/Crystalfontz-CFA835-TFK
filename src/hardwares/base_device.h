#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "math/vector2i.h"

#include <string>

class BaseDevice {
public:
	virtual bool initialize() = 0;
	std::string get_device_path() const;

	Vector2i get_screen_size() const;
	void set_screen_size(const Vector2i &p_size);

	virtual void upload_file(std::string p_path) = 0;

protected:
	BaseDevice(std::string p_device);
	virtual ~BaseDevice() = 0;

private:
	// The path representing an ACM (Abstract Control Model) device.
	std::string device;
	Vector2i screen_size;
};

#endif // BASE_DEVICE_H