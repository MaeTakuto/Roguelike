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

	// �X�V�ƕ`��
	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

	// �K�i�ł̑I���̎��s
	void executeStairSelect();

	// �K�i�ł̑I���̎��s
	void executeStairSelectEnd();

	// ���b�Z�[�W���Z�b�g
	void setMessage(const std::string& message, float draw_time = -1.0f);

	void clearMessage();

	// HP�o�[�ɃX�e�[�^�X���Z�b�g
	void setHP_BarStatus(int max_hp, int hp);

	// ���݂̊K�����Z�b�g
	void setFloor(int floor) { floor_ = floor; }

	// std::shared_ptr<MessageWindow> getMessageWindow() { return message_window_; }
	//std::shared_ptr<HP_Bar> getHP_Bar() { return hp_bar_; }

private:
	// �e UI�N���X
	std::shared_ptr<MessageWindow> message_window_;
	std::shared_ptr<SelectWindow> select_window_;
	std::shared_ptr<HP_Bar> hp_bar_;

	int floor_;

public:
	// �K�i�ł̑I�����ʂ�Ԃ��B
	inline bool selectStairResult() { return select_window_->isYes(); }

};