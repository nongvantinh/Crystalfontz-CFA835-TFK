#include "workflows_page.h"

#include "container.h"
#include "font_string_names.h"
#include "text.h"

WorkflowsPage::WorkflowsPage() {
}

WorkflowsPage::~WorkflowsPage() {
	std::cout << "WorkflowsPage::~WorkflowsPage\n";
}

void WorkflowsPage::initialize() {
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
	text->set_font(FontStringName(roboto_condensed_13));
	text->set_text("VIEWING WORKFLOWS");
	center_container->add_component(text);
}

void WorkflowsPage::shutdown() {
}

void WorkflowsPage::update(UNUSED_PARAM float p_delta) {
}
