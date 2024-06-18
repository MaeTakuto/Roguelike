#pragma once

class Camera;
class MessageWindow;
class SelectWindow;
class StatusBar;
class Charater;
class MagicBase;

// =====================================================================================
// ゲーム内のUIを管理するクラス
// =====================================================================================
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
	// 操作説明ウィンドウ
	std::shared_ptr<MessageWindow> control_explanation_window_;
	// ステータスを確認するウィンドウ
	std::shared_ptr<MessageWindow> view_status_window_;
	// 魔法説明のウィンドウ
	std::shared_ptr<MessageWindow> magic_explation_window_;
	// メインメニューの選択ウィンドウ
	std::shared_ptr<SelectWindow> main_menu_select_window_;
	// 魔法一覧の選択ウィンドウ
	std::shared_ptr<SelectWindow> magic_select_window_;
	// 二択の選択ウィンドウ（ はい、いいえか ）
	std::shared_ptr<SelectWindow> two_select_window_;
	// 二択コマンドのコマンド名
	std::vector<std::vector<std::string>> two_select_cmd_names_;
	// プレイヤーのHPバー
	std::shared_ptr<StatusBar> hp_bar_;
	// プレイヤーのMPバー
	std::shared_ptr<StatusBar> mp_bar_;

	// 操作説明のメッセージ
	std::vector< std::vector< std::string > > control_explanation_message_;

	// UIのステータス参照用
	std::weak_ptr<Character> ui_target_;

	// 現在の階数
	int floor_;

public:
	inline void setUITargetCharacter(std::weak_ptr<Character> target) {
		ui_target_ = target;
	}

	// 階段選択の現在選択中のインデックスを返す
	int getSelectedIndexFromTwoSelectCmd();

	// ---------------- メインメニュー関連関数 -------------------------

	// メインメニューコマンドの選択しているインデックスを返す
	int getSelectedIndexFromMainMenuCmd();
	// メインメニューを開く
	void openMainMenu();
	// メインメニューを閉じる
	void closeMainMenu();
	// 魔法選択ウィンドウの一覧更新
	void updateMagicList();
	// 魔法リストの選択しているインデックスを返す
	int getSelectedIndexFromMagicListCmd();
	// 魔法選択ウィンドウを開く
	void openMagicListWindow();
	// 魔法選択ウィンドウを閉じる
	void closeMagicListWindow();
	// 魔法の説明を更新
	void updateMagicExplation();
	// 魔法を使うか選択するウィンドウを表示
	void executeSletctToUseMagic();
	// 魔法を使うか選択するウィンドウを閉じる
	void executeSletctToUseMagicEnd();
	// ステータスウィンドウを表示する
	void displayStatusWindow();
	// ステータスウィンドウを非表示にする
	void hideStatusWindow();

	// -------------------------------------------------------------------

	// 階段での選択の実行
	void executeStairSelect();
	// 階段での選択の実行
	void executeStairSelectEnd();

	// 操作画面ウィンドウの表示を切り替える
	void changeCtrlExplanationWindowType(int window_type);

	// メッセージウィンドウにメッセージをセット
	void setMessage(const std::string& message, float draw_time = -1.0f);
	// メッセージウィンドウのメッセージを全削除する
	void clearMessage();

	// ステータスバーの状態を更新
	void updateStatusBar();

	// 現在の階数をセット
	void setFloor(int floor) { floor_ = floor; }
};