#include "../../dxlib_ext/dxlib_ext.h"
#include "select_window.h"


SelectWindow::SelectWindow() {

	message_.resize(message_line_);

	message_[0] = "はい";
	message_[1] = "いいえ";

	selected_cmd_pos_.resize(message_line_);
	for (int i = 0; i < message_line_; ++i) {
		selected_cmd_pos_[i] = tnl::Vector2i(window_pos_.x + 10, mess_str_pos_.y + (message_space_ * i));
	}

}

SelectWindow::~SelectWindow() {

	tnl::DebugTrace("SelectWindowのデストラクタが実行されました\n");
}

void SelectWindow::update(float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W, eKeys::KB_UP)) {
		is_yes_ = !is_yes_;
		selected_cmd_index_--;
		if (selected_cmd_index_ < 0) selected_cmd_index_ = message_line_ - 1;
	}
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S, eKeys::KB_DOWN)) {
		is_yes_ = !is_yes_;
		selected_cmd_index_++;
		if (selected_cmd_index_ >= message_line_) selected_cmd_index_ = 0;
	}
	
}

void SelectWindow::draw(const std::shared_ptr<Camera> camera) {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, GetColor(0, 0, 128), true);
	DrawBox(selected_cmd_pos_[selected_cmd_index_].x, selected_cmd_pos_[selected_cmd_index_].y,
		selected_cmd_pos_[selected_cmd_index_].x + selected_cmd_size_.x, selected_cmd_pos_[selected_cmd_index_].y + selected_cmd_size_.y,
		GetColor(0, 128, 128), true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, -1, false);

	SetFontSize(message_font_size_);
	for (int line = 0; line < message_line_; line++) {
		DrawStringEx(mess_str_pos_.x, mess_str_pos_.y + (message_space_ * line), -1, message_[line].c_str());
	}

}
