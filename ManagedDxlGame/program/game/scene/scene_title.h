#pragma once
#include "../base/scene_base.h"


class SceneTitle final : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	inline bool isTransitionProcess() override { return is_transition_process_; };

	void update(float delta_time) override;
	void draw() override;

private:
	int back_ground_gpc_hdl_ = 0;

};