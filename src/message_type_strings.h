#ifndef MESSAGE_TYPE_STRINGS_H
#define MESSAGE_TYPE_STRINGS_H

#include <string>

class MessageTypeStrings {
public:
	MessageTypeStrings();
	static MessageTypeStrings *get_singleton();

	// Declare the variables as extern
	std::string network_connection;
	std::string battery_state;
	std::string results;
	std::string report_list;
	std::string notification_center;
	std::string session;
	std::string engine_status;
	std::string trigger_scan;
	std::string trigger_scan_with_workflow;
	std::string trigger_custom_scan;
	std::string cancel_scan;
	std::string query_scan;
	std::string scan_finished;
	std::string select_report;
	std::string set_eula_sample_sharing;
	std::string generate_reports;
	std::string activate_license;
	std::string exit_app;
	std::string check_for_update;
	std::string trigger_copy_file;
	std::string cancel_copy_file;
	std::string open_wifi_setting;
	std::string generate_support_package;
	std::string cancel_generate_support_package;
	std::string get_all_workflow;

	// Frontend messages
	std::string modal_triger_scan_error_closed;
	std::string notify_lcd_on_ui_dashboard;
	std::string notify_lcd_on_ui_settings;
	std::string notify_lcd_on_ui_engines_page;
	std::string notify_lcd_on_ui_disks_page;
	std::string notify_lcd_leaving_ui_settings;
	std::string notify_lcd_ui_select_scan_mode;
	std::string notify_lcd_leaving_ui_dashboard;
	std::string notify_lcd_show_setup_modal;
	std::string notify_lcd_close_setup_modal;
	std::string notify_lcd_back_to_report;
	std::string trigger_scan_interface_early;

	// LCD request via UI
	std::string lcd_quick_scan;
	std::string lcd_full_scan;
};

#define MessageTypeString(m_name) MessageTypeStrings::get_singleton()->m_name

#endif // MESSAGE_TYPE_STRINGS_H