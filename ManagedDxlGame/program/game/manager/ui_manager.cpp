#include "../..//dxlib_ext/dxlib_ext.h"
#include "gm_manager.h"
#include "../common/enum.h"
#include "resource_manager.h"
#include "../ui/message_window.h"
#include "../ui/select_window.h"
#include "../ui/status_bar.h"
#include "../common/chara_status.h"
#include "../common/magic_status.h"
#include "../base/character_base.h"
#include "../base/magic_base.h"
#include "ui_manager.h"

namespace {
	// ���b�Z�[�W�E�B���h�E�̒ʏ�̈ʒu�A�T�C�Y
	const tnl::Vector2i	DEFAULT_MESS_WINDOW_POS = { 175, 500 };
	const tnl::Vector2i DEFAULT_MESS_WINDOW_SIZE = { 900, 160 };

	// �K�i�ł̑I�����̃��b�Z�[�W�E�B���h�E�̈ʒu�A�T�C�Y�A���b�Z�[�W
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_POS = { 150, 500 };
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_SIZE = { 750, 160 };

	// �K���̕\���ʒu
	const tnl::Vector2i FLOOR_STR_POS = { 750, 90 };
	const int FLOOR_STR_FONT_SIZE = 40;

	// �E�B���h�E�ƃE�B���h�E�̊Ԋu
	tnl::Vector2i WINDOW_SPACE = { 25, 25 };

	// �\������R�}���h���̃t�H���g�T�C�Y
	const int SELECT_WINDOW_CMD_FONT_SIZE = 30;
	// �R�}���h�Ǝ��̃R�}���h�Ƃ̕\���Ԋu
	const int SELECT_WINDOW_CMD_SPACE = 5;

	// ================= ��������E�B���h�E�̐ݒ� ===============================

	// ��������E�B���h�E�̕\���ʒu
	const tnl::Vector2i CONTROL_EXPLANATION_WINDOW_POS = { 850, 25 };
	// ��������E�B���h�E�̕\���T�C�Y
	const tnl::Vector2i CONTROL_EXPLANATION_WINDOW_SIZE = { 400, 25 };

	// ================= 2��I���E�B���h�E�i�͂��A�������j�̐ݒ� =================

	// �K�i�I���E�B���h�E�̕\���ʒu
	const tnl::Vector2i STAIR_SELECT_WINDOW_POS = { 950, 500 };
	// 2���E�B���h�E�̕\���T�C�Y
	const tnl::Vector2i TWO_SELECT_WINDOW_SIZE = { 160, 0 };

	// ================= ���C�����j���[�̐ݒ� ====================================

	// �I���E�B���h�E�̕\���ʒu
	const tnl::Vector2i MAIN_MENU_WINDOW_POS = { 100, 150 };
	// �I���E�B���h�E�̕\���ʒu
	const tnl::Vector2i MAIN_MENU_WINDOW_SIZE = { 300, 0 };

	// ================= �T�u���j���[�̐ݒ� ======================================

	// �T�u���j���[�̕\���ʒu
	const tnl::Vector2i SUB_WINDOW_POS = tnl::Vector2i( MAIN_MENU_WINDOW_POS.x + MAIN_MENU_WINDOW_SIZE.x + WINDOW_SPACE.x, MAIN_MENU_WINDOW_POS.y );
	// ���@�I�����j���[�̕\���T�C�Y
	const tnl::Vector2i MAGIC_WINDOW_SIZE = { 300, 0 };
	// 
	const tnl::Vector2i MAGIC_EXPLANTION_WINDOW_SIZE = { 450, 120 };

	// �X�e�[�^�X�o�[�̎n�_�ʒu
	const tnl::Vector2i STATUS_BAR_TOP_POS = { 200, 25 };
	// �X�e�[�^�X�o�[�̕\���T�C�Y
	const tnl::Vector2i STATUS_BAR_SIZE = { 500, 40 };
	// �X�e�[�^�X�o�[�̊Ԋu
	const tnl::Vector2i STATUS_BAR_SPACE = { 0, 20 };

}

// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
UI_Manager::UI_Manager() : message_window_(std::make_shared<MessageWindow>()), control_explanation_window_(std::make_shared<MessageWindow>()),
	view_status_window_(std::make_shared<MessageWindow>()), magic_explation_window_(std::make_shared<MessageWindow>()),
	two_select_window_(std::make_shared<SelectWindow>()), main_menu_select_window_(std::make_shared<SelectWindow>()), magic_select_window_(std::make_shared<SelectWindow>()),
	hp_bar_(std::make_shared<StatusBar>()), mp_bar_(std::make_shared<StatusBar>()), floor_(0), draw_message_window_time_(3.0f)
{
	// ------------------------- ���b�Z�[�W�E�B���h�E�̏����� -------------------------
	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize( tnl::Vector2i( DEFAULT_MESS_WINDOW_SIZE.x, DEFAULT_MESS_WINDOW_SIZE.y));

	// ------------------------- ��������E�B���h�E�̏����� ---------------------------

	// ��������e�L�X�g�̎擾
	CsvData& control_explanation_message = ResourceManager::getInstance()->loadCsvData("csv/control_explanation_data.csv");

	control_explanation_message_.resize(control_explanation_message.size() - 1);

	for (int y = GameManager::CSV_CELL_ROW_START; y < control_explanation_message.size(); ++y) {
		control_explanation_message_[y - 1].resize(control_explanation_message[y][0].getInt());

		for (int x = GameManager::CSV_CELL_ROW_START; x < control_explanation_message[y][0].getInt() + 1; ++x) {
			control_explanation_message_[y - 1][x - 1] = control_explanation_message[y][x].getString();
		}
	}

	// ��������E�B���h�E�̐ݒ�
	control_explanation_window_->setWindowPos(CONTROL_EXPLANATION_WINDOW_POS);
	control_explanation_window_->setMessageTopPos( tnl::Vector2i(10, 10) );
	control_explanation_window_->setWindowSize( tnl::Vector2i( CONTROL_EXPLANATION_WINDOW_SIZE.x, CONTROL_EXPLANATION_WINDOW_SIZE.y * control_explanation_message_[0].size() ) );
	control_explanation_window_->setFontSize(25);
	control_explanation_window_->setMessageSpace(0);
	control_explanation_window_->setMessageLine( control_explanation_message_[0].size() );
	
	// ��������e�L�X�g�𑀍�����E�B���h�E�ɃZ�b�g
	for (int i = 0; i < control_explanation_message_[0].size(); ++i) {
		control_explanation_window_->setMessgae(control_explanation_message_[0][i]);
	}

	control_explanation_window_->setEnable(true);

	// ------------------------- �X�e�[�^�X�m�F�E�B���h�E�̏����� ---------------------
	view_status_window_->setWindowPos( SUB_WINDOW_POS );
	view_status_window_->setWindowSize(tnl::Vector2i(400, 0));
	view_status_window_->setMessageLine(6);
	view_status_window_->calculateWindowSize();

	// ------------------------- 2��I���E�B���h�E�̏����� ----------------------------

	two_select_cmd_names_.resize(2);
	for (int i = 0; i < two_select_cmd_names_.size(); ++i) {
		two_select_cmd_names_[i].resize(std::underlying_type< eTwoSelectCmd >::type( eTwoSelectCmd::MAX ) );
	}
	two_select_cmd_names_[0][0] = "�͂�";
	two_select_cmd_names_[0][1] = "������";
	two_select_cmd_names_[1][0] = "�g��";
	two_select_cmd_names_[1][1] = "�g��Ȃ�";

	two_select_window_->setWindowPos( STAIR_SELECT_WINDOW_POS );
	two_select_window_->setWindowSize( tnl::Vector2i( TWO_SELECT_WINDOW_SIZE.x, TWO_SELECT_WINDOW_SIZE.y + ( SELECT_WINDOW_CMD_FONT_SIZE + SELECT_WINDOW_CMD_FONT_SIZE ) * ( two_select_cmd_names_.size() - 1 ) ) );
	two_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	two_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );
	two_select_window_->setSelectCmdMax( std::underlying_type<eTwoSelectCmd>::type( eTwoSelectCmd::MAX ) );
	two_select_window_->setCommandNames( two_select_cmd_names_[0] );
	two_select_window_->calculateWindowSize();

	// ------------------------- ���C�����j���[�E�B���h�E�̏����� ----------------------
	std::vector<std::string> select_cmd_names(static_cast<size_t>(eMainMenuCmd::MAX));

	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::MAGIC_SELECT) ]	= "���@���g��";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_CELL) ]		= "�������m�F";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_STATUS) ]	= "�X�e�[�^�X��\��";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CLOSE) ]			= "�Ƃ���";
	
	main_menu_select_window_->setWindowPos( MAIN_MENU_WINDOW_POS );
	main_menu_select_window_->setWindowSize(MAIN_MENU_WINDOW_SIZE);
	main_menu_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	main_menu_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );
	main_menu_select_window_->setSelectCmdMax( std::underlying_type<eMainMenuCmd>::type( eMainMenuCmd::MAX ) );
	main_menu_select_window_->setCommandNames( select_cmd_names );
	main_menu_select_window_->calculateWindowSize();

	// ------------------------- ���@�ꗗ�̑I���E�B���h�E�̏����� ----------------------
	magic_select_window_->setWindowPos( SUB_WINDOW_POS );
	magic_select_window_->setWindowSize( MAGIC_WINDOW_SIZE );
	magic_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	magic_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );
	magic_select_window_->calculateWindowSize();

	magic_explation_window_->setWindowPos( SUB_WINDOW_POS 
		+ tnl::Vector2i( MAGIC_WINDOW_SIZE.x, two_select_window_->getWindowSize().y )
		+ WINDOW_SPACE
	);

	magic_explation_window_->setWindowSize( MAGIC_EXPLANTION_WINDOW_SIZE );
	magic_explation_window_->setMessageLine(5);
	magic_explation_window_->setMessageTopPos(tnl::Vector2i(10, 10));
	magic_explation_window_->calculateWindowSize();

	// ------------------------- �X�e�[�^�X�o�[�̏����� --------------------------------
	hp_bar_->setStatusBarPos(STATUS_BAR_TOP_POS);
	// hp_bar_->setStatusBarSize(STATUS_BAR_SIZE);
	hp_bar_->setStatusTypeText("HP");

	mp_bar_->setStatusBarPos( STATUS_BAR_TOP_POS + tnl::Vector2i( 0, STATUS_BAR_SIZE.y ) + STATUS_BAR_SPACE );
	// mp_bar_->setStatusBarSize( STATUS_BAR_SIZE );
	mp_bar_->setStatusTypeText("MP");
	mp_bar_->setStatusBarColor(StatusBar::eStatusBarColor::LIGHT_BLUE);

}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
UI_Manager::~UI_Manager() {

	tnl::DebugTrace("UI_Manager�̃f�X�g���N�^�����s����܂���\n");
}

