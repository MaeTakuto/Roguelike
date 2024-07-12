#pragma once

class MagicBase;
class MessageWindow;
class UI_Manager;
class Character;
class Camera;
class Animation;

// =====================================================================================
// �o�����閂�@�������_���ɑI�т��A�I���������@���K���ł���N���X
// 
// �I���ł��閂�@�́A"magic_selector.cpp"���ɂ���"SELECT_MAGIC_MAX"�̐�����ݒ肵�܂��B
// =====================================================================================
class MagicSelector {
public:
	MagicSelector();
	~MagicSelector();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// �}�W�b�N�Z���N�^�[�̃V�[�P���X
	tnl::Sequence<MagicSelector> sequence_;

	// UI_Manager�̃|�C���^�擾�p
	std::weak_ptr<UI_Manager> ui_manager_;

	// ���@���K������L�����N�^�[
	std::weak_ptr<Character> learning_character_;

	// ���@�̃��X�g
	std::vector< std::shared_ptr<MagicBase> > magic_list_;

	// �I���ł��閂�@���X�g
	std::vector< std::shared_ptr<MagicBase> > select_magic_list_;

	// ���@�̐����E�B���h�E
	std::vector<std::shared_ptr<MessageWindow> > magic_explanation_window_;

	// ���@�����E�B���h�E�̈ʒu
	std::vector<tnl::Vector2i> window_pos_;

	// �J�[�\���A�j���[�V����
	std::shared_ptr<Animation> cursor_anim_;

	// �I�����ʂ�\��
	std::string selected_result_log_;

	// �I���I������������
	bool is_select_end_;

	// �`�悷�邩����
	bool is_drawing_;

	// ���@���w�Ԃ�����
	bool is_learn_magic_;

	// �I�𒆂̖��@�E�B���h�E
	int selected_magic_window_;

public:
	// �I���I������������
	inline bool isSelectEnd() const { 
		return is_select_end_; 
	}

	// ���@���w�񂾂�����
	inline bool isLearnMagic() const {
		return is_learn_magic_;
	}

	// UI_Manager���Z�b�g
	inline void setUI_Manager(std::shared_ptr<UI_Manager> ui_manager) { 
		ui_manager_ = ui_manager; 
	}

	// ���@���o����"Character"�N���X��ݒ肷��
	inline void setLearningCharacter(std::shared_ptr<Character> learning_character) {
		learning_character_ = learning_character;
	}

	// ���@�I�������s
	void beginMagicSelect();

private:

	// �o�����閂�@�������_���Ɍ��肷��
	void getRandomMagic();
	// ���@�I�����I������
	void endToSelectMagic();
	// ���@�̃��x�����グ��
	void levelUpMagic(std::shared_ptr<MagicBase> magic);

	// ���b�Z�[�W��\������V�[�P���X
	bool seqDisplayMessage(const float delta_time);
	// �o���閂�@��I������V�[�P���X
	bool seqMagicSelect(const float delta_time);
	// �u�͂��v���u�������v��I������V�[�P���X
	bool seqSelectYesOrNo(const float delta_time);
	// ���@�������A�K������V�[�P���X
	bool seqLearnMagic(const float delta_time);

};