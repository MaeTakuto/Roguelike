#pragma once

class Camera;

// =====================================================================================
// 選択ウィンドウの表示、選択をクラス
// =====================================================================================
class SelectWindow {
public:
	SelectWindow();
	~SelectWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// 選択ウィンドウの位置
	tnl::Vector2i window_pos_;

	// 選択ウィンドウのサイズ
	tnl::Vector2i window_size_;

	// 選択中コマンドの位置
	std::vector<tnl::Vector2i> selected_cmd_ui_pos_;

	// 選択中コマンドのサイズ
	tnl::Vector2i selected_cmd_ui_size_;

	// 選択中コマンドUIの表示位置の間隔調整
	tnl::Vector2i seleted_cmd_ui_space_;

	// メッセージの位置
	tnl::Vector2i mess_str_pos_;

	// 選択ウィンドウが描画するか判定
	bool is_drawing_;

	// 選択ウィンドウを操作するか判定
	bool is_operate_;

	// 表示するメッセージ
	std::vector<std::string> command_names_;

	// 文字のサイズ
	int message_font_size_;

	// 選択コマンドの数
	int select_cmd_max_;

	// メッセージウィンドウに出せる文字の最大行数
	int message_line_;

	// 行と行の間隔
	int message_space_;

	// 現在の選択カーソルのインデックス。
	int selected_cmd_index_;

public:
	// 選択ウィンドウの表示位置を設定
	void setWindowPos(const tnl::Vector2i& pos);

	// 選択ウィンドウのサイズを設定
	void setWindowSize(const tnl::Vector2i& size);

	// コマンドメッセージの表示間隔を設定
	inline void setMessageSpace(int space) { message_space_ = space; }

	// コマンド名のフォントサイズを設定
	inline void setFontSize(int font_size) {
		message_font_size_ = font_size;
		selected_cmd_ui_size_.y = font_size;
	}

	// 選択コマンドの数を設定
	void setSelectCmdMax(int select_cmd_max);

	// 各選択コマンドの名前を設定
	void setCommandNames(const std::vector<std::string>& command_names);

	// =============== インライン関数 =================

	// 選択ウィンドウが有効かを返す
	inline bool isDrawing() { return is_drawing_; }

	// 選択ウィンドウを操作できるか判定を返す
	inline bool isOperate() { return is_operate_; }

	// 選択ウィンドウを描画するか設定する
	inline void setDrawing(bool is_drawing) {
		is_drawing_ = is_drawing;
		selected_cmd_index_ = 0;
	}

	// 選択ウィンドウを操作するか設定する
	inline void setOperate(bool is_operate) {
		is_operate_ = is_operate;
	}

	// 選択中のコマンド番号を返す。
	inline int getSelectedCmdIndex() { return selected_cmd_index_; }

};