#pragma once

class Camera;

// =====================================================================================
// �L�����N�^�[�����˂��铊�����i target_pos_�̕����Ɍ����� �j
// =====================================================================================
class Projectile {
public:
	Projectile();
	~Projectile();

	// �A�b�v�f�[�g
	void update(float delta_time);
	// �`��
	void draw(const std::shared_ptr<Camera> camera);

private:
	// ���݈ʒu
	tnl::Vector3 pos_;
	// �ړ��������
	eDir_8 move_dir_;

	// �ڕW�̈ʒu
	tnl::Vector3 target_pos_;
	// ���ˑ̂��q�b�g�����L�����N�^�[
	std::shared_ptr<Character> hit_character_;

	// �L��������
	bool is_enable_;
	// �������̉摜
	int projectile_gpc_hdl_;
	// ���ˑ̂��͂��ő�Z����
	int max_cell_reach_;
	// 
	int cell_count_;

public:
	void setupToLaunchProjectile(const tnl::Vector3& start_pos, eDir_8 move_dir, int max_cell_reach);
	void startToLaunchProjectile();
	void checkCellInMoveDir(const tnl::Vector3& pos);

	// -------------- �Q�b�^�[ ------------------
	// �L���ɂȂ��Ă��邩����
	inline bool isEnable() { return is_enable_; }
	// ���ˑ̂ɓ��������L�����N�^�[���擾�A���Ȃ���� "nullptr" ��Ԃ��B
	inline std::shared_ptr<Character> getHitCharacter() { return hit_character_; }

	// -------------- �Z�b�^�[ ------------------
	// �������̈ʒu���Z�b�g
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	// �ڕW�̈ʒu
	inline void setTargetPos(const tnl::Vector3& target_pos) { target_pos_ = target_pos; }
	// �������̉摜���Z�b�g
	inline void setProjectileGpcHdl(int gpc_hdl) { projectile_gpc_hdl_ = gpc_hdl; }
	// �L���ɂ��邩�ݒ�
	inline void setEnable(bool is_enable) { is_enable_ = is_enable; }
};