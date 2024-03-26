#pragma once
#include "../common/enum.h"

class Camera;
class Character;
class Animation;

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
	eMagicTarget magic_target_;

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
	virtual void setupToUseMagic(const std::shared_ptr<Character> user) = 0;
	virtual void startDrawEffectOnOwner(const tnl::Vector2i& pos, const tnl::Vector2i& size) {};
	virtual void startDrawEffectOnOther(const tnl::Vector2i& pos, const tnl::Vector2i& size, eDir_8 other_dir) {};
	virtual void useMagic(std::shared_ptr<Character> owner) = 0;

};