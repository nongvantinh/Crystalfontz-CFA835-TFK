#ifndef APPLICATION_H
#define APPLICATION_H

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

#include "managers/device_manager.h"
#include "managers/display_server.h"

class Component;
class Viewport;
class InputEvent;

class Application {
private:
	float fps;
	std::atomic<bool> is_running;
	std::shared_ptr<Viewport> viewport;
	DeviceManager *device_manager;
	DisplayServer *display_server;

public:
	Application(int p_fps) :
			fps(p_fps), is_running(false) {}

	static const float DEFAULT_FPS;
	static Application *get_singleton();

	float get_fps() const;
	void set_fps(float p_fps);

	std::shared_ptr<Viewport> get_viewport();

	void start();
	void stop();
	void process_input_event(std::shared_ptr<InputEvent> p_event);

private:
	void run();
	void update();
};
#endif // APPLICATION_H