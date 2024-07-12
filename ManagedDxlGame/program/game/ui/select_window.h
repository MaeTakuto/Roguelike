#pragma once

class Camera;

// =====================================================================================
// 選択ウィンドウの表示し、選択をするクラス
// =====================================================================================
class SelectWindow {
public:
	SelectWindow();
	~SelectWindow();

	void update(float delta_time);
	void draw();

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
	tnl::Vector2i mess_str_top_pos_;
	// メッセージの位置
	tnl::Vector2i mess_str_pos_;

	// 選択ウィンドウが描画するか判定
	bool is_drawing_;
	// 選択ウィンドウを操作するか判定
	bool is_operate_;

	// 選択中のコマンドUIの描画するか判定
	bool is_drawing_selected_cmd_ui_;
	// 選択中のUIの表示切り替え間隔
	float swicth_draw_selected_ui_interval_;
	// 切り替えからの経過時間
	float swicth_draw_selected_ui_elapsed_;

	// 表示するメッセージ
	std::vector<std::string> command_names_;

	// 文字のサイズ
	int message_font_size_;

	// 選択コマンドの最大数
	int select_cmd_max_;

	// メッセージウィンドウに出せる文字の最大行数
	int message_line_;

	// 行と行の間隔
	int message_space_;

	// 現在の選択カーソルのインデックス。
	int selected_cmd_index_;

public:

	// ==============================================
	// ゲッター
	// ==============================================

	// 選択ウィンドウが有効かを返す
	inline bool isDrawing() { return is_drawing_; }

	// 選択ウィンドウを操作できるか判定を返す
	inline bool isOperate() { return is_operate_; }

	// 選択ウィンドウの位置を取得
	const tnl::Vector2i& getWindowPos() const { return window_pos_; }

	// 選択ウィンドウのサイズを取得
	const tnl::Vector2i& getWindowSize() const { return window_size_; }

	// 選択中のコマンド番号を返す。
	inline int getSelectedCmdIndex() { return selected_cmd_index_; }

	// 選択できるコマンドの最大数を取得する
	inline int getSelectCmdMax() { return select_cmd_max_; }

	// ==============================================
	// セッター
	// ==============================================

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

	// 選択ウィンドウを描画するか設定する
	inline void setDrawing(bool is_drawing) {
		is_drawing_ = is_drawing;
		selected_cmd_index_ = 0;
	}

	// 選択ウィンドウを操作するか設定する
	inline void setOperate(bool is_operate) {
		is_operate_ = is_operate;
		is_drawing_selected_cmd_ui_ = true;
		swicth_draw_selected_ui_elapsed_ = 0.0f;
	}

	// ==============================================
	// その他
	// ==============================================

	// ウィンづサイズ（ 高さ ）を計算する。
	void calculateWindowSize();

};