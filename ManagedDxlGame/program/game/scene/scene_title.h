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
	// �^�C�g����ʃV�[�P���X
	tnl::Sequence<SceneTitle> sequence_;

	// �^�C�g���w�i�摜
	int back_ground_gpc_hdl_;

	// �������̉摜
	int sunlight_gpc_hdl_;

	// �^�C�g��BGM�p�X
	std::string title_bgm_hdl_path_;

	// �^�C�g��BGM
	int title_bgm_hdl_;

	// BGM�̏I���n�_�̃T���v�����O���g��
	int bgm_end_freqency_;

	// �������摜�̓����x
	int sunlight_alpha_;

	// �����x�̒���
	int alpha_center_;

	// sin �͈̔�
	int sin_range_;

	// �V�[���o�ߎ���
	float scene_elapsed_time_;

	// �V�[���J�ڒ��V�[�P���X
	bool seqSceneTransition(const float);
	// �^�C�g����ʂł̏����V�[�P���X
	bool seqRun(const float delta_time);

};