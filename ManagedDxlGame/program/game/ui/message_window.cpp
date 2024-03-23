#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "message_window.h"


// =====================================================================================
// コンストラクタ
// =====================================================================================
MessageWindow::MessageWindow() {

	command_names_.resize(message_line_);

	for (int line = 0; line < message_line_; line++) {
		command_names_[line] = "";
	}
}

// =====================================================================================
// デストラクタ
// =====================================================================================
MessageWindow::~MessageWindow() {
	
	tnl::DebugTrace("MessageWindowのデストラクタが実行されました\n");

}

// =====================================================================================
// アップデート
// =====================================================================================
void MessageWindow::update(float delta_time) {

	if (is_time_limit_) {
		time_limit_ -= delta_time;

		if (time_limit_ <= 0) {
			is_time_limit_ = false;
			is_enable_ = false;
			clearMessage();
		}
	}
}

// =====================================================================================
// 描画
// =====================================================================================
void MessageWindow::draw(std::shared_ptr<Camera> canmera) {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, GetColor(0, 0, 128), true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, -1, false);

	SetFontSize(message_font_size_);
	for (int line = 0; line < message_line_; line++) {
		DrawStringEx(mess_str_pos_.x, mess_str_pos_.y + (message_space_ * line), -1, command_names_[line].c_str());
	}
}

// =====================================================================================
// メッセージをセットする
// =====================================================================================
void MessageWindow::setMessgae(const std::string& message) {
	if (display_message_count_ >= message_line_) {
		for (int line = 0; line < message_line_ - 1; ++line) {
			command_names_[line] = command_names_[line + 1];
		}
		command_names_[command_names_.size() - 1] = message;
		return;
	}
	command_names_[display_message_count_] = message;
	++display_message_count_;
}

// =====================================================================================
// メッセージを全削除
// =====================================================================================
void MessageWindow::clearMessage() {
	display_message_count_ = 0;
	for (int line = 0; line < message_line_; ++line) {
		command_names_[line] = "";
	}
	cancelTimeLimit();
	setEnable(false);
};
