#include "network_connection_page.h"

#include "message_type_strings.h"

NetworkConnectionPage::NetworkConnectionPage() {
}

NetworkConnectionPage::~NetworkConnectionPage() {
}

void NetworkConnectionPage::initialize() {
	WebSocketClient::get_singleton()->register_message(MessageTypeString(network_connection), std::bind(&NetworkConnectionPage::on_received_network_connection, this, std::placeholders::_1));
}

void NetworkConnectionPage::shutdown() {
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(network_connection));
}

void NetworkConnectionPage::update(UNUSED_PARAM float p_delta) {
}

void NetworkConnectionPage::on_received_network_connection(const WebSocketClient::Message &p_message) {
	std::cout << "NetworkConnectionPage::on_received_network_connection\n";
	std::cout << "p_message: " << p_message.payload << std::endl;
}
