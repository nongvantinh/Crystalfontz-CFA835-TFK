#include "viewport.h"
#include "arrow_shape.h"
#include "font_string_names.h"
#include "managers/display_server.h"
#include "message_type_strings.h"
#include "web_socket_client.h"

#include "views/booting_page.h"
#include "views/license_activation_page.h"
#include "views/network_connection_page.h"

#include "views/auto_full_scan_page.h"
#include "views/dashboard_page.h"

#include "views/disks_page.h"

#include "views/engines_page.h"

#include "views/reports_page.h"

#include "views/settings_page.h"

#include "views/workflows_page.h"

#include <iostream>

Viewport::Viewport() {
	WebSocketClient::get_singleton()->register_message(MessageTypeString(notify_lcd_on_ui_dashboard), std::bind(&Viewport::on_open_dashboard_page, this, std::placeholders::_1));
	WebSocketClient::get_singleton()->register_message(MessageTypeString(notify_lcd_on_ui_engines_page), std::bind(&Viewport::on_open_engines_page, this, std::placeholders::_1));
	WebSocketClient::get_singleton()->register_message(MessageTypeString(notify_lcd_on_ui_disks_page), std::bind(&Viewport::on_open_disks_page, this, std::placeholders::_1));
	WebSocketClient::get_singleton()->register_message(MessageTypeString(results), std::bind(&Viewport::on_open_reports_page, this, std::placeholders::_1));
	WebSocketClient::get_singleton()->register_message(MessageTypeString(get_all_workflow), std::bind(&Viewport::on_open_workflows_page, this, std::placeholders::_1));
	WebSocketClient::get_singleton()->register_message(MessageTypeString(notify_lcd_on_ui_settings), std::bind(&Viewport::on_open_settings_page, this, std::placeholders::_1));
}

Viewport::~Viewport() {
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(notify_lcd_on_ui_dashboard));
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(notify_lcd_on_ui_engines_page));
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(notify_lcd_on_ui_disks_page));
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(results));
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(get_all_workflow));
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(notify_lcd_on_ui_settings));
}

void Viewport::initialize() {
	set_name("Viewport");
	queue_render();

	ws_client = WebSocketClient::get_singleton();
	ws_client->connect("localhost:8081/ws");
	ws_client->run();

	add_component(std::make_shared<NetworkConnectionPage>());

	set_root(std::make_shared<DashboardPage>());
}

void Viewport::notification(UNUSED_PARAM Notification p_which) {
	switch (p_which) {
		case Notification::DRAW:
			queue_render();
			break;

		default:
			break;
	}
}

void Viewport::render() {
	std::cout << get_name() << ":Viewport::render::get_size" << get_size() << std::endl;
}

void Viewport::input_event(UNUSED_PARAM std::shared_ptr<InputEvent> p_input_event) {
}

void Viewport::set_root(std::shared_ptr<Component> p_component) {
	if (root != nullptr) {
		remove_component(root);
	}
	root = p_component;
	add_component(root);
}

std::shared_ptr<Component> Viewport::get_root() {
	return root;
}

void Viewport::update(UNUSED_PARAM float p_delta) {
	ws_client->process_all_messages();
}

void Viewport::on_open_dashboard_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<DashboardPage>());
}

void Viewport::on_open_engines_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<EnginesPage>());
}

void Viewport::on_open_disks_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<DisksPage>());
}

void Viewport::on_open_reports_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<ReportsPage>());
}

void Viewport::on_open_workflows_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<WorkflowsPage>());
}

void Viewport::on_open_settings_page(UNUSED_PARAM const WebSocketClient::Message &p_message) {
	set_root(std::make_shared<SettingsPage>());
}
