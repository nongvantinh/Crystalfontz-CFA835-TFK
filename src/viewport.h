#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "component.h"
#include "web_socket_client.h"

#include <memory>

class Viewport : public Component {
private:
	std::shared_ptr<Component> root;
	WebSocketClient *ws_client;
	void on_open_dashboard_page(const WebSocketClient::Message &p_message);
	void on_open_engines_page(const WebSocketClient::Message &p_message);
	void on_open_disks_page(const WebSocketClient::Message &p_message);
	void on_open_reports_page(const WebSocketClient::Message &p_message);
	void on_open_workflows_page(const WebSocketClient::Message &p_message);
	void on_open_settings_page(const WebSocketClient::Message &p_message);

public:
	Viewport();
	~Viewport();
	void initialize() override;
	void update(float p_delta) override;
	void notification(UNUSED_PARAM Notification p_which);
	void input_event(std::shared_ptr<InputEvent> p_input_event) override;
	void render() override;

	void set_root(std::shared_ptr<Component> p_component);
	std::shared_ptr<Component> get_root();
};

#endif // VIEWPORT_H