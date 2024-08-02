#include "main.h"

#include "application.h"
#include "hardwares/base_device.h"
#include "hardwares/crystalfontz/cfa835.h"
#include "hardwares/dummy_device.h"
#include "hardwares/hardware_factory.h"
#include "managers/device_manager.h"

#include <iostream>
#include <mutex>

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

Application* application;
Catch::Session session;
std::mutex main_mutex;
void register_crystalfontz_hardwares(HardwareFactory *p_factory);
bool run_test(BaseDevice *p_device, std::string p_tag) {
	std::scoped_lock lock(main_mutex);

	DeviceManager::QUEUE_FOR_TESTING = p_device;

	const char *argv[] = {
		"lcd_device.out",
		"--success",
		"--reporter",
		"compact",
		p_tag.c_str(),
		nullptr // Null-terminate the array
	};

	int argc = sizeof(argv) / sizeof(argv[0]) - 1; // Exclude the null terminator

	int num_failed = 0;
	{
		num_failed = session.run(argc, argv);
	}

	return num_failed == 0;
}

int main() {
	std::cout << "===================STARTING THE LCD===================" << std::endl;
	HardwareFactory *factory = HardwareFactory::get_singleton();
	register_crystalfontz_hardwares(factory);
	application = Application::get_singleton();
	application->start();


	return 0;
}

void register_crystalfontz_hardwares(HardwareFactory *p_factory) {
	HardwareModel dummy("Some Manufacturer", "Dummy");
	p_factory->register_hardware(dummy, [](std::string p_com_port) -> std::shared_ptr<BaseDevice> { return std::make_shared<DummyDevice>(p_com_port); });

	HardwareModel cfa835("Crystalfontz", "CFA835");
	p_factory->register_hardware(cfa835, [](std::string p_com_port) -> std::shared_ptr<BaseDevice> { return std::make_shared<CFA835>(p_com_port); });
}
