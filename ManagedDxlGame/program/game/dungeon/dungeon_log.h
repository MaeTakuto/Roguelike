#pragma once
#include "../common/chara_status.h"

class MessageWindow;
class CharaStatus;

// =====================================================================================
// �_���W�����T���I�����̃_���W�����ł̋L�^��ۑ����A�\������N���X
// =====================================================================================
class DungeonLog {
public:
	DungeonLog();
	~DungeonLog();

	// "DungeonLog"�N���X�̕`�������
	void draw();

	// �`�悷�邩�ݒ肷��
	inline void setDrawing(bool is_drawing) { is_drawing_ = is_drawing; }

	// �_���W�����N���A�̃t���O���Z�b�g����
	inline void setDungeonClear(bool is_clear) { is_dungeon_clear_ = is_clear; }

	// �I�����̃��x�����Z�b�g����
	inline void setEndFloor(int floor) { end_floor_ = floor; };

	// �G�̌��ސ������Z����
	inline void additionRepellingEnemy() { ++repelling_enemy_count_; }

	// �I�����̃��b�Z�[�W���Z�b�g����B
	inline void setEndMessage(std::string message) { end_message_ = message; }

	// �I�����̃��x�����Z�b�g����
	void setEndStatus(CharaStatus status);
	// �_���W�����L�^�̍X�V
	void updateDungeonLogMessage();

private:
	// �_���W�����L�^�E�B���h�E
	std::shared_ptr<MessageWindow> dungeon_log_window_;

	// �I�������x��
	CharaStatus end_status_;
	// �I�����̃��b�Z�[�W
	std::string end_message_;
	// �_���W�����L�^�̃��b�Z�[�W�ꗗ
	std::vector<std::string> log_messages_;

	// �`�悵�Ă��邩����
	bool is_drawing_;
	// �_���W�������N���A����������
	bool is_dungeon_clear_;

	// ���B�t���A
	int end_floor_;
	// �G�̌��ސ�
	int repelling_enemy_count_;

};