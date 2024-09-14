#pragma once

class Camera;

// ���b�Z�[�W��\������E�B���h�E�N���X
class MessageWindow {
public:
	MessageWindow();
	~MessageWindow();

	void update(float delta_time);
	void draw();

private:
	// ���b�Z�[�W�E�B���h�E�̈ʒu
	tnl::Vector2i window_pos_;
	tnl::Vector2i window_size_;

	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_top_pos_;
	tnl::Vector2i mess_str_pos_;

	// �\�����郁�b�Z�[�W
	std::vector<std::string> message_;

	// �\�����Ă��邩����
	bool is_enable_;
	// ���Ԑ����w�肵�Ă��邩����
	bool is_time_limit_;
	// Enter��UI��`�悷�邩����
	bool is_draw_enter_ui_;
	// Enter��UI���L��������
	bool is_enable_enter_ui_;

	// ��������
	float draw_time_limit_;
	// Enter��UI�؂�ւ�����̌o�ߎ���
	float swicth_enter_ui_elapsed_;

	// �\�����Ă��郁�b�Z�[�W��
	int display_message_count_;
	// �����̃T�C�Y
	int message_font_size_;
	// ���b�Z�[�W�E�B���h�E�ɏo���镶���̍ő�s��
	int message_line_;
	// �s�ƍs�̊Ԋu
	int message_space_;

	// ----------- �摜�֘A -----------------
	// �E�B���h�E�ɕ\������摜
	int gpc_hdl_;
	// �摜�̕\���ʒu
	tnl::Vector2i draw_gpc_hdl_pos_;
	// �摜�̕\���T�C�Y
	tnl::Vector2i draw_gpc_hdl_size_;

public:

	// =====================================================================================
	// �e�Q�b�^�[�֐�
	// =====================================================================================

	inline bool isEnable() { return is_enable_; }

	// �E�B���h�E�ʒu���擾
	inline tnl::Vector2i& getWindowPos() { return window_pos_; }

	// �E�B���h�E�T�C�Y���擾
	inline tnl::Vector2i& getWindowSize() { return window_size_; }

	// =====================================================================================
	// �e�Z�b�^�[�֐�
	// =====================================================================================

	// �E�B���h�E�̈ʒu��ݒ�
	inline void setWindowPos(const tnl::Vector2i& pos) { 
		window_pos_ = pos;
		mess_str_pos_ = window_pos_ + mess_str_top_pos_;
	}

	// �E�B���h�E�̃T�C�Y��ݒ�
	inline void setWindowSize(const tnl::Vector2i& size) { window_size_ = size + tnl::Vector2i( mess_str_top_pos_.x, mess_str_top_pos_.y * 2 - message_space_); }

	// ���b�Z�[�W�̕\���ʒu�̎n�_�����߂�
	inline void setMessageTopPos(const tnl::Vector2i pos) { 
		mess_str_top_pos_ = pos;
		mess_str_pos_ = window_pos_ + mess_str_top_pos_;
	}

	// ���b�Z�[�W�̃t�H���g�T�C�Y��ݒ�
	inline void setFontSize(int size) { message_font_size_ = size; }

	// ���b�Z�[�W�̕\���Ԋu��ݒ肷��
	inline void setMessageSpace(int message_space) { 
		window_size_.y += message_space_ - message_space;
		message_space_ = message_space;
	}

	// ���b�Z�[�W�̍ő�s����ύX
	inline void setMessageLine(int line) {
		message_line_ = line;
		message_.resize(message_line_);
	}

	// �\�����Ԃ�ݒ肷��
	inline void setTimeLimit(float time_limit) { 
		is_enable_ = true;
		draw_time_limit_ = time_limit;
		is_time_limit_ = true;
	}

	// 
	void setGpcHdl(int gpc_hdl) {
		gpc_hdl_ = gpc_hdl;
	}

	// 
	void setDrawGpcHdlPos(const tnl::Vector2i& pos) {
		draw_gpc_hdl_pos_ = pos;
	}

	// 
	void setDrawGpcHdlSize(const tnl::Vector2i& size) {
		draw_gpc_hdl_size_ = size;
	}

	// ���b�Z�[�W���Z�b�g����
	void setMessgae(const std::string& message);

	// �S�Ă̍s�̃��b�Z�[�W���Z�b�g����B�i �ő�s���͂ݏo�����s�̓Z�b�g����Ȃ��B �j
	void setAllLineMessage(const std::vector<std::string>& messages);

	// ���b�Z�[�W�E�B���h�E��\���A��\���̐ݒ������B
	inline void setEnable(bool enable) { is_enable_ = enable; }

	// Enter��UI��\�����邩�ݒ肷��
	inline void setEnableEnterUI(bool is_enable_enter_ui) { is_enable_enter_ui_ = is_enable_enter_ui; }

	// =====================================================================================
	// ���̑��֐�
	// =====================================================================================

	// ���b�Z�[�W��S�폜
	void clearMessage();

	// �\�����Ԃ̐ݒ��������
	inline void cancelTimeLimit() {
		draw_time_limit_ = 0.0f;
		is_time_limit_ = false;
	}

	// �s���A�����Ԋu�A�����T�C�Y����E�B���h�E�T�C�Y���v�Z����
	void calculateWindowSize();

};