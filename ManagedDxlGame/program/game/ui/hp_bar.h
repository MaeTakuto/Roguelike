#pragma once


class StatusBar {
public:
	StatusBar();
	~StatusBar();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	tnl::Vector2i status_bar_pos_;
	tnl::Vector2i status_bar_size_;
	tnl::Vector2i status_text_pos_;

	int max_status_;
	int now_status_;

	std::string status_type_text_;
	std::string status_text_;

	int message_font_size_;

public:
	// ステータスバーの位置を設定
	inline void setStatusBarPos(const tnl::Vector2i& pos) { 
		status_bar_pos_ = pos;
		status_text_pos_ = status_bar_pos_ - tnl::Vector2i(150, -5);
	}
	// ステータスバーのサイズを設定
	inline void setStatusBarSize(const tnl::Vector2i& size) { status_bar_size_ = size; }
	// 表示するステータスのタイプを設定（ HP, MPなど ）
	inline void setStatusTypeText(const std::string& status_type_text) { status_type_text_ = status_type_text; }
	// ステータスの最大値を設定
	inline void setMaxStatus(int max_status) { max_status_ = max_status; }
	// 現在のステータスの値を設定
	inline void setStatus(int now_status) { now_status_ = now_status; }

	// ステータスバーを現在の値に更新
	void updateStatus_Text(int max_status, int now_status);

};