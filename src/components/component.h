#ifndef COMPONENT_H
#define COMPONENT_H

#if defined(__GNUC__) || defined(__clang__)
#define UNUSED_PARAM [[maybe_unused]]
#elif defined(_MSC_VER)
#define UNUSED_PARAM
#pragma warning(push)
#pragma warning(disable : 4100)
#else
#define UNUSED_PARAM
#endif

#include <cstdint>
#include <list>
#include <memory>

#include "hardwares/input.h"
#include "math/vector2i.h"

template <typename T>
class BitField {
	int64_t value = 0;

public:
	BitField<T> &set_flag(T p_flag) {
		value |= (int64_t)p_flag;
		return *this;
	}
	bool has_flag(T p_flag) const { return value & (int64_t)p_flag; }
	bool is_empty() const { return value == 0; }
	BitField<T> clear_flag(T p_flag) {
		value &= ~(int64_t)p_flag;
		return *this;
	}
	BitField<T> clear() {
		value = 0;
		return *this;
	}
	constexpr BitField() = default;
	constexpr BitField(int64_t p_value) { value = p_value; }
	constexpr BitField(T p_value) { value = (int64_t)p_value; }
	operator int64_t() const { return value; }
	BitField<T> operator^(const BitField<T> &p_b) const { return BitField<T>(value ^ p_b.value); }
};

class Viewport;

class Component {
public:
	enum class Notification {
		DRAW
	};

	enum class SizeFlags {
		SHRINK_BEGIN = 0,
		FILL = 1,
		EXPAND = 2,
		SHRINK_CENTER = 4,
		SHRINK_END = 8,

		EXPAND_FILL = EXPAND | FILL,
	};

	Component() = default;
	virtual ~Component() = default;
	virtual void initialize();
	virtual void notification(UNUSED_PARAM Notification p_which);
	virtual void update(UNUSED_PARAM float p_delta);
	virtual void render();
	virtual void input_event(UNUSED_PARAM std::shared_ptr<InputEvent> p_event);
	virtual void shutdown();

	void queue_render(bool p_needs_rendering = true);
	bool should_perform_render() const;
	void add_component(std::shared_ptr<Component> p_component);
	void remove_component(std::shared_ptr<Component> p_component);
	std::shared_ptr<Component> get_component(size_t p_index) const;
	std::list<std::shared_ptr<Component>> get_components() const;
	size_t get_component_size() const;
	void traverse_depth_first(const std::function<bool(Component *)> &p_callback);
	void traverse_post_order(const std::function<bool(Component *)> &p_callback);

	std::shared_ptr<Viewport> get_viewport() const;
	Component *get_parent() const;

	std::string get_name() const;
	void set_name(std::string p_name);

	void set_size(Vector2i p_size);
	Vector2i get_size() const;
	Vector2i get_combine_minimum_size() const;

	virtual Vector2i get_minimum_size() const;
	void set_custom_minimum_size(Vector2i p_size);
	Vector2i get_custom_minimum_size() const;

	void set_position(Vector2i p_size);
	Vector2i get_position() const;

	void set_h_size_flags(BitField<SizeFlags> p_flags);
	BitField<SizeFlags> get_h_size_flags() const;
	void set_v_size_flags(BitField<SizeFlags> p_flags);
	BitField<SizeFlags> get_v_size_flags() const;
	float get_stretch_ratio() const;
	void set_stretch_ratio(float p_ratio);
	bool is_drawable() const;
	void set_drawable(bool p_can_draw);

private:
	Component *parent = nullptr;
	std::list<std::shared_ptr<Component>> components = {};
	std::string name = "";
	bool needs_rendering = false;
	Vector2i position = Vector2i::ZERO;
	Vector2i size = Vector2i::ZERO;
	Vector2i custom_minimum_size = Vector2i::ZERO;
	BitField<Component::SizeFlags> h_size_flags = Component::SizeFlags::FILL;
	BitField<Component::SizeFlags> v_size_flags = Component::SizeFlags::FILL;
	float stretch_ratio = 1.0f;
	bool can_draw = true;
};

#endif // COMPONENT_H