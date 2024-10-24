#include "no_threats_found_page.h"

#include "core/container.h"

#include "core/text.h"

void NoThreatsFoundPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<VBoxContainer> vboxcontainer = std::make_shared<VBoxContainer>();
	vboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	vboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(vboxcontainer);
	vboxcontainer->set_position(top_left_position);
	vboxcontainer->set_size(full_size);

	{
		std::shared_ptr<HBoxContainer> hboxcontainer = std::make_shared<HBoxContainer>();
		hboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(hboxcontainer);

		{
			std::shared_ptr<Text> text = std::make_shared<Text>();
			text->set_font(Font::ROBOTO_LIGHT);
			text->set_text("RESULT   ");
			hboxcontainer->add_component(text);
		}

		{
			std::shared_ptr<Text> text = std::make_shared<Text>();
			text->set_font(Font::ROBOTO_LIGHT);
			text->set_text("SAFE DEVICE");
			hboxcontainer->add_component(text);
		}
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		message = std::make_shared<Text>();
		message->set_font(Font::ROBOTO_LIGHT);
		message->set_text("NO THREATS FOUND");
		center_container->add_component(message);
	}
}

void NoThreatsFoundPage::update(UNUSED_PARAM float p_delta) {
}
