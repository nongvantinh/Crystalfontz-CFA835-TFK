#ifndef INPUT_H
#define INPUT_H

#include <functional>
#include <memory>

enum class KeyCode {
	NONE = 0,
	UP = 1 << 0,
	ENTER = 1 << 1,
	CANCEL = 1 << 2,
	LEFT = 1 << 3,
	RIGHT = 1 << 4,
	DOWN = 1 << 5,
};

KeyCode operator|(KeyCode lhs, KeyCode rhs);
KeyCode &operator|=(KeyCode &lhs, KeyCode rhs);

struct InputEvent {
	KeyCode key;
	bool is_pressed;
	InputEvent() :
			key(KeyCode::NONE), is_pressed(false) {}
	InputEvent(KeyCode p_key, bool p_is_pressed) :
			key(p_key), is_pressed(p_is_pressed) {}
};

class Input {
private:
	std::shared_ptr<InputEvent> input_event;

public:
	static Input *get_singleton();
	void parse_input_event(std::shared_ptr<InputEvent> p_event);
	void set_input_as_handled();
	std::shared_ptr<InputEvent> get_input_event();
	bool has_input() const;
	std::function<void(std::shared_ptr<InputEvent>)> process_input_event_callback;
};

#endif // INPUT_H