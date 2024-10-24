#include "core/container.h"
#include "helpers.h"


#include <map>
#include <memory>

struct _MinSizeCache {
	int32_t min_size = 0;
	bool will_stretch = false;
	int32_t final_size = 0;
};

Container::Container() {
}

Container::~Container() {
}

void Container::fit_child_in_rect(std::shared_ptr<Component> p_child, const Vector2i &p_position, const Vector2i &p_size) {
	Vector2i minsize = p_child->get_combine_minimum_size();
	Vector2i position = get_position() + p_position;
	Vector2i size = p_size;

	if (!(p_child->get_h_size_flags().has_flag(Component::SizeFlags::FILL))) {
		size.x = minsize.x;
		if (p_child->get_h_size_flags().has_flag(Component::SizeFlags::SHRINK_END)) {
			position.x += p_size.x - minsize.x;
		} else if (p_child->get_h_size_flags().has_flag(Component::SizeFlags::SHRINK_CENTER)) {
			position.x += (p_size.x - minsize.x) / 2;
		} else {
			position.x += 0;
		}
	}

	if (!(p_child->get_v_size_flags().has_flag(Component::SizeFlags::FILL))) {
		size.y = minsize.y;
		if (p_child->get_v_size_flags().has_flag(Component::SizeFlags::SHRINK_END)) {
			position.y += p_size.y - minsize.y;
		} else if (p_child->get_v_size_flags().has_flag(Component::SizeFlags::SHRINK_CENTER)) {
			position.y += (p_size.y - minsize.y) / 2;
		} else {
			position.y += 0;
		}
	}

	p_child->set_position(position);
	p_child->set_size(size);
}

Vector2i Container::get_required_min_size() {
	Vector2i required_size = Vector2i::ZERO;

	traverse_post_order([&required_size](Component *component) -> bool {
		required_size += component->get_minimum_size();
		return false;
	});

	return required_size;
}

std::vector<Component::SizeFlags> Container::get_allowed_size_flags_horizontal() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	flags.push_back(Component::SizeFlags::EXPAND);
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

std::vector<Component::SizeFlags> Container::get_allowed_size_flags_vertical() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	flags.push_back(Component::SizeFlags::EXPAND);
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

void BoxContainer::initialize() {
}

void BoxContainer::update(UNUSED_PARAM float p_delta) {
	resort();

	// {
	// 	uint8_t intensity = 0;
	// 	uint8_t border_intensity = 255;

	// 	DisplayServer::get_singleton()->draw_rectangle(get_position(), get_size(), border_intensity, intensity);
	// }
}

void BoxContainer::resort() {
	Vector2i new_size = get_size();
	int32_t stretch_min = 0;
	int32_t stretch_avail = 0;
	float stretch_ratio_total = 0.0f;
	std::map<std::shared_ptr<Component>, _MinSizeCache> min_size_cache;

	std::list<std::shared_ptr<Component>> components = get_components();
	for (const std::shared_ptr<Component> &component : components) {
		Vector2i size = component->get_combine_minimum_size();
		_MinSizeCache msc;

		if (vertical) {
			stretch_min += size.y;
			msc.min_size = size.y;
			msc.will_stretch = component->get_v_size_flags().has_flag(Component::SizeFlags::EXPAND);
		} else {
			stretch_min += size.x;
			msc.min_size = size.x;
			msc.will_stretch = component->get_h_size_flags().has_flag(Component::SizeFlags::EXPAND);
		}

		if (msc.will_stretch) {
			stretch_avail += msc.min_size;
			stretch_ratio_total += component->get_stretch_ratio();
		}

		msc.final_size += msc.min_size;
		min_size_cache[component] = msc;
	}

	int32_t stretch_max = (vertical ? new_size.y : new_size.x);
	int32_t stretch_diff = stretch_max - stretch_min;
	if (stretch_diff < 0) {
		stretch_diff = 0;
	}

	stretch_avail += stretch_diff;

	bool has_stretched = false;
	while (stretch_ratio_total > 0) {
		has_stretched = true;
		bool refit_successful = true;
		float error = 0.0f;

		for (const std::shared_ptr<Component> &component : components) {
			_MinSizeCache &msc = min_size_cache[component];

			if (msc.will_stretch) {
				float final_pixel_size = stretch_avail * component->get_stretch_ratio() / stretch_ratio_total;
				error += final_pixel_size - (int32_t)final_pixel_size;

				if (final_pixel_size < msc.min_size) {
					msc.will_stretch = false;
					stretch_ratio_total -= component->get_stretch_ratio();
					refit_successful = false;
					stretch_avail -= msc.min_size;
					msc.final_size = msc.min_size;
					break;
				} else {
					msc.final_size = final_pixel_size;
					if (error >= 1) {
						msc.final_size += 1;
						error -= 1;
					}
				}
			}
		}

		if (refit_successful) {
			break;
		}
	}

	int32_t ofs = 0;
	if (!has_stretched) {
		if (!vertical) {
			switch (alignment) {
				case AlignmentMode::BEGIN:
					break;
				case AlignmentMode::CENTER:
					ofs = stretch_diff / 2;
					break;
				case AlignmentMode::END:
					ofs = stretch_diff;
					break;
			}
		} else {
			switch (alignment) {
				case AlignmentMode::BEGIN:
					break;
				case AlignmentMode::CENTER:
					ofs = stretch_diff / 2;
					break;
				case AlignmentMode::END:
					ofs = stretch_diff;
					break;
			}
		}
	}

	for (const std::shared_ptr<Component> &component : components) {
		_MinSizeCache &msc = min_size_cache[component];

		int32_t from = ofs;
		int32_t to = ofs + msc.final_size;

		int32_t size = to - from;

		Vector2i c_position;
		Vector2i c_size;
		if (vertical) {
			c_position = Vector2i(0, from);
			c_size = Vector2i(new_size.x, size);
		} else {
			c_position = Vector2i(from, 0);
			c_size = Vector2i(size, new_size.y);
		}

		fit_child_in_rect(component, c_position, c_size);

		ofs = to;
	}
}

