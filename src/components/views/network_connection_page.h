#ifndef NETWORK_CONNECTION_PAGE_H
#define NETWORK_CONNECTION_PAGE_H

#include "component.h"

#include "web_socket_client.h"

class NetworkConnectionPage : public Component {
private:
public:
	NetworkConnectionPage();
	~NetworkConnectionPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;

	void on_received_network_connection(const WebSocketClient::Message &p_message);
};

#endif // NETWORK_CONNECTION_PAGE_H