// =====================================================================================
// �A�b�v�f�[�g
// =====================================================================================
void UI_Manager::update(float delta_time) {

	if (message_window_->isEnable()) {
		message_window_->update(delta_time);
	}
	if (two_select_window_->isOperate()) {
		two_select_window_->update(delta_time);
	}
	if (main_menu_select_window_->isOperate()) {
		main_menu_select_window_->update(delta_time);
	}
	if (magic_select_window_->isOperate()) {
		magic_select_window_->update(delta_time);
	}
	if (magic_explation_window_->isEnable()) {
		magic_explation_window_->update(delta_time);
		updateMagicExplation();
	}
	if(view_status_window_->isEnable()) view_status_window_->update(delta_time);
}

// =====================================================================================
// �`��
// =====================================================================================
void UI_Manager::draw(const std::shared_ptr<Camera> camera) {

	if (message_window_->isEnable()) {
		message_window_->draw();
	}
	if (control_explanation_window_->isEnable()) {
		control_explanation_window_->draw();
	}
	if (two_select_window_->isDrawing()) {
		two_select_window_->draw();
	}
	if (main_menu_select_window_->isDrawing()) {
		main_menu_select_window_->draw();
	}
	if (magic_select_window_->isDrawing()) {
		magic_select_window_->draw();
	}
	if (magic_explation_window_->isEnable()) {
		magic_explation_window_->draw();
	}
	if (view_status_window_->isEnable()) {
		view_status_window_->draw();
	}
	hp_bar_->draw();
	mp_bar_->draw();

	auto player = ui_target_.lock();

	SetFontSize(FLOOR_STR_FONT_SIZE);
	DrawStringEx(FLOOR_STR_POS.x, FLOOR_STR_POS.y, -1, "%dF", floor_);

	if (player) {
		DrawStringEx(FLOOR_STR_POS.x - 20, FLOOR_STR_POS.y - FLOOR_STR_FONT_SIZE - 20, -1, "Lv%d", player->getStatus().getLevel());
	}
}

