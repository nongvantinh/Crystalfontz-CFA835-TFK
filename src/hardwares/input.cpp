#include "input.h"

#include <type_traits>

Input *Input::get_singleton() {
	static Input self;
	return &self;
}

KeyCode operator|(KeyCode lhs, KeyCode rhs) {
	using T = std::underlying_type_t<KeyCode>;
	return static_cast<KeyCode>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

KeyCode &operator|=(KeyCode &lhs, KeyCode rhs) {
	lhs = lhs | rhs;
	return lhs;
}

void Input::parse_input_event(std::shared_ptr<InputEvent> p_event) {
	input_event = p_event;
	if (process_input_event_callback) {
		process_input_event_callback(p_event);
	}
}

void Input::set_input_as_handled() {
	input_event = nullptr;
}

std::shared_ptr<InputEvent> Input::get_input_event() {
	return input_event;
}

bool Input::has_input() const {
	return input_event != nullptr;
}
