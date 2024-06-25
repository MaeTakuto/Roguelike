#pragma once

class Camera;

// メッセージを表示するウィンドウクラス
class MessageWindow {
public:
	MessageWindow();
	~MessageWindow();

	void update(float delta_time);
	void draw();

private:
	// メッセージウィンドウの位置
	tnl::Vector2i window_pos_ = { 250, 450 };
	tnl::Vector2i window_size_ = { 800, 200 };

	// メッセージの位置
	tnl::Vector2i mess_str_top_pos_ = { 30, 20 };
	tnl::Vector2i mess_str_pos_ = window_pos_ + mess_str_top_pos_;

	//
	std::string ui_message_;
	// 表示するメッセージ
	std::vector<std::string> message_;

	// 表示しているか判定
	bool is_enable_ = false;
	// 時間制限指定しているか判定
	bool is_time_limit_ = false;
	// 
	bool is_draw_enter_ui_;

	// 制限時間
	float time_limit_ = 0.0f;

	// 表示しているメッセージ数
	int display_message_count_ = 0;
	// 文字のサイズ
	int message_font_size_ = 30;
	// メッセージウィンドウに出せる文字の最大行数
	int message_line_ = 4;
	// 行と行の間隔
	int message_space_ = 10;

public:

	// =====================================================================================
	// 各ゲッター関数
	// =====================================================================================

	inline bool isEnable() { return is_enable_; }

	// ウィンドウ位置を取得
	inline tnl::Vector2i& getWindowPos() { return window_pos_; }

	// ウィンドウサイズを取得
	inline tnl::Vector2i& getWindowSize() { return window_size_; }

	// =====================================================================================
	// 各セッター関数
	// =====================================================================================

	// ウィンドウの位置を設定
	inline void setWindowPos(const tnl::Vector2i& pos) { 
		window_pos_ = pos;
		mess_str_pos_ = window_pos_ + mess_str_top_pos_;
	}

	// ウィンドウのサイズを設定
	inline void setWindowSize(const tnl::Vector2i& size) { window_size_ = size + tnl::Vector2i( mess_str_top_pos_.x, mess_str_top_pos_.y * 2 - message_space_); }

	// メッセージの表示位置の始点を決める
	inline void setMessageTopPos(const tnl::Vector2i pos) { 
		mess_str_top_pos_ = pos;
		mess_str_pos_ = window_pos_ + mess_str_top_pos_;
	}

	// メッセージのフォントサイズを設定
	inline void setFontSize(int size) { message_font_size_ = size; }

	// メッセージの表示間隔を設定する
	inline void setMessageSpace(int message_space) { 
		window_size_.y += message_space_ - message_space;
		message_space_ = message_space;
	}

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

	// メッセージをセットする
	void setMessgae(const std::string& message);

	// 全ての行のメッセージをセットする。（ 最大行をはみ出した行はセットされない。 ）
	void setAllLineMessage(const std::vector<std::string>& messages);

	// メッセージウィンドウを表示、非表示の設定をする。
	inline void setEnable(bool enable) { is_enable_ = enable; }

	// =====================================================================================
	// その他関数
	// =====================================================================================

	// メッセージを全削除
	void clearMessage();

	// 表示時間の設定を取り消し
	inline void cancelTimeLimit() {
		time_limit_ = 0.0f;
		is_time_limit_ = false;
	}

	// 行数、文字間隔、文字サイズからウィンドウサイズを計算する
	void calculateWindowSize();

};