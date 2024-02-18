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

	std::shared_ptr<MessageWindow> getMessageWindow() { return message_window_; }
	std::shared_ptr<HP_Bar> getHP_Bar() { return hp_bar_; }

private:
	// メッセージウィンドウの通常の位置、サイズ
	const tnl::Vector2i	DEFAULT_MESS_WINDOW_POS = { 250, 450 };
	const tnl::Vector2i DEFAULT_MESS_WINDOW_SIZE = { 800, 200 };

	// 階段での選択時のメッセージウィンドウの位置、サイズ、メッセージ
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_POS = { 150, 450 };
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_SIZE = { 750, 200 };
	std::string STAIR_SEL_MESSAGE = "穴がある。落ちますか？";

	std::shared_ptr<MessageWindow> message_window_ = nullptr;
	std::shared_ptr<SelectWindow> select_window_ = nullptr;
	std::shared_ptr<HP_Bar> hp_bar_ = nullptr;

public:
	// 階段での選択結果を返す。
	inline bool selectStairResult() { return select_window_->isYes(); }

};