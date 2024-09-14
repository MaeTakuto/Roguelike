#pragma once
#include "../common/enum.h"

class Camera;
class Character;
class Animation;
class MagicStatus;

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
	// ���@�̃A�C�R���摜
	int magic_icon_gpc_hdl_;
	// ���@�G�t�F�N�g
	std::shared_ptr<Animation> magic_effect_;
	// �G�t�F�N�g�摜
	std::vector<int> magic_effect_gpc_hdl_;
	// ���@�̃X�e�[�^�X
	std::shared_ptr<MagicStatus> magic_status_;
	// ���x���A�b�v���̏㏸�X�e�[�^�X
	std::shared_ptr<MagicStatus> level_up_bonus_status_;
	// ���@�̌��ʑΏ�
	eMagicTarget magic_target_;
	// �G�t�F�N�g�̕\���t���O
	bool is_draw_effect_;
	// ���@���ʗ�
	int magic_effect_amount_;

	// ���@�̐���
	std::vector<std::string> magic_explantion_;

	// ���@�̐����̃x�[�X����
	std::vector<std::string> magic_status_explantion_;

public:
	// �G�t�F�N�g��`�悵�Ă��邩���肷��
	inline bool isDrawEffect() const { return is_draw_effect_; }
	
	// ���@�̃A�C�R���摜��Ԃ�
	inline int getMagicIconGpcHdl() const { return magic_icon_gpc_hdl_; }

	// ���@�̃X�e�[�^�X���擾����
	inline const std::shared_ptr<MagicStatus> getMagicStatus() const { return magic_status_; }

	// ���@���ʗʂ��擾�i �񕜗ʁA�_���[�W�ʂȂ� �j
	inline int getMagicEffectAmount() const { return magic_effect_amount_; }
	
	// ���@�̌��ʑΏۂ�Ԃ�
	inline eMagicTarget getMagicTarget() const { return magic_target_; }
	// ���@�̐�������Ԃ�
	inline const std::vector<std::string>& getMagicExplantion() const { return magic_explantion_; }
	
	//
	std::vector<std::string> getStatusComparisonText() const;

	// 
	std::vector<std::string> getLevelUpStatusComparisonText() const;

	// ���g�̃N���[�����@�𐶐�����
	virtual std::shared_ptr<MagicBase> createClone() = 0;

	// ���@�̃��x�����グ��
	virtual void levelUpMagic() = 0;

	// �g�����@�̏���������
	virtual void setupToUseMagic(const std::shared_ptr<Character> owner) = 0;
	virtual void startDrawEffect() = 0;
	virtual void useMagic(std::shared_ptr<Character> owner) = 0;

protected:
	// ���@�������X�V
	virtual void updateMagicExplantion() = 0;

};