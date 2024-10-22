#include "confirm_full_scan_page.h"

#include "arrow_shape.h"
#include "container.h"
#include "font_string_names.h"
#include "message_type_strings.h"
#include "text.h"
#include "viewport.h"
#include "web_socket_client.h"

#include "dashboard_page.h"

void ConfirmFullScanPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<HBoxContainer> hboxcontainer = std::make_shared<HBoxContainer>();
	hboxcontainer->set_alignment(BoxContainer::AlignmentMode::CENTER);
	hboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	hboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(hboxcontainer);
	hboxcontainer->set_position(top_left_position);
	hboxcontainer->set_size(full_size);

	{
		std::shared_ptr<CenterContainer> arrow_center_container = std::make_shared<CenterContainer>();
		arrow_center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		hboxcontainer->add_component(arrow_center_container);

		std::shared_ptr<ArrowShape> arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::LEFT);
		arrow_center_container->add_component(arrow);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		hboxcontainer->add_component(center_container);

		std::shared_ptr<Text> text = std::make_shared<Text>();
		text->set_font(FontStringName(roboto_condensed_13));
		text->set_text("CONFIRM FULL SCAN");
		center_container->add_component(text);
	}
}

void ConfirmFullScanPage::input_event(std::shared_ptr<InputEvent> p_input_event) {
	if (p_input_event->is_pressed && p_input_event->key == KeyCode::ENTER) {
		WebSocketClient::Message msg(MessageTypeString(lcd_full_scan));
		WebSocketClient::get_singleton()->send_message(msg);

		Input::get_singleton()->set_input_as_handled();
	} else if (p_input_event->is_pressed && p_input_event->key == KeyCode::CANCEL) {
		get_viewport()->set_root(std::make_shared<DashboardPage>());
		Input::get_singleton()->set_input_as_handled();
	}
}

ConfirmFullScanPage::ConfirmFullScanPage() {
}

ConfirmFullScanPage::~ConfirmFullScanPage() {
	std::cout << "ConfirmFullScanPage::~ConfirmFullScanPage\n";
}
