#pragma once

// =====================================================================================
// �Ώۂ̃X�e�[�^�X���Q�[�W�ŕ\������UI�N���X
// =====================================================================================
class StatusBar {
public:
	StatusBar();
	~StatusBar();

	void update(float delta_time);
	void draw();

	enum class eStatusBarColor {
		GREEN = 0,
		LIGHT_BLUE,
		MAX
	};

private:
	// �X�e�[�^�X�o�[�̃t�����g�摜
	int status_bar_front_gpc_hdl_[std::underlying_type<eStatusBarColor>::type(eStatusBarColor::MAX)];
	// �X�e�[�^�X�o�[�̃o�b�N�摜
	int status_bar_back_gpc_hdl_;

	// �X�e�[�^�X�o�[�̈ʒu
	tnl::Vector2i status_bar_back_pos_;
	// �X�e�[�^�X�o�[�̃T�C�Y
	tnl::Vector2i status_bar_back_size_;

	// �X�e�[�^�X�o�[�̈ʒu
	tnl::Vector2i status_bar_front_pos_;
	// �X�e�[�^�X�o�[�̈ʒu
	tnl::Vector2i status_bar_front_size_;
	// 
	tnl::Vector2i status_text_pos_;

	// �I�������X�e�[�^�X�o�[�̐F
	int selected_color_;

	// �X�e�[�^�X�̍ő�l�A���ݒl
	int max_status_;
	int now_status_;

	std::string status_type_text_;
	std::string status_text_;

	// �����T�C�Y
	int message_font_size_;

	// �X�e�[�^�X�o�[�̗֊s�T�C�Y
	int status_bar_outline_size_;

public:
	// �X�e�[�^�X�o�[�̈ʒu��ݒ�
	inline void setStatusBarPos(const tnl::Vector2i& pos) { 
		status_bar_back_pos_ = pos;
		status_bar_front_pos_ = status_bar_back_pos_ + (status_bar_back_size_ - status_bar_front_size_) / 2;
		status_text_pos_ = status_bar_back_pos_ - tnl::Vector2i(180, -5);
	}
	// �X�e�[�^�X�o�[�̃T�C�Y��ݒ�
	inline void setStatusBarSize(const tnl::Vector2i& size) { status_bar_back_size_ = size; }
	// �\������X�e�[�^�X�̃^�C�v��ݒ�i HP, MP�Ȃ� �j
	inline void setStatusTypeText(const std::string& status_type_text) { status_type_text_ = status_type_text; }
	// �X�e�[�^�X�̍ő�l��ݒ�
	inline void setMaxStatus(int max_status) { max_status_ = max_status; }
	// ���݂̃X�e�[�^�X�̒l��ݒ�
	inline void setStatus(int now_status) { now_status_ = now_status; }
	//
	void setStatusBarColor(eStatusBarColor selected_color) {
		selected_color_ = std::underlying_type<eStatusBarColor>::type(selected_color);
	}

	// �X�e�[�^�X�o�[�����݂̒l�ɍX�V
	void updateStatus_Text(int max_status, int now_status);

};