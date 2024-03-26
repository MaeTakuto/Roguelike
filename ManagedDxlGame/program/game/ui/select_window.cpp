#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "select_window.h"


namespace {
	// 選択切り替え効果音のパス
	const std::string SELECT_SE_HDL_PATH = "sound/button_select.mp3";
}

SelectWindow::SelectWindow() : window_pos_(0, 0), window_size_(150, 70), selected_cmd_ui_size_(window_size_.x, DEFAULT_FONT_SIZE), seleted_cmd_ui_space_(10, DEFAULT_FONT_SIZE),
mess_str_pos_(window_pos_ + tnl::Vector2i(20, 20)), is_drawing_(false), is_operate_(false), message_font_size_(DEFAULT_FONT_SIZE), select_cmd_max_(0),
message_line_(0), message_space_(0), selected_cmd_index_(0)
{
	selected_cmd_ui_size_.x -= seleted_cmd_ui_space_.x * 2;
}

SelectWindow::~SelectWindow() {

	tnl::DebugTrace("SelectWindowのデストラクタが実行されました\n");
	ResourceManager::getInstance()->deleteSound(SELECT_SE_HDL_PATH);
}

// =====================================================================================
// アップデート
// =====================================================================================
void SelectWindow::update(float delta_time) {

	if (!is_operate_) {
		return;
	}

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W, eKeys::KB_UP)) {
		ResourceManager::getInstance()->playSound(SELECT_SE_HDL_PATH, DX_PLAYTYPE_BACK);
		selected_cmd_index_--;
		if (selected_cmd_index_ < 0) selected_cmd_index_ = message_line_ - 1;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S, eKeys::KB_DOWN)) {
		ResourceManager::getInstance()->playSound(SELECT_SE_HDL_PATH, DX_PLAYTYPE_BACK);
		selected_cmd_index_++;
		if (selected_cmd_index_ >= message_line_) selected_cmd_index_ = 0;
	}

}

// =====================================================================================
// 描画
// =====================================================================================
void SelectWindow::draw(const std::shared_ptr<Camera> camera) {

	if (!is_drawing_) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	// 選択ウィンドウの表示
	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, GetColor(0, 0, 128), true);

	// 選択中のコマンドUIを表示
	if (is_operate_) {
		DrawBox(selected_cmd_ui_pos_[selected_cmd_index_].x, selected_cmd_ui_pos_[selected_cmd_index_].y,
			selected_cmd_ui_pos_[selected_cmd_index_].x + selected_cmd_ui_size_.x, selected_cmd_ui_pos_[selected_cmd_index_].y + selected_cmd_ui_size_.y,
			GetColor(0, 128, 128), true);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, -1, false);

	SetFontSize(message_font_size_);
	for (int line = 0; line < message_line_; line++) {
		DrawStringEx(mess_str_pos_.x, mess_str_pos_.y + (message_space_ * line), -1, command_names_[line].c_str());
	}

}

void SelectWindow::setWindowPos(const tnl::Vector2i& pos) {
	window_pos_ = pos;
	mess_str_pos_ = pos + tnl::Vector2i(20, 20);

	for (int i = 0; i < command_names_.size(); ++i) {
		selected_cmd_ui_pos_[i] = tnl::Vector2i(window_pos_.x + seleted_cmd_ui_space_.x, mess_str_pos_.y + (message_space_ * i));
	}
}

void SelectWindow::setWindowSize(const tnl::Vector2i& size) {
	window_size_ = size;
	selected_cmd_ui_size_.x = size.x - seleted_cmd_ui_space_.x * 2;
}

// =====================================================================================
// コマンドの数を設定
// =====================================================================================
void SelectWindow::setSelectCmdMax(int select_cmd_max) {
	select_cmd_max_ = select_cmd_max;
	message_line_ = select_cmd_max;
	selected_cmd_index_ = 0;
}

// =====================================================================================
// 各コマンドの名前を設定
// =====================================================================================
void SelectWindow::setCommandNames(const std::vector<std::string>& command_names) {

	select_cmd_max_ = command_names.size();
	message_line_ = command_names.size();
	selected_cmd_index_ = 0;
	command_names_.resize(command_names.size());
	selected_cmd_ui_pos_.resize(command_names.size());

	// 各コマンドのコマンド名と選択中のコマンドの表示位置を各コマンドの位置に設定する
	for (int i = 0; i < command_names.size(); ++i) {
		command_names_[i] = command_names[i];
		selected_cmd_ui_pos_[i] = tnl::Vector2i(window_pos_.x + seleted_cmd_ui_space_.x, mess_str_pos_.y + (message_space_ * i));
	}
}