BoxContainer::BoxContainer(bool p_vertical) {
	vertical = p_vertical;
}

BoxContainer::~BoxContainer() {
}

void BoxContainer::set_alignment(AlignmentMode p_alignment) {
	alignment = p_alignment;
	resort();
}

BoxContainer::AlignmentMode BoxContainer::get_alignment() const {
	return alignment;
}

void BoxContainer::set_vertical(bool p_vertical) {
	vertical = p_vertical;
}

bool BoxContainer::is_vertical() const {
	return vertical;
}

Vector2i BoxContainer::get_minimum_size() const {
	Vector2i min_size = Vector2i::ZERO;

	for (std::shared_ptr<Component> child : get_components()) {
		Vector2i child_size = child->get_combine_minimum_size();
		if (vertical) {
			min_size.x = std::max(min_size.x, child_size.x);
			min_size.y += child_size.y;
		} else {
			min_size.x += child_size.x;
			min_size.y = std::max(min_size.y, child_size.y);
		}
	}

	return min_size;
}

std::vector<Component::SizeFlags> BoxContainer::get_allowed_size_flags_horizontal() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	if (!vertical) {
		flags.push_back(Component::SizeFlags::EXPAND);
	}
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

std::vector<Component::SizeFlags> BoxContainer::get_allowed_size_flags_vertical() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	if (vertical) {
		flags.push_back(Component::SizeFlags::EXPAND);
	}
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

MarginContainer::MarginContainer(size_t p_margin_left, size_t p_margin_right, size_t p_margin_top, size_t p_margin_bottom) :
		margin_left(p_margin_left), margin_right(p_margin_right), margin_top(p_margin_top), margin_bottom(p_margin_bottom) {
}

void MarginContainer::initialize() {
}

void MarginContainer::update(UNUSED_PARAM float p_delta) {
	Vector2i size = get_size();

	std::list<std::shared_ptr<Component>> components = get_components();
	for (const std::shared_ptr<Component> &component : components) {
		int32_t width = size.x - get_margin_left() - get_margin_right();
		int32_t height = size.y - get_margin_top() - get_margin_bottom();
		fit_child_in_rect(component, Vector2i(get_margin_left(), get_margin_top()), Vector2i(width, height));
	}
}

Vector2i MarginContainer::get_minimum_size() const {
	Vector2i max = Vector2i::ZERO;

	std::list<std::shared_ptr<Component>> components = get_components();
	for (const std::shared_ptr<Component> &component : components) {
		Vector2i s = component->get_combine_minimum_size();
		if (s.x > max.x) {
			max.x = s.x;
		}
		if (s.y > max.y) {
			max.y = s.y;
		}
	}

	max.x += get_margin_left() + get_margin_right();
	max.y += get_margin_top() + get_margin_bottom();

	return max;
}

std::vector<Component::SizeFlags> MarginContainer::get_allowed_size_flags_horizontal() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

std::vector<Component::SizeFlags> MarginContainer::get_allowed_size_flags_vertical() const {
	std::vector<Component::SizeFlags> flags;
	flags.push_back(Component::SizeFlags::FILL);
	flags.push_back(Component::SizeFlags::SHRINK_BEGIN);
	flags.push_back(Component::SizeFlags::SHRINK_CENTER);
	flags.push_back(Component::SizeFlags::SHRINK_END);
	return flags;
}

void CenterContainer::update(UNUSED_PARAM float p_delta) {
	Vector2i size = get_size();

	std::list<std::shared_ptr<Component>> components = get_components();
	for (const std::shared_ptr<Component> &component : components) {
		Vector2i ms = component->get_combine_minimum_size();
		Vector2i ofs = (size - ms) / 2.0;
		fit_child_in_rect(component, ofs, ms);
	}
}

Vector2i CenterContainer::get_minimum_size() const {
	Vector2i minsize = Vector2i::ZERO;

	std::list<std::shared_ptr<Component>> components = get_components();
	for (const std::shared_ptr<Component> &component : components) {
		Vector2i ms = component->get_combine_minimum_size();
		minsize = minsize.max(ms);
	}

	return minsize;
}

std::vector<Component::SizeFlags> CenterContainer::get_allowed_size_flags_horizontal() const {
	std::vector<Component::SizeFlags> flags;
	return flags;
}

std::vector<Component::SizeFlags> CenterContainer::get_allowed_size_flags_vertical() const {
	std::vector<Component::SizeFlags> flags;
	return flags;
}