// =====================================================================================
// ���C�����j���[�R�}���h�̑I�����Ă���C���f�b�N�X��Ԃ�
// =====================================================================================
int UI_Manager::getSelectedIndexFromMainMenuCmd() { 
	return main_menu_select_window_->getSelectedCmdIndex();
}

// =====================================================================================
// ���C�����j���[���J��
// =====================================================================================
void UI_Manager::openMainMenu() {
	main_menu_select_window_->setDrawing(true);
	main_menu_select_window_->setOperate(true);
	//control_explanation_window_->setEnable(false);

	tnl::Vector2i ctrl_explanation_window_pos_ = tnl::Vector2i(MAIN_MENU_WINDOW_POS.x - 65, MAIN_MENU_WINDOW_POS.y + main_menu_select_window_->getWindowSize().y + 20);

	control_explanation_window_->setWindowPos(ctrl_explanation_window_pos_);
	control_explanation_window_->setMessageLine(control_explanation_message_[2].size());
	control_explanation_window_->setWindowSize(tnl::Vector2i(CONTROL_EXPLANATION_WINDOW_SIZE.x - 45, CONTROL_EXPLANATION_WINDOW_SIZE.y * control_explanation_message_[2].size()));
	control_explanation_window_->setMessageLine(control_explanation_message_[2].size());

	for (int i = 0; i < control_explanation_message_[2].size(); ++i) {
		control_explanation_window_->setMessgae(control_explanation_message_[2][i]);
	}

}

// =====================================================================================
// ���C�����j���[�����
// =====================================================================================
void UI_Manager::closeMainMenu() {
	main_menu_select_window_->setDrawing(false);
	main_menu_select_window_->setOperate(false);
	control_explanation_window_->setEnable(true);

}

// =====================================================================================
// ���@�I���E�B���h�E�̈ꗗ���X�V
// =====================================================================================
void UI_Manager::updateMagicList() {

	std::shared_ptr<Character> target = ui_target_.lock();

	if (!target) {
		return;
	}

	auto& magic_list = target->getMagicList();

	std::vector<std::string> select_cmd_names(magic_list.size());

	for (int i = 0; i < magic_list.size(); ++i) {
		select_cmd_names[i] = magic_list[i]->getMagicStatus()->getMagicName();
	}

	magic_select_window_->setWindowSize(tnl::Vector2i( MAGIC_WINDOW_SIZE.x, MAGIC_WINDOW_SIZE.y + SELECT_WINDOW_CMD_SPACE * ( select_cmd_names.size() - 1 ) ) );
	magic_select_window_->setSelectCmdMax(select_cmd_names.size());
	magic_select_window_->setCommandNames(select_cmd_names);
	magic_select_window_->calculateWindowSize();

}

// =====================================================================================
// ���@���X�g�̑I�����Ă���C���f�b�N�X��Ԃ�
// =====================================================================================
int UI_Manager::getSelectedIndexFromMagicListCmd() {
	return magic_select_window_->getSelectedCmdIndex();
}

