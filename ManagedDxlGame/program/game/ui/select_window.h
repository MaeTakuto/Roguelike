#pragma once

class Camera;

// =====================================================================================
// �I���E�B���h�E�̕\�����A�I��������N���X
// =====================================================================================
class SelectWindow {
public:
	SelectWindow();
	~SelectWindow();

	void update(float delta_time);
	void draw();

private:
	// �I���E�B���h�E�̈ʒu
	tnl::Vector2i window_pos_;

	// �I���E�B���h�E�̃T�C�Y
	tnl::Vector2i window_size_;

	// �I�𒆃R�}���h�̈ʒu
	std::vector<tnl::Vector2i> selected_cmd_ui_pos_;

	// �I�𒆃R�}���h�̃T�C�Y
	tnl::Vector2i selected_cmd_ui_size_;
	// �I�𒆃R�}���hUI�̕\���ʒu�̊Ԋu����
	tnl::Vector2i seleted_cmd_ui_space_;
	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_top_pos_;
	// ���b�Z�[�W�̈ʒu
	tnl::Vector2i mess_str_pos_;

	// �I���E�B���h�E���`�悷�邩����
	bool is_drawing_;
	// �I���E�B���h�E�𑀍삷�邩����
	bool is_operate_;

	// �I�𒆂̃R�}���hUI�̕`�悷�邩����
	bool is_drawing_selected_cmd_ui_;
	// �I�𒆂�UI�̕\���؂�ւ��Ԋu
	float swicth_draw_selected_ui_interval_;
	// �؂�ւ�����̌o�ߎ���
	float swicth_draw_selected_ui_elapsed_;

	// �\�����郁�b�Z�[�W
	std::vector<std::string> command_names_;

	// �����̃T�C�Y
	int message_font_size_;

	// �I���R�}���h�̍ő吔
	int select_cmd_max_;

	// ���b�Z�[�W�E�B���h�E�ɏo���镶���̍ő�s��
	int message_line_;

	// �s�ƍs�̊Ԋu
	int message_space_;

	// ���݂̑I���J�[�\���̃C���f�b�N�X�B
	int selected_cmd_index_;

public:

	// ==============================================
	// �Q�b�^�[
	// ==============================================

	// �I���E�B���h�E���L������Ԃ�
	inline bool isDrawing() { return is_drawing_; }

	// �I���E�B���h�E�𑀍�ł��邩�����Ԃ�
	inline bool isOperate() { return is_operate_; }

	// �I���E�B���h�E�̈ʒu���擾
	const tnl::Vector2i& getWindowPos() const { return window_pos_; }

	// �I���E�B���h�E�̃T�C�Y���擾
	const tnl::Vector2i& getWindowSize() const { return window_size_; }

	// �I�𒆂̃R�}���h�ԍ���Ԃ��B
	inline int getSelectedCmdIndex() { return selected_cmd_index_; }

	// �I���ł���R�}���h�̍ő吔���擾����
	inline int getSelectCmdMax() { return select_cmd_max_; }

	// ==============================================
	// �Z�b�^�[
	// ==============================================

	// �I���E�B���h�E�̕\���ʒu��ݒ�
	void setWindowPos(const tnl::Vector2i& pos);
	// �I���E�B���h�E�̃T�C�Y��ݒ�
	void setWindowSize(const tnl::Vector2i& size);
	
	// �R�}���h���b�Z�[�W�̕\���Ԋu��ݒ�
	inline void setMessageSpace(int space) { message_space_ = space; }

	// �R�}���h���̃t�H���g�T�C�Y��ݒ�
	inline void setFontSize(int font_size) {
		message_font_size_ = font_size;
		selected_cmd_ui_size_.y = font_size;
	}

	// �I���R�}���h�̐���ݒ�
	void setSelectCmdMax(int select_cmd_max);

	// �e�I���R�}���h�̖��O��ݒ�
	void setCommandNames(const std::vector<std::string>& command_names);

	// �I���E�B���h�E��`�悷�邩�ݒ肷��
	inline void setDrawing(bool is_drawing) {
		is_drawing_ = is_drawing;
		selected_cmd_index_ = 0;
	}

	// �I���E�B���h�E�𑀍삷�邩�ݒ肷��
	inline void setOperate(bool is_operate) {
		is_operate_ = is_operate;
		is_drawing_selected_cmd_ui_ = true;
		swicth_draw_selected_ui_elapsed_ = 0.0f;
	}

	// ==============================================
	// ���̑�
	// ==============================================

	// �E�B���ÃT�C�Y�i ���� �j���v�Z����B
	void calculateWindowSize();

};