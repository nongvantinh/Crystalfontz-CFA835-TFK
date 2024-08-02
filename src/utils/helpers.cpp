#include "helpers.h"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <sstream>

std::string Helpers::run_command(const std::string &p_command) {
	boost::process::ipstream out_stream;
	boost::process::child c("/bin/sh -c \"" + p_command + "\"", boost::process::std_out > out_stream);

	c.wait();
	std::string line;
	std::ostringstream output;
	while (out_stream && std::getline(out_stream, line)) {
		output << line << "\n";
	}
	return output.str();
}

bool Helpers::file_exists(const std::string &p_file_path) {
	return boost::filesystem::exists(p_file_path);
}
