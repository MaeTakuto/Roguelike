#pragma once


class SceneBase {
public:
	SceneBase();
	virtual ~SceneBase() {};

	// �X�V
	virtual void update(float delta_time) = 0;

	// �`��
	virtual void draw() = 0;
//
//protected:
//	// �V�[���J�ڏ��������̊m�F�p
//	bool is_transition_process_;
//
//public:
//	// �V�[���J�ڂ̏������������Ă��邩��Ԃ��B
//	inline bool isTransitionProcess() { return is_transition_process_; }

};