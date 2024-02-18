#pragma once

class Camera;

// ダンジョンで起きた出来事をメッセージとして表示するクラス
class MessageWindow {
public:
	MessageWindow();
	/*MessageWindow(int line);*/
	~MessageWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const tnl::Vector2i MESS_STR_POS = { 30, 20 };
	// メッセージウィンドウの位置
	tnl::Vector2i window_pos_ = { 250, 450 };
	tnl::Vector2i window_size_ = { 800, 200 };

	// メッセージの位置
	tnl::Vector2i mess_str_pos_ = window_pos_ + MESS_STR_POS;

	// 表示しているか判定
	bool is_enable_ = false;

	// 時間制限指定しているか判定
	bool is_time_limit_ = false;

	// 制限時間
	float time_limit_ = 0.0f;

	// 表示するメッセージ
	std::vector<std::string> message_;

	// 表示しているメッセージ数
	int display_message_count_ = 0;

	// 文字のサイズ
	int message_font_size_ = 30;

	// メッセージウィンドウに出せる文字の最大行数
	int message_line_ = 4;
	
	// 行と行の間隔
	int message_space_ = 40;

public:
	// ゲッター
	inline bool isEnable() { return is_enable_; }

	// ウィンドウの位置を設定
	inline void setWindowPos(const tnl::Vector2i& pos) { 
		window_pos_ = pos;
		mess_str_pos_ = window_pos_ + MESS_STR_POS;
	}

	// ウィンドウのサイズを設定
	inline void setWindowSize(const tnl::Vector2i& size) { window_size_ = size; }

	// メッセージのフォントサイズを設定
	inline void setFontSize(int size) { message_font_size_ = size; }

	// メッセージの最大行数を変更
	inline void setMessageLine(int line) {
		message_line_ = line;
		message_.resize(message_line_);
	}

	// 表示時間を設定する
	inline void setTimeLimit(float time_limit) { 
		is_enable_ = true;
		time_limit_ = time_limit;
		is_time_limit_ = true;
	}

	// 表示時間の設定を取り消し
	inline void cancelTimeLimit() {
		time_limit_ = 0.0f;
		is_time_limit_ = false;
	}

	// メッセージウィンドウを表示、非表示の設定をする。
	inline void setEneble(bool enable) { is_enable_ = enable; }

	// メッセージを全削除
	inline void clearMessage() {
		display_message_count_ = 0;
		for (int line = 0; line < message_line_; ++line) {
			message_[line] = "";
		}
	};

	// メッセージをセットする
	inline void setMessgae(const std::string& message) {
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

};