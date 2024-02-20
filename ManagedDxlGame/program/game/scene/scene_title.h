#pragma once
#include "../base/scene_base.h"


class SceneTitle : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	inline bool isTransitionProcess() override { return is_transition_process_; };

	void update(float delta_time) override;
	void draw() override;

private:
	const tnl::Vector2i BACKGROUND_POS = { 0, 0 };
	const tnl::Vector2i TITLE_POS = { 400, 200 };
	const std::string TITLE = "ïsévãcÇ»êX";

	int back_ground_gpc_hdl_ = 0;

};