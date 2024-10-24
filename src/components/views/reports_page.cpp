#include "reports_page.h"

#include "core/container.h"

#include "core/text.h"

ReportsPage::ReportsPage() {
}

ReportsPage::~ReportsPage() {
	std::cout << "ReportsPage::~ReportsPage\n";
}

void ReportsPage::initialize() {
	Vector2i top_left_position = get_parent()->get_position();
	Vector2i full_size = get_parent()->get_size();

	set_position(top_left_position);
	set_size(full_size);

	std::shared_ptr<CenterContainer> center_container = std::make_shared<CenterContainer>();
	center_container->set_h_size_flags(Component::SizeFlags::EXPAND_FILL);
	center_container->set_v_size_flags(Component::SizeFlags::EXPAND_FILL);
	add_component(center_container);
	center_container->set_position(top_left_position);
	center_container->set_size(full_size);

	std::shared_ptr<Text> text = std::make_shared<Text>();
	text->set_font(Font::ROBOTO_LIGHT);
	text->set_text("VIEWING REPORTS PAGE");
	center_container->add_component(text);
}

void ReportsPage::shutdown() {
}

void ReportsPage::update(UNUSED_PARAM float p_delta) {
}