// =====================================================================================
// ���@�I���E�B���h�E���J��
// =====================================================================================
void UI_Manager::openMagicListWindow() {
	
	std::shared_ptr<Character> target = ui_target_.lock();
	if (!target) {
		return;
	}

	updateMagicList();
	magic_select_window_->setDrawing(true);
	magic_select_window_->setOperate(true);
	main_menu_select_window_->setOperate(false);


	auto& magic_list = target->getMagicList();
	magic_explation_window_->setEnable(true);
	magic_explation_window_->setAllLineMessage( magic_list[magic_select_window_->getSelectedCmdIndex()]->getMagicExplantion() );
}

// =====================================================================================
// ���@�I���E�B���h�E�����
// =====================================================================================
void UI_Manager::closeMagicListWindow() {
	magic_select_window_->setDrawing(false);
	magic_select_window_->setOperate(false);
	main_menu_select_window_->setOperate(true);
	magic_explation_window_->setEnable(false);
}

void UI_Manager::updateMagicExplation() {
	
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W, eKeys::KB_S, eKeys::KB_UP, eKeys::KB_DOWN)) {
		std::shared_ptr<Character> target = ui_target_.lock();
		if (!target) {
			return;
		}
		auto& magic_list = target->getMagicList();
		magic_explation_window_->setAllLineMessage(magic_list[magic_select_window_->getSelectedCmdIndex()]->getMagicExplantion());
	}
}

// =====================================================================================
// ���@���g�p���邩�I������E�B���h�E���J��
// =====================================================================================
void UI_Manager::executeSletctToUseMagic() {
	magic_select_window_->setOperate(false);
	two_select_window_->setDrawing(true);
	two_select_window_->setOperate(true);
	two_select_window_->setWindowPos( SUB_WINDOW_POS + tnl::Vector2i( MAGIC_WINDOW_SIZE.x + WINDOW_SPACE.x, 0 ) );
	two_select_window_->setCommandNames(two_select_cmd_names_[1]);

}

// =====================================================================================
// ���@���g�p���邩�I������E�B���h�E�����
// =====================================================================================
void UI_Manager::executeSletctToUseMagicEnd() {
	magic_select_window_->setOperate(true);
	two_select_window_->setDrawing(false);
	two_select_window_->setOperate(false);
}

// =====================================================================================
// �X�e�[�^�X�E�B���h�E��\������
// =====================================================================================
void UI_Manager::displayStatusWindow() {
	view_status_window_->clearMessage();

	// ���݃X�e�[�^�X�̐ݒ�
	std::shared_ptr<Character> target = ui_target_.lock();
	if (target) {
		const CharaStatus& status = target->getStatus();
		view_status_window_->setMessgae( "���x���@�@�@�@�F" + std::to_string( status.getLevel() ) );
		view_status_window_->setMessgae( "HP   �@�@�@�@ �F" + std::to_string( status.getHP()) + " / " + std::to_string(status.getMaxHP()));
		view_status_window_->setMessgae( "MP   �@�@�@�@ �F" + std::to_string( status.getMP()) + " / " + std::to_string(status.getMaxMP()));
		view_status_window_->setMessgae( "�U���́@�@�@�@�F" + std::to_string( status.getAtk() ) );
		view_status_window_->setMessgae( "�o���l�@�@�@�@�F" + std::to_string( status.getExp() ) );
		view_status_window_->setMessgae( "���̃��x���܂ŁF" + std::to_string( target->getExpToNextLevel() ) );
	}

	view_status_window_->setEnable(true);
	main_menu_select_window_->setOperate(false);
}

// =====================================================================================
// �X�e�[�^�X�E�B���h�E���\���ɂ���
// =====================================================================================
void UI_Manager::hideStatusWindow() {
	view_status_window_->setEnable(false);
	main_menu_select_window_->setOperate(true);
}

