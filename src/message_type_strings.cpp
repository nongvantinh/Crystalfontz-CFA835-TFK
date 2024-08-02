#include "message_type_strings.h"

MessageTypeStrings *MessageTypeStrings::get_singleton() {
	static MessageTypeStrings self;
	return &self;
}

MessageTypeStrings::MessageTypeStrings() {
	network_connection = "NetworkConnection";
	battery_state = "BatteryState";
	results = "Results";
	report_list = "ReportList";
	notification_center = "NotificationCenter";
	session = "Session";
	engine_status = "EngineStatus";
	trigger_scan = "TriggerScan";
	trigger_scan_with_workflow = "TriggerScanWithWorkflow";
	trigger_custom_scan = "TriggerCustomScan";
	cancel_scan = "CancelScan";
	query_scan = "QueryScan";
	scan_finished = "ScanFinished";
	select_report = "SelectReport";
	set_eula_sample_sharing = "SetEulaSampleSharing";
	generate_reports = "GenerateReports";
	activate_license = "ActivateLicense";
	exit_app = "ExitApp";
	check_for_update = "CheckForUpdate";
	trigger_copy_file = "TriggerCopyFile";
	cancel_copy_file = "CancelCopyFile";
	open_wifi_setting = "OpenWifiSetting";
	generate_support_package = "GenerateSupportPackage";
	cancel_generate_support_package = "CancelGenerateSupportPackage";
	get_all_workflow = "GetAllWorkFlow";
    
	// Frontend messages
	modal_triger_scan_error_closed = "ModalTrigerScanErrorClosed";
	notify_lcd_on_ui_dashboard = "NotifyLcdOnUiDashboard";
	notify_lcd_on_ui_settings = "NotifyLcdOnUiSettings";
	notify_lcd_on_ui_engines_page = "NotifyLcdOnUiEnginesPage";
	notify_lcd_on_ui_disks_page = "NotifyLcdOnUiDisksPage";
	notify_lcd_leaving_ui_settings = "NotifyLcdLeavingUiSettings";
	notify_lcd_ui_select_scan_mode = "NotifyLcdUiSelectScanMode";
	notify_lcd_leaving_ui_dashboard = "NotifyLcdLeavingUiDashboard";
	notify_lcd_show_setup_modal = "NotifyLcdShowSetupModal";
	notify_lcd_close_setup_modal = "NotifyLcdCloseSetupModal";
	notify_lcd_back_to_report = "NotifyLcdBackToReport";
	trigger_scan_interface_early = "TriggerScanInterface_early";

	// LCD request via UI
	lcd_quick_scan = "LCDQuickScan";
	lcd_full_scan = "LCDFullScan";
}
