#pragma once

class Camera;

// �_���W�����ŋN�����o���������b�Z�[�W�Ƃ��ĕ\������N���X
class MessageWindow {
public:
	MessageWindow();
	/*MessageWindow(int line);*/
	~MessageWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	const tnl::Vector2i MESS_STR_POS = { 30, 20 };
	// ���b�Z�[�W�E�B���h�E�̈ʒu
	tnl::Vector2i window_pos_ = { 250, 450 };
	tnl::Vector2i window_size_ = { 800, 200 };

	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_pos_ = window_pos_ + MESS_STR_POS;

	// �\�����Ă��邩����
	bool is_enable_ = false;

	// ���Ԑ����w�肵�Ă��邩����
	bool is_time_limit_ = false;

	// ��������
	float time_limit_ = 0.0f;

	// �\�����郁�b�Z�[�W
	std::vector<std::string> message_;

	// �\�����Ă��郁�b�Z�[�W��
	int display_message_count_ = 0;

	// �����̃T�C�Y
	int message_font_size_ = 30;

	// ���b�Z�[�W�E�B���h�E�ɏo���镶���̍ő�s��
	int message_line_ = 4;
	
	// �s�ƍs�̊Ԋu
	int message_space_ = 40;

public:
	// �Q�b�^�[
	inline bool isEnable() { return is_enable_; }

	// �E�B���h�E�̈ʒu��ݒ�
	inline void setWindowPos(const tnl::Vector2i& pos) { 
		window_pos_ = pos;
		mess_str_pos_ = window_pos_ + MESS_STR_POS;
	}

	// �E�B���h�E�̃T�C�Y��ݒ�
	inline void setWindowSize(const tnl::Vector2i& size) { window_size_ = size; }

	// ���b�Z�[�W�̃t�H���g�T�C�Y��ݒ�
	inline void setFontSize(int size) { message_font_size_ = size; }

	// ���b�Z�[�W�̍ő�s����ύX
	inline void setMessageLine(int line) {
		message_line_ = line;
		message_.resize(message_line_);
	}

	// �\�����Ԃ�ݒ肷��
	inline void setTimeLimit(float time_limit) { 
		is_enable_ = true;
		time_limit_ = time_limit;
		is_time_limit_ = true;
	}

	// �\�����Ԃ̐ݒ��������
	inline void cancelTimeLimit() {
		time_limit_ = 0.0f;
		is_time_limit_ = false;
	}

	// ���b�Z�[�W�E�B���h�E��\���A��\���̐ݒ������B
	inline void setEneble(bool enable) { is_enable_ = enable; }

	// ���b�Z�[�W��S�폜
	inline void clearMessage() {
		display_message_count_ = 0;
		for (int line = 0; line < message_line_; ++line) {
			message_[line] = "";
		}
	};

	// ���b�Z�[�W���Z�b�g����
	inline void setMessgae(const std::string& message) {
		if (display_message_count_ >= message_line_) {
			for (int line = 0; line < message_line_ - 1; ++line) {
				message_[line] = message_[line + 1];
			}
			message_[message_.size() - 1] = message;
			return;
		}
		message_[display_message_count_] = message;
		++display_message_count_;
	}

};