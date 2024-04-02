#pragma once
#include "../base/scene_base.h"

class MessageWindow;

class SceneGameClear final : public SceneBase {
public:
	SceneGameClear();
	~SceneGameClear();

	void update(float delta_time) override;
	void draw() override;

private:
	tnl::Sequence<SceneGameClear> sequence_;

	std::shared_ptr<MessageWindow> message_window_;

	std::vector<std::vector<std::string>> event_messages_;

	int now_page_;

	// ƒ^ƒCƒgƒ‹”wŒi‰æ‘œ
	int back_ground_gpc_hdl_;

private:
	bool seqSceneTransition(const float delta_time);
	bool seqMessageEvent(const float delta_time);
	bool seqEnd(const float delta_time);

};

