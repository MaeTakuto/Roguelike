#pragma once

class Camera;

// �_���W�����ŋN�����o���������b�Z�[�W�Ƃ��ĕ\������N���X
class MessageWindow {
public:
	MessageWindow();
	~MessageWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const int MESSAGE_WINDOW_WIDTH = 800;
	const int MESSAGE_WINDOW_HEIGHT = 200;
	const int MAX_MESSAGE_LINE = 4;
	const int MESSAGE_STR_SIZE = 30;
	const int MESSAGE_SPACE = 40;

	// �\�����Ă��邩����
	bool is_display_ = true;

	// ���Ԑ����w�肵�Ă��邩����
	bool is_time_limit_ = true;
	
	// ���b�Z�[�W�E�B���h�E�̉摜
	int mess_window_gpc_hdl_ = 0;

	// ��������
	float time_limit_ = 0.0f;

	// ���b�Z�[�W�E�B���h�E�̈ʒu
	tnl::Vector2i window_pos_ = { 250, 450 };

	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_pos_ = window_pos_ + tnl::Vector2i(30, 20);

	// �\�����郁�b�Z�[�W
	std::string* mess_str_ = nullptr;

	// �\�����Ă��郁�b�Z�[�W��
	int display_message_count_ = 0;

public:
	// �Q�b�^�[
	inline bool isDisplay() { return is_display_; }

	// �\�����Ԃ�ݒ肷��
	inline void setTimeLimit(float time_limit) { 
		is_display_ = true;
		time_limit_ = time_limit;
		is_time_limit_ = true;
	}

	// ���b�Z�[�W�E�B���h�E��\������B
	inline void displayWindow() { is_display_ = true; }

	// ���b�Z�[�W�E�B���h�E���\���ɂ���B
	inline void hideWindow() { is_display_ = false; }

	// ���b�Z�[�W��S�폜
	inline void clearMessage() {
		display_message_count_ = 0;
		for (int line = 0; line < MAX_MESSAGE_LINE; ++line) {
			mess_str_[line] = "";
		}
	};

	// ���b�Z�[�W���Z�b�g����
	inline void setMessgae(std::string& message) {
		if (display_message_count_ >= MAX_MESSAGE_LINE) {
			for (int line = 0; line < MAX_MESSAGE_LINE - 1; ++line) {
				mess_str_[line] = mess_str_[line + 1];
			}
			mess_str_[3] = message;
			return;
		}
		mess_str_[display_message_count_] = message;
		++display_message_count_;
	}

};