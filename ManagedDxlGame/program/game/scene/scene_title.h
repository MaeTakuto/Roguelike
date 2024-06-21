#pragma once
#include "../base/scene_base.h"

class DungeonLog;
class SelectWindow;

class SceneTitle final : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	void update(float delta_time) override;
	void draw() override;

private:
	// �^�C�g����ʃV�[�P���X
	tnl::Sequence<SceneTitle> sequence_;

	// �^�C�g�����j���[
	std::shared_ptr<SelectWindow> title_menu_;
	// �_���W�������O���j���[
	std::shared_ptr<SelectWindow> dungeon_log_menu_;
	// �_���W�����L�^���X�g
	std::vector<std::shared_ptr<DungeonLog>> dungeon_log_list_;

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
	// �^�C�g�����j���[�̓����x
	int title_menu_alpha_;
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
	// ���j���[�I����ʃV�[�P���X
	bool seqSelectMenu(const float delta_time);
	// �_���W�����L�^��I������V�[�P���X
	bool seqSelectDungeonLogList(const float delta_time);

};