// =====================================================================================
// 2���R�}���h�̌��ݑI�𒆂̃C���f�b�N�X��Ԃ�
// =====================================================================================
int UI_Manager::getSelectedIndexFromTwoSelectCmd() { 
	return two_select_window_->getSelectedCmdIndex(); 
}


// =====================================================================================
// �K�i�ł̑I�������̎��s
// =====================================================================================
void UI_Manager::executeSelectYesOrNoWindow(const std::string& message) {
	// ����̑I���E�B���h�E�̗L���ɂ���
	two_select_window_->setDrawing(true);
	two_select_window_->setOperate(true);
	two_select_window_->setWindowPos(STAIR_SELECT_WINDOW_POS);
	two_select_window_->setCommandNames(two_select_cmd_names_[0]);

	// ���b�Z�[�W�E�B���h�E�֘A�̏���
	message_window_->cancelTimeLimit();
	message_window_->clearMessage();
	message_window_->setWindowPos(STAIR_SEL_MESS_WINDOW_POS);
	message_window_->setWindowSize(STAIR_SEL_MESS_WINDOW_SIZE);
	message_window_->setMessgae(message);
	message_window_->setEnable(true);
}

// =====================================================================================
// �K�i�ł̑I���I���̏����̎��s
// =====================================================================================
void UI_Manager::finishSelectYesOrNoWindow() {
	// ���b�Z�[�W�E�B���h�E�֘A�̏���
	two_select_window_->setDrawing(false);
	two_select_window_->setOperate(false);
	
	// ���b�Z�[�W�E�B���h�E�֘A�̏���
	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize(DEFAULT_MESS_WINDOW_SIZE);
	message_window_->clearMessage();
	message_window_->setEnable(false);
}

// =====================================================================================
// �����ʃE�B���h�E�̕\����؂�ւ���
// =====================================================================================
void UI_Manager::changeCtrlExplanationWindowType(int window_type) {

	control_explanation_window_->setWindowPos(CONTROL_EXPLANATION_WINDOW_POS);
	control_explanation_window_->setWindowSize( 
		tnl::Vector2i( CONTROL_EXPLANATION_WINDOW_SIZE.x, CONTROL_EXPLANATION_WINDOW_SIZE.y * control_explanation_message_[window_type].size() ) 
	);
	control_explanation_window_->setMessageLine(control_explanation_message_[window_type].size());

	for (int i = 0; i < control_explanation_message_[window_type].size(); ++i) {
		control_explanation_window_->setMessgae(control_explanation_message_[window_type][i]);
	}
}

void UI_Manager::hideCtrlExplanationWindow() {
	control_explanation_window_->setEnable(false);
}

// =====================================================================================
// ���b�Z�[�W�E�B���h�E�Ƀ��b�Z�[�W���Z�b�g
// =====================================================================================
void UI_Manager::setMessage(const std::string& message, float draw_time) {

	// �\�����Ԃ𐧌�����ꍇ
	if (draw_time > 0.0f) {
		message_window_->setMessgae(message);
		message_window_->setTimeLimit(draw_time);
		return;
	}
	// �ʏ�̏ꍇ
	message_window_->setMessgae(message);
	message_window_->setEnable(true);
}

// =====================================================================================
// ���b�Z�[�W�E�B���h�E�̃��b�Z�[�W��S�폜����
// =====================================================================================
void UI_Manager::clearMessage() {
	message_window_->clearMessage();
}

void UI_Manager::setEnableEnterUI(bool is_enable) {
	message_window_->setEnableEnterUI(is_enable);
}

// =====================================================================================
// �X�e�[�^�X�o�[�̒l���X�V
// =====================================================================================
void UI_Manager::updateStatusBar() {

	std::shared_ptr<Character> target = ui_target_.lock();
	
	if (!target) {
		tnl::DebugTrace("target�̎Q�Ƃ������ł�\n");
		return;
	}
		
	const CharaStatus& status = target->getStatus();

	hp_bar_->updateStatus_Text(status.getMaxHP(), status.getHP());
	mp_bar_->updateStatus_Text(status.getMaxMP(), status.getMP());

}
