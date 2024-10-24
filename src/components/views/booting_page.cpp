#include "booting_page.h"

#include "core/container.h"

#include "core/text.h"

void BootingPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<VBoxContainer> vboxcontainer = std::make_shared<VBoxContainer>();
	vboxcontainer->set_alignment(BoxContainer::AlignmentMode::CENTER);
	vboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	vboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(vboxcontainer);
	vboxcontainer->set_position(top_left_position);
	vboxcontainer->set_size(full_size);

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		std::shared_ptr<Text> message = std::make_shared<Text>();
		message->set_font(Font::ROBOTO_LIGHT);
		message->set_text("BOOTING OPSWAT");
		center_container->add_component(message);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		std::shared_ptr<Text> message = std::make_shared<Text>();
		message->set_font(Font::ROBOTO_LIGHT);
		message->set_text("METADEFENDER DRIVE");
		center_container->add_component(message);
	}
}
