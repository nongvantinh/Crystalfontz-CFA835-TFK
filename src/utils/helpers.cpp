#include "helpers.h"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>

std::string Helpers::sanitize_filename(char p_character) {
	std::unordered_map<char, std::string> numbers_to_words = {
		{ '0', "zero" }, { '1', "one" }, { '2', "two" }, { '3', "three" },
		{ '4', "four" }, { '5', "five" }, { '6', "six" }, { '7', "seven" },
		{ '8', "eight" }, { '9', "nine" }
	};

	if (std::isalnum(p_character)) {
		auto it = numbers_to_words.find(p_character);
		return (it != numbers_to_words.end()) ? it->second : std::string(1, p_character);
	} else {
		std::unordered_map<char, std::string> special_chars = {
			{ ' ', "space" }, { '!', "exclamation" }, { '@', "at" }, { '#', "hash" },
			{ '$', "dollar" }, { '%', "percent" }, { '^', "caret" }, { '&', "ampersand" },
			{ '*', "asterisk" }, { '(', "left_paren" }, { ')', "right_paren" },
			{ '-', "dash" }, { '+', "plus" }, { '=', "equals" }, { '{', "left_brace" },
			{ '}', "right_brace" }, { '[', "left_bracket" }, { ']', "right_bracket" },
			{ ':', "colon" }, { ';', "semicolon" }, { ',', "comma" }, { '.', "period" },
			{ '?', "question" }, { '/', "slash" }, { '\\', "backslash" },
			{ '|', "pipe" }, { '~', "tilde" }, { '`', "backtick" },
			{ '"', "quote" }, { '\'', "apostrophe" }
		};

		auto it = special_chars.find(p_character);
		if (it != special_chars.end()) {
			return it->second;
		} else {
			return "char_" + std::to_string(static_cast<int>(p_character));
		}
	}
}

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
