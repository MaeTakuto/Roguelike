#pragma once

class Camera;
class MessageWindow;
class SelectWindow;
class HP_Bar;
class Charater;

class UI_Manager {
public:
	UI_Manager();
	~UI_Manager();

	// 更新と描画
	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// ------------------- 各 UIクラス -------------------
	// メッセージウィンドウ
	std::shared_ptr<MessageWindow> message_window_;
	// ステータスを確認するウィンドウ
	std::shared_ptr<MessageWindow> view_status_window_;
	// メインメニューの選択ウィンドウ
	std::shared_ptr<SelectWindow> main_menu_select_window_;
	// 魔法一覧の選択ウィンドウ
	std::shared_ptr<SelectWindow> magic_select_window_;
	// 二択の選択ウィンドウ（ はい、いいえか ）
	std::shared_ptr<SelectWindow> two_select_window_;
	// プレイヤーのHPバー
	std::shared_ptr<HP_Bar> hp_bar_;

	// UIのステータス参照用
	std::weak_ptr<Character> ui_target_;

	// 現在の階数
	int floor_;

public:
	inline void setUITargetCharacter(std::weak_ptr<Character> target) {
		ui_target_ = target;
	}

	// 階段選択の現在選択中のインデックスを返す
	int getSelectedStairSelectCmdIndex();

	// ---------------- メインメニュー関連関数 -------------------------

	// メインメニューの現在選択中のインデックスを返す
	int getSelectedMainMenuCmdIndex();
	// メインメニューを開く
	void openMainMenu();
	// メインメニューを閉じる
	void closeMainMenu();
	// 魔法選択ウィンドウの一覧更新
	void updateMagicList();
	// メインメニューの現在選択中のインデックスを返す
	int getSelectedMagicListCmdIndex();
	// 魔法選択ウィンドウを開く
	void openMagicListWindow();
	// 魔法選択ウィンドウを閉じる
	void closeMagicListWindow();
	// ステータスウィンドウを表示する
	void displayStatusWindow();
	// ステータスウィンドウを非表示にする
	void hideStatusWindow();

	// -------------------------------------------------------------------

	// 階段での選択の実行
	void executeStairSelect();
	// 階段での選択の実行
	void executeStairSelectEnd();

	// メッセージウィンドウにメッセージをセット
	void setMessage(const std::string& message, float draw_time = -1.0f);
	// メッセージウィンドウのメッセージを全削除する
	void clearMessage();

	// HPバーにステータスをセット
	void setHP_BarStatus();

	// 現在の階数をセット
	void setFloor(int floor) { floor_ = floor; }
};