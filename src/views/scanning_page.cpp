#include "scanning_page.h"

#include "arrow_shape.h"
#include "container.h"
#include "font_string_names.h"
#include "text.h"

void ScanningPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<HBoxContainer> hboxcontainer = std::make_shared<HBoxContainer>();
	hboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	hboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(hboxcontainer);
	hboxcontainer->set_position(top_left_position);
	hboxcontainer->set_size(full_size);

	{
		std::shared_ptr<MarginContainer> margincontainer = std::make_shared<MarginContainer>();
		margincontainer->set_margin_right(5);
		hboxcontainer->add_component(margincontainer);
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		margincontainer->add_component(center_container);
		down_arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::LEFT);
		center_container->add_component(down_arrow);
	}

	{
		std::shared_ptr<VBoxContainer> vboxcontainer = std::make_shared<VBoxContainer>();
		vboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		hboxcontainer->add_component(vboxcontainer);

		{
			std::shared_ptr<Text> text = std::make_shared<Text>();
			text->set_font(FontStringName(roboto_condensed_12));
			text->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
			text->set_text("FULL SCAN");
			vboxcontainer->add_component(text);
		}

		{
			std::shared_ptr<Text> text = std::make_shared<Text>();
			text->set_font(FontStringName(roboto_condensed_12));
			text->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
			text->set_text("TIME");
			vboxcontainer->add_component(text);
		}

		{
			std::shared_ptr<Text> text = std::make_shared<Text>();
			text->set_font(FontStringName(roboto_condensed_12));
			text->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
			text->set_text("REMAINING");
			vboxcontainer->add_component(text);
		}

		{
			std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
			vboxcontainer->add_component(center_container);
			down_arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::DOWN);
			center_container->add_component(down_arrow);
		}
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		hboxcontainer->add_component(center_container);

		message = std::make_shared<Text>();
		message->set_font(FontStringName(roboto_condensed_16));
		message->set_text("0:00:00");
		center_container->add_component(message);
	}
}

void ScanningPage::update(UNUSED_PARAM float p_delta) {
}
