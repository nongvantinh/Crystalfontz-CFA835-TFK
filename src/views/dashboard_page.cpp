#include "dashboard_page.h"
#include "arrow_shape.h"
#include "container.h"
#include "font_string_names.h"
#include "text.h"
#include "viewport.h"

#include "confirm_full_scan_page.h"
#include "initializing_scan_page.h"

#include "message_type_strings.h"
#include "web_socket_client.h"

DashboardPage::DashboardPage() :
		tabs(), tab_callbacks() {
	std::initializer_list<std::string> options = { "FULL SCAN", "QUICK SCAN", "SHUTDOWN" };
	tabs.insert(tabs.end(), options);

	tab_callbacks[tabs[0]] = std::bind(&DashboardPage::on_full_scan_selected, this);
	tab_callbacks[tabs[1]] = std::bind(&DashboardPage::on_quick_scan_selected, this);
	tab_callbacks[tabs[2]] = std::bind(&DashboardPage::on_shutdown_selected, this);

	WebSocketClient::get_singleton()->register_message(MessageTypeString(trigger_scan_interface_early), std::bind(&DashboardPage::on_trigger_scan_interface_early_message_received, this, std::placeholders::_1));
}

DashboardPage::~DashboardPage() {
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(trigger_scan_interface_early));
	std::cout << "DashboardPage::~DashboardPage\n";
}

void DashboardPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);
	std::shared_ptr<VBoxContainer> container = std::make_shared<VBoxContainer>();
	add_component(container);
	container->set_position(top_left_position);
	container->set_size(full_size);

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		container->add_component(center_container);
		up_arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::UP);
		center_container->add_component(up_arrow);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		container->add_component(center_container);
		option = std::make_shared<Text>();
		option->set_font(FontStringName(roboto_condensed_16));
		option->set_text(tabs[current_index]);
		center_container->add_component(option);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		container->add_component(center_container);
		down_arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::DOWN);
		center_container->add_component(down_arrow);
	}
}

void DashboardPage::shutdown() {
}

void DashboardPage::update(UNUSED_PARAM float p_delta) {
	if (current_index == 0) {
		up_arrow->set_drawable(false);
		down_arrow->set_drawable(true);
	} else if (current_index == tabs.size() - 1) {
		up_arrow->set_drawable(true);
		down_arrow->set_drawable(false);
	}
}

void DashboardPage::notification(UNUSED_PARAM Notification p_which) {
}

void DashboardPage::input_event(UNUSED_PARAM std::shared_ptr<InputEvent> p_input_event) {
	if (p_input_event->is_pressed && p_input_event->key == KeyCode::DOWN) {
		increment_index();
		option->set_text(tabs[current_index]);
		Input::get_singleton()->set_input_as_handled();
	} else if (p_input_event->is_pressed && p_input_event->key == KeyCode::UP) {
		decrement_index();
		option->set_text(tabs[current_index]);
		Input::get_singleton()->set_input_as_handled();
	} else if (p_input_event->is_pressed && p_input_event->key == KeyCode::ENTER) {
		std::string selected_tab = tabs[current_index];
		tab_callbacks[selected_tab]();
		Input::get_singleton()->set_input_as_handled();
	}
}

void DashboardPage::on_session_message_received(const WebSocketClient::Message &p_message) {
	bool is_license_active = p_message.payload["license_active"].get<bool>();
	std::cout << "On session message received\n";
	std::cout << "License active: " << (is_license_active ? "True" : "False") << std::endl;
}

void DashboardPage::on_trigger_scan_interface_early_message_received(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	get_viewport()->set_root(std::make_shared<InitializingScanPage>());
}

void DashboardPage::increment_index() {
	if (!tabs.empty()) {
		if (current_index < tabs.size() - 1) {
			current_index++;
		}
	}
}

void DashboardPage::decrement_index() {
	if (!tabs.empty()) {
		if (current_index > 0) {
			current_index--;
		}
	}
}

void DashboardPage::on_full_scan_selected() {
	get_viewport()->set_root(std::make_shared<ConfirmFullScanPage>());
}

void DashboardPage::on_quick_scan_selected() {
	std::cout << "DashboardPage::on_quick_scan_selected\n";
}

void DashboardPage::on_shutdown_selected() {
	std::cout << "DashboardPage::on_shutdown_selected\n";
}
