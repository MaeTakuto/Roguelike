#pragma once
#include "../common/enum.h"

class Camera;
class Character;
class Animation;

// =====================================================================================
// ���@���쐬���邽�߂̃x�[�X�N���X
// =====================================================================================
class MagicBase {
public:
	MagicBase();
	virtual ~MagicBase();

	virtual void update(float delta_time) = 0;
	virtual void draw(const std::shared_ptr<Camera> camera) = 0;

protected:
	// ���@�G�t�F�N�g
	std::shared_ptr<Animation> magic_effect_;
	// �G�t�F�N�g�摜
	std::vector<int> magic_effect_gpc_hdl_;
	// ���@��
	std::string magic_name_;
	// ���@�̌��ʑΏ�
	eMagicTarget magic_target_;
	// �G�t�F�N�g�̕\���t���O
	bool is_draw_effect_;
	// ����MP
	int consumed_mp_;

	// ���@�̐���
	std::vector<std::string> magic_explantion_;

public:
	inline bool isDrawEffect() const { return is_draw_effect_; }
	inline int getConsumedMP() const { return consumed_mp_; }
	inline const std::string& getMagicName() const { return magic_name_; }
	// ���@�̌��ʑΏۂ�Ԃ�
	inline eMagicTarget getMagicTarget() const { return magic_target_; }
	// ���@�̐�������Ԃ�
	inline const std::vector<std::string>& getMagicExplantion() const { return magic_explantion_; }

	// �g�����@�̏���������
	virtual void setupToUseMagic(const std::shared_ptr<Character> owner) = 0;
	virtual void startDrawEffect() = 0;
	virtual void useMagic(std::shared_ptr<Character> owner) = 0;

};