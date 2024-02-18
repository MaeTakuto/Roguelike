#include "../..//dxlib_ext/dxlib_ext.h"
#include "../ui/message_window.h"
#include "../ui/hp_bar.h"
#include "ui_manager.h"

UI_Manager::UI_Manager() {

	message_window_ = std::make_shared<MessageWindow>();
	select_window_ = std::make_shared<SelectWindow>();
	hp_bar_ = std::make_shared<HP_Bar>();
}

UI_Manager::~UI_Manager() {

	tnl::DebugTrace("UI_Manager�̃f�X�g���N�^�����s����܂���\n");
}

void UI_Manager::update(float delta_time) {

	if (message_window_->isEnable()) message_window_->update(delta_time);
	if (select_window_->isEnable()) select_window_->update(delta_time);
}

void UI_Manager::draw(const std::shared_ptr<Camera> camera) {

	if (message_window_->isEnable()) message_window_->draw(camera);
	if (select_window_->isEnable()) select_window_->draw(camera);
	hp_bar_->draw(camera);
}

// �K�i�ł̑I�������̎��s
void UI_Manager::executeStairSelect() {
	select_window_->setEnable(true);
	message_window_->cancelTimeLimit();
	message_window_->clearMessage();
	message_window_->setWindowPos(STAIR_SEL_MESS_WINDOW_POS);
	message_window_->setWindowSize(STAIR_SEL_MESS_WINDOW_SIZE);
	message_window_->setMessgae(STAIR_SEL_MESSAGE);
	message_window_->setEneble(true);
}

// �K�i�ł̑I���I���̏����̎��s
void UI_Manager::executeStairSelectEnd() {
	select_window_->setEnable(false);
	select_window_->init();
	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize(DEFAULT_MESS_WINDOW_SIZE);
	message_window_->clearMessage();
	message_window_->setEneble(false);
}
