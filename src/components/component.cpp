#include "component.h"

#include "application.h"
#include "hardwares/input.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <stack>

void Component::initialize() {
}

void Component::notification(UNUSED_PARAM Notification p_which) {
}

void Component::update(UNUSED_PARAM float p_delta) {
}

void Component::render() {
}

void Component::input_event(UNUSED_PARAM std::shared_ptr<InputEvent> p_event) {
}

void Component::shutdown() {
}

void Component::queue_render(bool p_needs_rendering) {
	needs_rendering = p_needs_rendering;
}

bool Component::should_perform_render() const {
	return needs_rendering;
}

void Component::add_component(std::shared_ptr<Component> p_component) {
	if (std::find(components.begin(), components.end(), p_component) == components.end()) {
		components.push_back(p_component);
		p_component->parent = this;
		p_component->initialize();
		std::cout << "Component added.\n";
	} else {
		std::cout << "Component already exists in the list.\n";
	}
}

void Component::remove_component(std::shared_ptr<Component> p_component) {
	auto it = std::find(components.begin(), components.end(), p_component);
	if (it != components.end()) {
		components.erase(it);
		p_component->shutdown();
		p_component->parent = nullptr;
		std::cout << "Component removed.\n";
	} else {
		std::cout << "Component not found in the list.\n";
	}
}

std::shared_ptr<Component> Component::get_component(size_t p_index) const {
	if (p_index >= components.size()) {
		throw std::out_of_range("Index out of range");
	}
	auto it = components.begin();
	std::advance(it, p_index);
	return *it;
}

std::list<std::shared_ptr<Component>> Component::get_components() const {
	return components;
}

std::shared_ptr<Viewport> Component::get_viewport() const {
	return Application::get_singleton()->get_viewport();
}

Component *Component::get_parent() const {
	return parent;
}

size_t Component::get_component_size() const {
	return components.size();
}

std::string Component::get_name() const {
	return name;
}

void Component::set_name(std::string p_name) {
	name = p_name;
}

void Component::set_size(Vector2i p_size) {
	Vector2i new_size = p_size;
	Vector2i min = get_combine_minimum_size();
	if (p_size.x < min.x) {
		new_size.x = min.x;
	}
	if (p_size.y < min.y) {
		new_size.y = min.y;
	}
	size = new_size;
}

Vector2i Component::get_size() const {
	return size < get_combine_minimum_size() ? get_combine_minimum_size() : size;
}

Vector2i Component::get_combine_minimum_size() const {
	Vector2i true_size = get_minimum_size() < custom_minimum_size ? custom_minimum_size : get_minimum_size();

	return true_size;
}

Vector2i Component::get_minimum_size() const {
	return Vector2i::ZERO;
}

void Component::set_custom_minimum_size(Vector2i p_size) {
	custom_minimum_size = p_size;
}

Vector2i Component::get_custom_minimum_size() const {
	return custom_minimum_size;
}

void Component::set_position(Vector2i p_position) {
	position = p_position;
}

Vector2i Component::get_position() const {
	return position;
}

void Component::set_h_size_flags(BitField<SizeFlags> p_flags) {
	if ((int)h_size_flags == (int)p_flags) {
		return;
	}
	h_size_flags = p_flags;
}

BitField<Component::SizeFlags> Component::get_h_size_flags() const {
	return h_size_flags;
}

void Component::set_v_size_flags(BitField<SizeFlags> p_flags) {
	if ((int)v_size_flags == (int)p_flags) {
		return;
	}
	v_size_flags = p_flags;
}

BitField<Component::SizeFlags> Component::get_v_size_flags() const {
	return v_size_flags;
}

float Component::get_stretch_ratio() const {
	return stretch_ratio;
}

void Component::set_stretch_ratio(float p_ratio) {
	stretch_ratio = p_ratio;
}

bool Component::is_drawable() const {
	return can_draw;
}

void Component::set_drawable(bool p_can_draw) {
	can_draw = p_can_draw;
}

void Component::traverse_depth_first(const std::function<bool(Component *)> &p_callback) {
	std::stack<Component *> stack;

	stack.push(this);

	while (!stack.empty()) {
		Component *current = stack.top();
		stack.pop();
		bool should_stop = p_callback(current);
		if (should_stop) {
			return;
		}

		auto components = current->get_components();

		for (auto it = components.rbegin(); it != components.rend(); ++it) {
			stack.push(it->get());
		}
	}
}

void Component::traverse_post_order(const std::function<bool(Component *)> &p_callback) {
	std::stack<Component *> stack1;
	std::stack<Component *> stack2;

	stack1.push(this);

	while (!stack1.empty()) {
		Component *current = stack1.top();
		stack1.pop();
		stack2.push(current);

		// Push child components onto the stack in reverse order (to visit the left-most child first)
		auto components = current->get_components();

		for (auto it = components.rbegin(); it != components.rend(); ++it) {
			stack1.push(it->get());
		}
	}

	while (!stack2.empty()) {
		Component *current = stack2.top();
		stack2.pop();
		bool should_stop = p_callback(current);
		if (should_stop) {
			return;
		}
	}
}