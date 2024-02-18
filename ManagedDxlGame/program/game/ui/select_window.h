#pragma once

class Camera;

class SelectWindow {
public:
	SelectWindow();
	~SelectWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// メッセージウィンドウの画像用
	int mess_window_gpc_hdl_ = 0;

	// 選択カーソルの画像用
	int select_cursor_gpc_hdl_ = 0;

	// 選択ウィンドウの位置、サイズ
	tnl::Vector2i window_pos_ = { 950, 450 };
	tnl::Vector2i window_size_ = { 150, 120 };

	// 選択中コマンドの位置、サイズ
	std::vector<tnl::Vector2i> selected_cmd_pos_;
	tnl::Vector2i selected_cmd_size_ = { window_size_.x - 20, 30 };


	// メッセージの位置
	tnl::Vector2i mess_str_pos_ = window_pos_ + tnl::Vector2i(30, 20);

	// 選択ウィンドウが有効か判定
	bool is_enable_ = false;

	// 選択中のコマンドが「はい」か判定。
	bool is_yes_ = true;

	// 表示するメッセージ
	std::vector<std::string> message_;

	// 表示しているメッセージ数
	int display_message_count_ = 0;

	// 文字のサイズ
	int message_font_size_ = 30;

	// メッセージウィンドウに出せる文字の最大行数
	int message_line_ = 2;

	// 行と行の間隔
	int message_space_ = 50;

	// 現在の選択カーソルのインデックス。
	int selected_cmd_index_ = 0;

public:
	// 選択ウィンドウが有効かを返す
	inline bool isEnable() { return is_enable_; }

	// 選択ウィンドウを有効、無効にする
	inline void setEnable(bool is_enable) { is_enable_ = is_enable; }

	// 選択中のコマンドが「はい」かを返す。
	inline bool isYes() { return is_yes_; }

	// 選択ウィンドウの初期化
	inline void init() {
		is_yes_ = true;
		selected_cmd_index_ = 0;
	}

};