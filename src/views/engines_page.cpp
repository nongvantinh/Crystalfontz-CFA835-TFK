#include "engines_page.h"

#include "arrow_shape.h"
#include "container.h"
#include "font_string_names.h"
#include "message_type_strings.h"
#include "stylized_progress_bar.h"
#include "text.h"
#include "web_socket_client.h"

#include <algorithm>
#include <functional>

EnginesPage::EnginesPage() {
	WebSocketClient::get_singleton()->register_message(MessageTypeString(engine_status), std::bind(&EnginesPage::on_engine_status_message_received, this, std::placeholders::_1));
}

EnginesPage::~EnginesPage() {
	WebSocketClient::get_singleton()->deregister_message(MessageTypeString(engine_status));
	std::cout << "EnginesPage::~EnginesPage\n";
}

void EnginesPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<VBoxContainer> vboxcontainer = std::make_shared<VBoxContainer>();
	vboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	vboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(vboxcontainer);
	vboxcontainer->set_position(top_left_position);
	vboxcontainer->set_size(full_size);

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		std::shared_ptr<StylizedProgressBar> progress_bar = std::make_shared<StylizedProgressBar>(3);
		center_container->add_component(progress_bar);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		header = std::make_shared<Text>();
		header->set_font(FontStringName(roboto_condensed_15));
		header->set_text("LOADING ENGINES 7/9");
		center_container->add_component(header);
	}

	{
		std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
		center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		vboxcontainer->add_component(center_container);

		std::shared_ptr<HBoxContainer> hboxcontainer = std::make_shared<HBoxContainer>();
		hboxcontainer->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
		hboxcontainer->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
		center_container->add_component(hboxcontainer);

		arrow = std::make_shared<ArrowShape>(ArrowShape::Direction::RIGHT);
		arrow->set_drawable(false);
		hboxcontainer->add_component(arrow);

		engine_name = std::make_shared<Text>();
		engine_name->set_font(FontStringName(roboto_condensed_13));
		engine_name->set_text("  ");
		hboxcontainer->add_component(engine_name);
	}
}

void EnginesPage::shutdown() {
}

void EnginesPage::update(UNUSED_PARAM float p_delta) {
}

void EnginesPage::on_engine_status_message_received(const WebSocketClient::Message &p_message) {
	num_total_engines = 0;
	num_loaded_engines = 0;
	for (const auto &engine : p_message.payload["engine_list"]) {
		std::string engine_type = engine["engine_type"].get<std::string>();
		std::string eng_name = engine["eng_name"].get<std::string>();
		bool active = engine["active"].get<bool>();

		if (engine_type != "av" && engine_type != "oesis" && engine_type != "dlp") {
			continue;
		}

		++num_total_engines;
		if (!active) {
			continue;
		}

		++num_loaded_engines;
		if (std::find(loaded_engines.begin(), loaded_engines.end(), eng_name) == loaded_engines.end()) {
			loaded_engines.push_back(eng_name);
		}
	}

	std::string text = "LOADING ENGINES " + std::to_string(num_loaded_engines) + "/" + std::to_string(num_total_engines);
	header->set_text(text);

	if (loaded_engines.size()) {
		std::string name = loaded_engines[loaded_engines.size() - 1];
		engine_name->set_text(name);
	}
}
