#define CATCH_CONFIG_RUNNER

#include "application.h"
#include "catch.hpp"
#include "generated/bitmap_class_factory.h"

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

Catch::Session session;

class ArgumentParser {
public:
	using Callback = std::function<void()>;
	struct TupleHash {
		template <class T1, class T2>
		std::size_t operator()(const std::tuple<T1, T2> &tuple) const {
			auto hash1 = std::hash<T1>{}(std::get<0>(tuple));
			auto hash2 = std::hash<T2>{}(std::get<1>(tuple));
			return hash1 ^ (hash2 << 1);
		}
	};

	class Option {
	public:
		virtual ~Option() = default;
		virtual std::ostream &print(std::ostream &p_os) const = 0;
		friend std::ostream &operator<<(std::ostream &p_os, const Option &p_option) {
			p_os << std::setw(70) << std::left;
			return p_option.print(p_os);
		}
	};

	class LongOption : public Option {
	public:
		LongOption(const std::string &p_key, const std::string &p_value, const std::string &p_description) :
				key(p_key), value(p_value), description(p_description) {}

		std::string key;
		std::string value;
		std::string description;

		std::ostream &print(std::ostream &p_os) const override {
			return p_os << "--" + key + "=" + value << description;
		}
	};

	class ShortOption : public Option {
	public:
		ShortOption(const std::string &p_key, const std::string &p_value, const std::string &p_description) :
				key(p_key), value(p_value), description(p_description) {}

		std::string key;
		std::string value;
		std::string description;

		std::ostream &print(std::ostream &p_os) const override {
			return p_os << "-" + key + " " + value << description;
		}
	};

	class Positional : public Option {
	public:
		Positional(const std::string &p_key, const std::string &p_description) :
				key(p_key), description(p_description) {}

		std::string key;
		std::string description;

		std::ostream &print(std::ostream &p_os) const override {
			return p_os << key << description;
		}
	};

	void parse(int argc, char *argv[]) {
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "--help" || arg == "-h") {
				show_help();
			} else if (arg.substr(0, 2) == "--") {
				// Handle long options
				size_t equal_pos = arg.find('=');
				std::string option = arg.substr(2, equal_pos - 2);
				std::string value = (equal_pos != std::string::npos) ? arg.substr(equal_pos + 1) : "";
				options[option] = value;
			} else if (arg.substr(0, 1) == "-") {
				// Handle short options
				std::string option = arg.substr(1);
				options[option] = (i + 1 < argc) ? argv[++i] : "";
			} else {
				// Handle positional arguments (non-option arguments)
				positionals.push_back(arg);
			}
		}
	}

	void add_long_option(const std::string &p_key, const std::string &p_value, Callback p_callback, const std::string &p_description = "") {
		option_list.push_back(std::make_unique<LongOption>(p_key, p_value, p_description));
		option_callbacks[std::make_tuple(p_key, p_value)] = p_callback;
	}

	void add_short_option(const std::string &p_key, const std::string &p_value, Callback p_callback, const std::string &p_description = "") {
		option_list.push_back(std::make_unique<ShortOption>(p_key, p_value, p_description));
		option_callbacks[std::make_tuple(p_key, p_value)] = p_callback;
	}

	void add_positional(const std::string &p_key, Callback p_callback, const std::string &p_description = "") {
		option_list.push_back(std::make_unique<Positional>(p_key, p_description));
		positional_callbacks[p_key] = p_callback;
	}

	void process_options() {
		for (const auto &[key, value] : options) {
			std::unordered_map<std::tuple<std::string, std::string>, Callback, TupleHash>::iterator it = option_callbacks.find(std::make_tuple(key, value));
			if (it != option_callbacks.end()) {
				it->second();
			} else {
				std::cout << "No callback found for option: [" << key << ", " << value << "]" << std::endl;
			}
		}

		for (const std::string &arg : positionals) {
			std::unordered_map<std::string, ArgumentParser::Callback>::iterator it = positional_callbacks.find(arg);
			if (it != positional_callbacks.end()) {
				it->second();
			} else {
				std::cout << "No callback found for option: " << arg << "!" << std::endl;
			}
		}
	}

	void show_help() const {
		std::cout << "Usage: [options]\n";
		std::cout << "Options:\n";
		for (const std::unique_ptr<ArgumentParser::Option> &option : option_list) {
			std::cout << *option << "\n";
		}
		std::cout << std::setw(70) << std::left << "--help, -h" << "Show help message\n";
		exit(0);
	}

	const std::unordered_map<std::string, std::string> &get_options() const {
		return options;
	}

	const std::vector<std::string> &get_positionals() const {
		return positionals;
	}

private:
	std::unordered_map<std::string, std::string> options;
	std::vector<std::string> positionals;
	std::unordered_map<std::tuple<std::string, std::string>, Callback, TupleHash> option_callbacks;
	std::unordered_map<std::string, Callback> positional_callbacks;
	std::vector<std::unique_ptr<Option>> option_list;
};

void process_cache_bitmap();
void process_short_option();
void process_run_unit_test();

int main(int argc, char *argv[]) {
	ArgumentParser parser;
	parser.add_long_option("cache-data", "bitmap", process_cache_bitmap, "Generate cache data. Possible value: (bitmap, api)");
	parser.add_short_option("o", "Value", process_short_option, "Example short option");
	parser.add_positional("test", process_run_unit_test, "Run unit test");
	parser.parse(argc, argv);
	parser.process_options();

	BitmapClassFactory::initialize();

	Application::get_singleton()->start();

	return 0;
}

void process_cache_bitmap() {
	std::cout << "process_cache_bitmap" << std::endl;
}

void process_short_option() {
	std::cout << "process_short_option" << std::endl;
}

void process_run_unit_test() {
	std::cout << "process_run_unit_test" << std::endl;
}
