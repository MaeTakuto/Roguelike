#pragma once

class Camera;

// ダンジョンで起きた出来事をメッセージとして表示するクラス
class MessageWindow {
public:
	MessageWindow();
	~MessageWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int MESSAGE_WINDOW_WIDTH = 800;
	const int MESSAGE_WINDOW_HEIGHT = 200;
	const int MAX_MESSAGE_LINE = 4;
	const int MESSAGE_STR_SIZE = 30;
	const int MESSAGE_SPACE = 40;

	// 表示しているか判定
	bool is_display_ = true;

	// 時間制限指定しているか判定
	bool is_time_limit_ = true;
	
	// メッセージウィンドウの画像
	int mess_window_gpc_hdl_ = 0;

	// 制限時間
	float time_limit_ = 0.0f;

	// メッセージウィンドウの位置
	tnl::Vector2i window_pos_ = { 250, 450 };

	// メッセージの位置
	tnl::Vector2i mess_str_pos_ = window_pos_ + tnl::Vector2i(30, 20);

	// 表示するメッセージ
	std::string* mess_str_ = nullptr;

	// 表示しているメッセージ数
	int display_message_count_ = 0;

public:
	// ゲッター
	inline bool isDisplay() { return is_display_; }

	// 表示時間を設定する
	inline void setTimeLimit(float time_limit) { 
		is_display_ = true;
		time_limit_ = time_limit;
		is_time_limit_ = true;
	}

	// メッセージウィンドウを表示する。
	inline void displayWindow() { is_display_ = true; }

	// メッセージウィンドウを非表示にする。
	inline void hideWindow() { is_display_ = false; }

	// メッセージを全削除
	inline void clearMessage() {
		display_message_count_ = 0;
		for (int line = 0; line < MAX_MESSAGE_LINE; ++line) {
			mess_str_[line] = "";
		}
	};

	// メッセージをセットする
	inline void setMessgae(std::string& message) {
		if (display_message_count_ >= MAX_MESSAGE_LINE) {
			for (int line = 0; line < MAX_MESSAGE_LINE - 1; ++line) {
				mess_str_[line] = mess_str_[line + 1];
			}
			mess_str_[3] = message;
			return;
		}
		mess_str_[display_message_count_] = message;
		++display_message_count_;
	}

};