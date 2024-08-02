#include "web_socket_client.h"

#include <iostream>
#include <utility>

const int WebSocketClient::DEFAULT_QUEUE_SIZE = 200;
WebSocketClient::WebSocketClient() :
		wsclient(), message_queue(DEFAULT_QUEUE_SIZE) {
}

WebSocketClient::~WebSocketClient() {
	if (receive_thread.joinable()) {
		receive_thread.join();
	}
}

WebSocketClient *WebSocketClient::get_singleton() {
	static WebSocketClient self;
	return &self;
}

void WebSocketClient::on_message(websocketpp::connection_hdl p_connection_handler, message_ptr p_message) {
	nlohmann::json data = nlohmann::json::parse(p_message->get_payload());
	// std::cout << "on_message: \"" << data << "\"" << std::endl;
	std::string msg_type = data["msg_type"];
	nlohmann::json payload = data["payload"];
	// std::cout << "Client received message: \"" << msg_type << "\"" << std::endl;
	// std::cout << payload << std::endl;

	WebSocketClient::Message msg = WebSocketClient::Message(p_connection_handler, msg_type, payload);
	message_queue.push(msg);
}

void WebSocketClient::process_all_messages() {
	while (!message_queue.empty()) {
		WebSocketClient::Message message;
		if (message_queue.pop(message)) {
			std::cout << "[lcd] process message: " << message.type << std::endl;

			if (message_handler_callbacks.count(message.type)) {
				message_handler_callbacks[message.type](message);
			}
		}
	}
}

void WebSocketClient::send_message(Message p_message) {
	nlohmann::json msg = {
		{ "msg_type", p_message.type },
		{ "payload", p_message.payload }
	};

	std::string out_message = msg.dump() + "\n\n";

	websocketpp::lib::error_code ec;
	wsclient.send(server_connection, out_message, websocketpp::frame::opcode::TEXT, ec);
	if (ec) {
		std::cerr << "Error sending message: " << ec.message() << std::endl;
	}
}

void WebSocketClient::connect(std::string url) {
	std::string uri = "ws://" + url;
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		wsclient.set_access_channels(websocketpp::log::alevel::all);
		wsclient.clear_access_channels(websocketpp::log::alevel::frame_payload);

		wsclient.init_asio();

		wsclient.set_message_handler([this](websocketpp::connection_hdl hdl, std::shared_ptr<websocketpp::message_buffer::message<websocketpp::message_buffer::alloc::con_msg_manager>> msg) {
			on_message(hdl, msg);
		});

		websocketpp::lib::error_code ec;
		websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = wsclient.get_connection(uri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return;
		}

		std::cout << "Starting web socket client..." << std::endl;
		server_connection = wsclient.connect(con);
	} catch (websocketpp::exception const &e) {
		std::cout << e.what() << std::endl;
	}
}

void WebSocketClient::run() {
	receive_thread = std::thread([this]() {
		wsclient.run();
	});
	if (receive_thread.joinable()) {
		receive_thread.detach();
	}
}

void WebSocketClient::register_message(std::string p_type, const std::function<void(const Message &)> &handler) {
	if (message_handler_callbacks.find(p_type) == message_handler_callbacks.end()) {
		message_handler_callbacks[p_type] = handler;
	} else {
		throw std::runtime_error("The message \"" + p_type + "\" has already been registered");
	}
}

void WebSocketClient::deregister_message(std::string p_type) {
	auto it = message_handler_callbacks.find(p_type);
	if (it != message_handler_callbacks.end()) {
		message_handler_callbacks.erase(it);
	} else {
		throw std::runtime_error("The message \"" + p_type + "\" has NOT been registered");
	}
}
