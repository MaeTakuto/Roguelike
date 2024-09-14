#pragma once

// =====================================================================================
// 対象のステータスをゲージで表示するUIクラス
// =====================================================================================
class StatusBar {
public:
	StatusBar();
	~StatusBar();

	void update(float delta_time);
	void draw();

	enum class eStatusBarColor {
		GREEN = 0,
		LIGHT_BLUE,
		MAX
	};

private:
	// ステータスバーのフロント画像
	int status_bar_front_gpc_hdl_[std::underlying_type<eStatusBarColor>::type(eStatusBarColor::MAX)];
	// ステータスバーのバック画像
	int status_bar_back_gpc_hdl_;

	// ステータスバーの位置
	tnl::Vector2i status_bar_back_pos_;
	// ステータスバーのサイズ
	tnl::Vector2i status_bar_back_size_;

	// ステータスバーの位置
	tnl::Vector2i status_bar_front_pos_;
	// ステータスバーの位置
	tnl::Vector2i status_bar_front_size_;
	// 
	tnl::Vector2i status_text_pos_;

	// 選択したステータスバーの色
	int selected_color_;

	// ステータスの最大値、現在値
	int max_status_;
	int now_status_;

	std::string status_type_text_;
	std::string status_text_;

	// 文字サイズ
	int message_font_size_;

	// ステータスバーの輪郭サイズ
	int status_bar_outline_size_;

public:
	// ステータスバーの位置を設定
	inline void setStatusBarPos(const tnl::Vector2i& pos) { 
		status_bar_back_pos_ = pos;
		status_bar_front_pos_ = status_bar_back_pos_ + (status_bar_back_size_ - status_bar_front_size_) / 2;
		status_text_pos_ = status_bar_back_pos_ - tnl::Vector2i(180, -5);
	}
	// ステータスバーのサイズを設定
	inline void setStatusBarSize(const tnl::Vector2i& size) { status_bar_back_size_ = size; }
	// 表示するステータスのタイプを設定（ HP, MPなど ）
	inline void setStatusTypeText(const std::string& status_type_text) { status_type_text_ = status_type_text; }
	// ステータスの最大値を設定
	inline void setMaxStatus(int max_status) { max_status_ = max_status; }
	// 現在のステータスの値を設定
	inline void setStatus(int now_status) { now_status_ = now_status; }
	//
	void setStatusBarColor(eStatusBarColor selected_color) {
		selected_color_ = std::underlying_type<eStatusBarColor>::type(selected_color);
	}

	// ステータスバーを現在の値に更新
	void updateStatus_Text(int max_status, int now_status);

};