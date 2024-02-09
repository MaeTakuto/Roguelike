#pragma once


class SceneBase {
public:
	virtual ~SceneBase() {};

	// �V�[���J�ڂ̏������������Ă��邩��Ԃ��B
	virtual bool isTransitionProcess() = 0;

	// �X�V
	virtual void update(float delta_time) = 0;

	// �`��
	virtual void draw() = 0;

protected:
	// �V�[���J�ڏ��������̊m�F�p
	bool is_transition_process_;

};