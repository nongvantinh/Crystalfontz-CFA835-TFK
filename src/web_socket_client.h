#ifndef WEB_SOCKET_CLIENT_H
#define WEB_SOCKET_CLIENT_H

#include "json.hpp"
#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_no_tls_client.hpp"
#include <boost/lockfree/spsc_queue.hpp>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>

class WebSocketClient {
public:
	// pull out the type of messages sent by our config
	typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

	struct Message {
		websocketpp::connection_hdl connection;
		std::string type;
		nlohmann::json payload;
		Message(std::string p_type) :
				type(p_type){};
		Message() = default;
		Message(websocketpp::connection_hdl p_connection, std::string p_type, nlohmann::json p_payload) :
				connection(p_connection), type(p_type), payload(p_payload) {}
	};

	WebSocketClient();
	~WebSocketClient();
	static WebSocketClient *get_singleton();

	void process_all_messages();
	void send_message(Message p_message);
	void connect(std::string url);
	void run();
	void register_message(std::string p_type, const std::function<void(const Message &)> &handler);
	void deregister_message(std::string p_type);

private:
	// This message handler will be invoked once for each incoming message. It
	// prints the message and then sends a copy of the message back to the server.
	void on_message(websocketpp::connection_hdl p_connection_handler, message_ptr p_message);

	std::thread receive_thread;
	websocketpp::client<websocketpp::config::asio_client> wsclient;
	std::map<std::string, std::function<void(const Message &)>> message_handler_callbacks;
	static const int DEFAULT_QUEUE_SIZE;
	boost::lockfree::spsc_queue<Message> message_queue;
	std::shared_ptr<websocketpp::connection<websocketpp::config::asio_client>> server_connection;
};

#endif // WEB_SOCKET_CLIENT_H