#ifndef HARDWARE_FACTORY_H
#define HARDWARE_FACTORY_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class BaseDevice;

struct HardwareModel {
	std::string manufacturer;
	std::string model;

	HardwareModel(std::string p_manufacturer, std::string p_model) :
			manufacturer(p_manufacturer), model(p_model) {}

	struct Hash {
		std::size_t operator()(const HardwareModel &p_hardware) const {
			return std::hash<std::string>()(p_hardware.manufacturer + p_hardware.model);
		}
	};

	bool operator==(const HardwareModel &other) const {
		return manufacturer == other.manufacturer && model == other.model;
	}
};

class HardwareFactory {
public:
	static HardwareFactory *get_singleton();

	bool register_hardware(const HardwareModel &p_model, std::shared_ptr<BaseDevice> (*createFunc)(std::string));
	std::vector<HardwareModel> get_supported_hardwares() const;
	std::shared_ptr<BaseDevice> create_device(const HardwareModel &p_model, std::string p_com_port);

private:
	HardwareFactory();
	~HardwareFactory();

	std::unordered_map<HardwareModel, std::shared_ptr<BaseDevice> (*)(std::string), HardwareModel::Hash> device_creators;
	std::vector<HardwareModel> supported_hardwares;
};

#endif // HARDWARE_FACTORY_H