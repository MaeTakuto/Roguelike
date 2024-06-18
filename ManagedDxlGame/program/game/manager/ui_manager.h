#pragma once

class Camera;
class MessageWindow;
class SelectWindow;
class StatusBar;
class Charater;
class MagicBase;

// =====================================================================================
// �Q�[������UI���Ǘ�����N���X
// =====================================================================================
class UI_Manager {
public:
	UI_Manager();
	~UI_Manager();

	// �X�V�ƕ`��
	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// ------------------- �e UI�N���X -------------------
	// ���b�Z�[�W�E�B���h�E
	std::shared_ptr<MessageWindow> message_window_;
	// ��������E�B���h�E
	std::shared_ptr<MessageWindow> control_explanation_window_;
	// �X�e�[�^�X���m�F����E�B���h�E
	std::shared_ptr<MessageWindow> view_status_window_;
	// ���@�����̃E�B���h�E
	std::shared_ptr<MessageWindow> magic_explation_window_;
	// ���C�����j���[�̑I���E�B���h�E
	std::shared_ptr<SelectWindow> main_menu_select_window_;
	// ���@�ꗗ�̑I���E�B���h�E
	std::shared_ptr<SelectWindow> magic_select_window_;
	// ����̑I���E�B���h�E�i �͂��A�������� �j
	std::shared_ptr<SelectWindow> two_select_window_;
	// ����R�}���h�̃R�}���h��
	std::vector<std::vector<std::string>> two_select_cmd_names_;
	// �v���C���[��HP�o�[
	std::shared_ptr<StatusBar> hp_bar_;
	// �v���C���[��MP�o�[
	std::shared_ptr<StatusBar> mp_bar_;

	// ��������̃��b�Z�[�W
	std::vector< std::vector< std::string > > control_explanation_message_;

	// UI�̃X�e�[�^�X�Q�Ɨp
	std::weak_ptr<Character> ui_target_;

	// ���݂̊K��
	int floor_;

public:
	inline void setUITargetCharacter(std::weak_ptr<Character> target) {
		ui_target_ = target;
	}

	// �K�i�I���̌��ݑI�𒆂̃C���f�b�N�X��Ԃ�
	int getSelectedIndexFromTwoSelectCmd();

	// ---------------- ���C�����j���[�֘A�֐� -------------------------

	// ���C�����j���[�R�}���h�̑I�����Ă���C���f�b�N�X��Ԃ�
	int getSelectedIndexFromMainMenuCmd();
	// ���C�����j���[���J��
	void openMainMenu();
	// ���C�����j���[�����
	void closeMainMenu();
	// ���@�I���E�B���h�E�̈ꗗ�X�V
	void updateMagicList();
	// ���@���X�g�̑I�����Ă���C���f�b�N�X��Ԃ�
	int getSelectedIndexFromMagicListCmd();
	// ���@�I���E�B���h�E���J��
	void openMagicListWindow();
	// ���@�I���E�B���h�E�����
	void closeMagicListWindow();
	// ���@�̐������X�V
	void updateMagicExplation();
	// ���@���g�����I������E�B���h�E��\��
	void executeSletctToUseMagic();
	// ���@���g�����I������E�B���h�E�����
	void executeSletctToUseMagicEnd();
	// �X�e�[�^�X�E�B���h�E��\������
	void displayStatusWindow();
	// �X�e�[�^�X�E�B���h�E���\���ɂ���
	void hideStatusWindow();

	// -------------------------------------------------------------------

	// �K�i�ł̑I���̎��s
	void executeStairSelect();
	// �K�i�ł̑I���̎��s
	void executeStairSelectEnd();

	// �����ʃE�B���h�E�̕\����؂�ւ���
	void changeCtrlExplanationWindowType(int window_type);

	// ���b�Z�[�W�E�B���h�E�Ƀ��b�Z�[�W���Z�b�g
	void setMessage(const std::string& message, float draw_time = -1.0f);
	// ���b�Z�[�W�E�B���h�E�̃��b�Z�[�W��S�폜����
	void clearMessage();

	// �X�e�[�^�X�o�[�̏�Ԃ��X�V
	void updateStatusBar();

	// ���݂̊K�����Z�b�g
	void setFloor(int floor) { floor_ = floor; }
};