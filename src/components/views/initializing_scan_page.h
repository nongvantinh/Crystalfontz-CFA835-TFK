#ifndef INITIALIZING_SCAN_PAGE_H
#define INITIALIZING_SCAN_PAGE_H

#include "component.h"

#include "web_socket_client.h"

class InitializingScanPage : public Component {
public:
	InitializingScanPage();
	~InitializingScanPage();
	void initialize() override;

private:
	void on_trigger_scan_message_received(const WebSocketClient::Message &p_message);
};

#endif // INITIALIZING_SCAN_PAGE_H