#ifndef CONTAINER_H
#define CONTAINER_H

#include "component.h"

#include <memory>

class Container : public Component {
private:
public:
	Container();
	~Container();
	void fit_child_in_rect(std::shared_ptr<Component> p_child, const Vector2i &p_position, const Vector2i &p_size);
	Vector2i get_required_min_size();
	virtual std::vector<Component::SizeFlags> get_allowed_size_flags_horizontal() const;
	virtual std::vector<Component::SizeFlags> get_allowed_size_flags_vertical() const;
};

class BoxContainer : public Container {
public:
	enum class AlignmentMode {
		BEGIN,
		CENTER,
		END
	};

	void initialize() override;
	void update(float delta) override;

	BoxContainer(bool p_vertical);
	~BoxContainer();

	void set_alignment(AlignmentMode p_alignment);
	AlignmentMode get_alignment() const;

	void set_vertical(bool p_vertical);
	bool is_vertical() const;

	Vector2i get_minimum_size() const override;

	std::vector<Component::SizeFlags> get_allowed_size_flags_horizontal() const override;
	std::vector<Component::SizeFlags> get_allowed_size_flags_vertical() const override;

protected:
	bool is_fixed = false;
	void resort();

private:
	bool vertical = false;
	AlignmentMode alignment = AlignmentMode::BEGIN;
};

class VBoxContainer : public BoxContainer {
public:
	VBoxContainer() :
			BoxContainer(true) { is_fixed = true; }
};

class HBoxContainer : public BoxContainer {
public:
	HBoxContainer() :
			BoxContainer(false) { is_fixed = true; }
};

class MarginContainer : public Container {
private:
	size_t margin_left = 0;
	size_t margin_right = 0;
	size_t margin_top = 0;
	size_t margin_bottom = 0;

public:
	MarginContainer() = default;
	MarginContainer(size_t p_margin_left, size_t p_margin_right, size_t p_margin_top, size_t p_margin_bottom);
	void initialize() override;
	void update(float p_delta) override;
	Vector2i get_minimum_size() const override;

	std::vector<Component::SizeFlags> get_allowed_size_flags_horizontal() const override;
	std::vector<Component::SizeFlags> get_allowed_size_flags_vertical() const override;

	size_t get_margin_left() const {
		return margin_left;
	}

	void set_margin_left(size_t value) {
		margin_left = value;
	}

	size_t get_margin_right() const {
		return margin_right;
	}

	void set_margin_right(size_t value) {
		margin_right = value;
	}

	size_t get_margin_top() const {
		return margin_top;
	}

	void set_margin_top(size_t value) {
		margin_top = value;
	}

	size_t get_margin_bottom() const {
		return margin_bottom;
	}

	void set_margin_bottom(size_t value) {
		margin_bottom = value;
	}
};

class CenterContainer : public Container {
public:
	void update(float p_delta) override;

	Vector2i get_minimum_size() const override;

	std::vector<Component::SizeFlags> get_allowed_size_flags_horizontal() const override;
	std::vector<Component::SizeFlags> get_allowed_size_flags_vertical() const override;
};

#endif // CONTAINER_H