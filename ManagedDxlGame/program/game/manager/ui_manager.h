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

	std::shared_ptr<MessageWindow> getMessageWindow() { return message_window_; }
	std::shared_ptr<HP_Bar> getHP_Bar() { return hp_bar_; }

private:
	// ���b�Z�[�W�E�B���h�E�̒ʏ�̈ʒu�A�T�C�Y
	const tnl::Vector2i	DEFAULT_MESS_WINDOW_POS = { 250, 450 };
	const tnl::Vector2i DEFAULT_MESS_WINDOW_SIZE = { 800, 200 };

	// �K�i�ł̑I�����̃��b�Z�[�W�E�B���h�E�̈ʒu�A�T�C�Y�A���b�Z�[�W
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_POS = { 150, 450 };
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_SIZE = { 750, 200 };
	std::string STAIR_SEL_MESSAGE = "��������B�����܂����H";

	std::shared_ptr<MessageWindow> message_window_ = nullptr;
	std::shared_ptr<SelectWindow> select_window_ = nullptr;
	std::shared_ptr<HP_Bar> hp_bar_ = nullptr;

public:
	// �K�i�ł̑I�����ʂ�Ԃ��B
	inline bool selectStairResult() { return select_window_->isYes(); }

};