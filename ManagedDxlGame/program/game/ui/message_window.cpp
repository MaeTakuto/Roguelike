#include "../../dxlib_ext/dxlib_ext.h"
#include "../manager/resource_manager.h"
#include "../common/camera.h"
#include "message_window.h"


// =====================================================================================
// コンストラクタ
// =====================================================================================
MessageWindow::MessageWindow() : window_pos_(250, 450), window_size_(800, 200), mess_str_top_pos_(30, 20),
	mess_str_pos_(window_pos_ + mess_str_top_pos_), is_enable_(false), is_time_limit_(false), is_draw_enter_ui_(true),
	is_enable_enter_ui_(false), draw_time_limit_(0.0f), swicth_enter_ui_elapsed_(0.0f), display_message_count_(0), message_font_size_(30),
	message_line_(4), message_space_(10), gpc_hdl_(0), draw_gpc_hdl_pos_(0, 0), draw_gpc_hdl_size_(0, 0)
{

	message_.resize(message_line_);

	for (int line = 0; line < message_line_; line++) {
		message_[line] = "";
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
		draw_time_limit_ -= delta_time;

		if (draw_time_limit_ <= 0) {
			is_time_limit_ = false;
			is_enable_ = false;
			clearMessage();
		}
	}

	if (!is_enable_enter_ui_) {
		return;
	}

	swicth_enter_ui_elapsed_ += delta_time;

	if (swicth_enter_ui_elapsed_ < 0.5f) {
		return;
	}

	is_draw_enter_ui_ = !is_draw_enter_ui_;
	swicth_enter_ui_elapsed_ = 0.0f;

}

// =====================================================================================
// 描画
// =====================================================================================
void MessageWindow::draw() {

	if (!is_enable_) {
		return;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 160);
	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, GetColor(0, 0, 160), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawBox(window_pos_.x, window_pos_.y, window_pos_.x + window_size_.x, window_pos_.y + window_size_.y, -1, false);

	SetFontSize(message_font_size_);
	for (int line = 0; line < message_line_; line++) {
		DrawStringEx(mess_str_pos_.x, mess_str_pos_.y + ( ( message_font_size_ + message_space_ ) * line), -1, message_[line].c_str());
	}

	if (gpc_hdl_) {
		DrawExtendGraph(draw_gpc_hdl_pos_.x, draw_gpc_hdl_pos_.y,
			draw_gpc_hdl_pos_.x + draw_gpc_hdl_size_.x, draw_gpc_hdl_pos_.y + draw_gpc_hdl_size_.y,
			gpc_hdl_, true
		);
	}

	if (!is_enable_enter_ui_ || !is_draw_enter_ui_ ) {
		return;
	}

	SetFontSize(25);

	tnl::Vector2i draw_pos = window_pos_ + window_size_ - tnl::Vector2i( 150, 30 );

	DrawStringEx(draw_pos.x, draw_pos.y, -1, "Next Enter");
}

// =====================================================================================
// メッセージをセットする
// =====================================================================================
void MessageWindow::setMessgae(const std::string& message) {
	if (display_message_count_ >= message_line_) {
		for (int line = 0; line < message_line_ - 1; ++line) {
			message_[line] = message_[line + 1];
		}
		message_[message_.size() - 1] = message;
		return;
	}
	message_[display_message_count_] = message;
	++display_message_count_;
}

// =====================================================================================
// メッセージを全削除
// =====================================================================================
void MessageWindow::clearMessage() {
	display_message_count_ = 0;
	swicth_enter_ui_elapsed_ = 0.0f;
	is_enable_enter_ui_ = false;

	for (int line = 0; line < message_line_; ++line) {
		message_[line] = "";
	}
	cancelTimeLimit();
	setEnable(false);
}

// =====================================================================================
// メッセージを全削除
// =====================================================================================
void MessageWindow::calculateWindowSize() {
	window_size_.y = (message_font_size_) * message_line_ + message_space_ * ( message_line_ - 1 ) + (mess_str_top_pos_.y * 2);
};

// =====================================================================================
// 全ての行のメッセージをセットする。（ 最大行をはみ出した行はセットされない。 ）
// =====================================================================================
void MessageWindow::setAllLineMessage(const std::vector<std::string>& messages) {
	int size = messages.size() < message_.size() ? messages.size() : message_.size();
	display_message_count_ = size;

	for (int i = 0; i < size; ++i) {
		message_[i] = messages[i];
	}
}
