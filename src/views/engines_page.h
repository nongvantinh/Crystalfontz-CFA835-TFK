#ifndef ENGINES_PAGE_H
#define ENGINES_PAGE_H

#include "component.h"
#include "web_socket_client.h"

#include <vector>

class Text;
class ArrowShape;
class EnginesPage : public Component {
public:
	EnginesPage();
	~EnginesPage();
	void initialize() override;
	void shutdown() override;
	void update(UNUSED_PARAM float p_delta) override;

private:
	void on_engine_status_message_received(const WebSocketClient::Message &p_message);

	std::shared_ptr<Text> header;
	std::shared_ptr<ArrowShape> arrow;
	std::shared_ptr<Text> engine_name;
	int32_t num_total_engines = 0;
	int32_t num_loaded_engines = 0;
	std::vector<std::string /*engine name*/> loaded_engines;
	float min_display_time = 0.2;
};

#endif // ENGINES_PAGE_H