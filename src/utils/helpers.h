#ifndef HELPERS_H
#define HELPERS_H

#include <string>

class Helpers {
public:
	static std::string run_command(const std::string &p_command);
	static bool file_exists(const std::string &p_file_path);
};

#endif // HELPERS_H
