#pragma once

class Camera;

class SelectWindow {
public:
	SelectWindow();
	~SelectWindow();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// ���b�Z�[�W�E�B���h�E�̉摜�p
	int mess_window_gpc_hdl_ = 0;

	// �I���J�[�\���̉摜�p
	int select_cursor_gpc_hdl_ = 0;

	// �I���E�B���h�E�̈ʒu�A�T�C�Y
	tnl::Vector2i window_pos_ = { 950, 450 };
	tnl::Vector2i window_size_ = { 150, 120 };

	// �I�𒆃R�}���h�̈ʒu�A�T�C�Y
	std::vector<tnl::Vector2i> selected_cmd_pos_;
	tnl::Vector2i selected_cmd_size_ = { window_size_.x - 20, 30 };


	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_pos_ = window_pos_ + tnl::Vector2i(30, 20);

	// �I���E�B���h�E���L��������
	bool is_enable_ = false;

	// �I�𒆂̃R�}���h���u�͂��v������B
	bool is_yes_ = true;

	// �\�����郁�b�Z�[�W
	std::vector<std::string> message_;

	// �\�����Ă��郁�b�Z�[�W��
	int display_message_count_ = 0;

	// �����̃T�C�Y
	int message_font_size_ = 30;

	// ���b�Z�[�W�E�B���h�E�ɏo���镶���̍ő�s��
	int message_line_ = 2;

	// �s�ƍs�̊Ԋu
	int message_space_ = 50;

	// ���݂̑I���J�[�\���̃C���f�b�N�X�B
	int selected_cmd_index_ = 0;

public:
	// �I���E�B���h�E���L������Ԃ�
	inline bool isEnable() { return is_enable_; }

	// �I���E�B���h�E��L���A�����ɂ���
	inline void setEnable(bool is_enable) { is_enable_ = is_enable; }

	// �I�𒆂̃R�}���h���u�͂��v����Ԃ��B
	inline bool isYes() { return is_yes_; }

	// �I���E�B���h�E�̏�����
	inline void init() {
		is_yes_ = true;
		selected_cmd_index_ = 0;
	}

};