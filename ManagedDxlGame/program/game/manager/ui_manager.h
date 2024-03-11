#pragma once
#include "../ui/select_window.h"

class Camera;
class MessageWindow;
class SelectWindow;
class HP_Bar;

class UI_Manager {
public:
	UI_Manager();
	~UI_Manager();

	// 更新と描画
	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

	// 階段での選択の実行
	void executeStairSelect();

	// 階段での選択の実行
	void executeStairSelectEnd();

	// メッセージをセット
	void setMessage(const std::string& message, float draw_time = -1.0f);

	void clearMessage();

	// HPバーにステータスをセット
	void setHP_BarStatus(int max_hp, int hp);

	// 現在の階数をセット
	void setFloor(int floor) { floor_ = floor; }

	// std::shared_ptr<MessageWindow> getMessageWindow() { return message_window_; }
	//std::shared_ptr<HP_Bar> getHP_Bar() { return hp_bar_; }

private:
	// 各 UIクラス
	std::shared_ptr<MessageWindow> message_window_;
	std::shared_ptr<SelectWindow> select_window_;
	std::shared_ptr<HP_Bar> hp_bar_;

	int floor_;

public:
	// 階段での選択結果を返す。
	inline bool selectStairResult() { return select_window_->isYes(); }

};