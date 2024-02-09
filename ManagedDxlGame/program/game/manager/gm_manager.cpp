#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_main.h"
#include "../base/scene_base.h"
#include "gm_manager.h"
#include "resource_manager.h"

// �R���X�g���N�^
GameManager::GameManager(std::shared_ptr<SceneBase> start_scene) {
	now_scene_ = start_scene;
	transition_gpc_hdl_path_ = "graphics/black.bmp";

	transition_gpc_hdl_ = LoadGraph(transition_gpc_hdl_path_.c_str());
}

GameManager::~GameManager() {

}

// �Q�[���}�l�[�W���[�̃A�h���X��Ԃ�
GameManager* GameManager::GetInstance(std::shared_ptr<SceneBase> start_scene) {

	static GameManager* instance = nullptr;

	if (!instance) {
		instance = new GameManager(start_scene);
	}
	return instance;
}

// �A�b�v�f�[�g
void GameManager::update(float delta_time) {

	if (now_scene_) now_scene_->update(delta_time);
	if (now_scene_) now_scene_->draw();

	sequence_.update(delta_time);
}

// �V�[���̐؂�ւ����Z�b�g
void GameManager::changeScene(std::shared_ptr<SceneBase> next_scene, float fade_time) {

	if (is_transition_) return;

	next_scene_ = next_scene;
	fade_time_ = fade_time;
	sequence_.change(&GameManager::seqTransOut);
	is_transition_ = true;
}

// �������Ȃ�
bool GameManager::seqRun(const float delta_time) {

	return true;
}

// �t�F�[�h�A�E�g
bool GameManager::seqTransOut(const float delta_time) {
	int alpha = (sequence_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha >= 255) {
		sequence_.change(&GameManager::seqWait);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}

// ��ʂ��t�F�[�h�摜�œh��Ԃ��A�V�[����؂�ւ���
bool GameManager::seqWait(const float delta_time) {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (sequence_.getProgressTime() <= fade_time_wait_) return true;

	// ���݂̃V�[���N���X�̑J�ڏ������I����Ă��Ȃ����́A�����܂�
	if (now_scene_->isTransitionProcess() == false) return true;

	now_scene_ = nullptr;
	now_scene_ = next_scene_;

	sequence_.change(&GameManager::seqTransIn);

	return true;
}

// �t�F�[�h�C��
bool GameManager::seqTransIn(const float delta_time) {
	int alpha = 255 - (sequence_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha <= 0) {
		sequence_.change(&GameManager::seqRun);
		is_transition_ = false;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}