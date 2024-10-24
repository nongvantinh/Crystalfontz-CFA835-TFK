#ifndef APPLICATION_H
#define APPLICATION_H

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "display_server.h"

class Component;
class Viewport;
class InputEvent;

class Application {
private:
	float fps;
	std::atomic<bool> is_running;
	std::shared_ptr<Viewport> viewport;
	DisplayServer *display_server;
	std::unordered_map<std::string, std::shared_ptr<BaseDevice>> connected_devices;
	std::vector<std::string> excluded_ports;

public:
	Application(int p_fps) :
			fps(p_fps), is_running(false) {}

	static const float DEFAULT_FPS;
	static Application *get_singleton();

	float get_fps() const;
	void set_fps(float p_fps);

	std::shared_ptr<Viewport> get_viewport();
	// Devices can be plugged or unplugged anytime, so don't store
	// a cache reference to the function's result. Use the returned value directly.
	std::vector<std::shared_ptr<BaseDevice>> get_connected_devices();

	void start();
	void stop();
	void process_input_event(std::shared_ptr<InputEvent> p_event);
	void detect_device_event();

private:
	void run();
	void update();
};
#endif // APPLICATION_H