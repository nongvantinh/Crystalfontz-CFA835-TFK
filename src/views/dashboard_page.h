#ifndef DASHBOARD_PAGE_H
#define DASHBOARD_PAGE_H

#include <functional>
#include <vector>

#include "component.h"
#include "web_socket_client.h"

class ArrowShape;
class Text;
class DashboardPage : public Component {
public:
	DashboardPage();
	~DashboardPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;
	void notification(UNUSED_PARAM Notification p_which);
	void input_event(std::shared_ptr<InputEvent> p_input_event) override;

private:
	void increment_index();
	void decrement_index();
	void on_full_scan_selected();
	void on_quick_scan_selected();
	void on_shutdown_selected();

	void on_session_message_received(const WebSocketClient::Message &p_message);
	void on_trigger_scan_interface_early_message_received(const WebSocketClient::Message &p_message);


	size_t current_index = 0;
	std::vector<std::string> tabs;
	std::map<std::string, std::function<void()>> tab_callbacks;
	std::shared_ptr<ArrowShape> up_arrow;
	std::shared_ptr<ArrowShape> down_arrow;
	std::shared_ptr<Text> option;
};

#endif //DASHBOARD_PAGE_H