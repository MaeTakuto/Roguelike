#include "../..//dxlib_ext/dxlib_ext.h"
#include "../ui/message_window.h"
#include "../ui/hp_bar.h"
#include "ui_manager.h"

namespace {
	// ���b�Z�[�W�E�B���h�E�̒ʏ�̈ʒu�A�T�C�Y
	const tnl::Vector2i	DEFAULT_MESS_WINDOW_POS = { 175, 500 };
	const tnl::Vector2i DEFAULT_MESS_WINDOW_SIZE = { 900, 200 };

	// �K�i�ł̑I�����̃��b�Z�[�W�E�B���h�E�̈ʒu�A�T�C�Y�A���b�Z�[�W
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_POS = { 150, 450 };
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_SIZE = { 750, 200 };
	const std::string STAIR_SEL_MESSAGE = "��������B�����܂����H";

	// �K���̕\���ʒu
	const tnl::Vector2i FLOOR_STR_POS = { 1000, 55 };
	const int FLOOR_STR_FONT_SIZE = 40;
}

UI_Manager::UI_Manager() : message_window_(std::make_shared<MessageWindow>()), 
select_window_(std::make_shared<SelectWindow>()), hp_bar_(std::make_shared<HP_Bar>()), floor_(0) 
{

	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize(DEFAULT_MESS_WINDOW_SIZE);
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
	SetFontSize(FLOOR_STR_FONT_SIZE);
	DrawStringEx(FLOOR_STR_POS.x, FLOOR_STR_POS.y, -1, "%dF", floor_);
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

// ���b�Z�[�W�E�B���h�E�Ƀ��b�Z�[�W���Z�b�g
void UI_Manager::setMessage(const std::string& message, float draw_time) {

	// �\�����Ԃ𐧌�����ꍇ
	if (draw_time > 0.0f) {
		message_window_->setMessgae(message);
		message_window_->setTimeLimit(draw_time);
		return;
	}
	// �ʏ�̏ꍇ
	message_window_->setMessgae(message);
	message_window_->setEneble(true);
}

void UI_Manager::clearMessage() {
	message_window_->clearMessage();
}

// HP�o�[�ɃX�e�[�^�X���Z�b�g
void UI_Manager::setHP_BarStatus(int max_hp, int hp) {

	hp_bar_->setMaxHP(max_hp);
	hp_bar_->setHP(hp);
	hp_bar_->updateHP_Text();
}
