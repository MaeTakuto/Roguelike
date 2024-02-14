#include "../..//dxlib_ext/dxlib_ext.h"
#include "../ui/message_window.h"
#include "ui_manager.h"

UI_Manager::UI_Manager() {

	message_window_ = std::make_shared<MessageWindow>();
}

UI_Manager::~UI_Manager() {

	tnl::DebugTrace("UI_Managerのデストラクタが実行されました\n");
}

void UI_Manager::update(float delta_time) {

	if (message_window_->isDisplay()) message_window_->update(delta_time);
}

void UI_Manager::draw(const std::shared_ptr<Camera> camera) {

	if (message_window_->isDisplay()) message_window_->draw(camera);
}