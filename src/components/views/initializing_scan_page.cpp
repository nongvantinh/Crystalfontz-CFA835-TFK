#include "initializing_scan_page.h"

#include "core/container.h"

#include "core/stylized_progress_bar.h"
#include "core/text.h"
#include "viewport.h"

#include "scanning_page.h"

#include "message_type_strings.h"

void InitializingScanPage::initialize() {
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

		std::shared_ptr<Text> text = std::make_shared<Text>();
		text->set_font(Font::ROBOTO_LIGHT);
		text->set_text("INITIALIZING SCAN");
		center_container->add_component(text);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		std::shared_ptr<StylizedProgressBar> progress_bar = std::make_shared<StylizedProgressBar>(3);
		center_container->add_component(progress_bar);
	}
}

InitializingScanPage::InitializingScanPage() {
	WebSocketClient::get_singleton()->register_message(MessageTypeString(trigger_scan), std::bind(&InitializingScanPage::on_trigger_scan_message_received, this, std::placeholders::_1));
}

InitializingScanPage::~InitializingScanPage() {
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(trigger_scan));
}

void InitializingScanPage::on_trigger_scan_message_received(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	get_viewport()->set_root(std::make_shared<ScanningPage>());
}
