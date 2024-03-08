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

	// -------------- �Q�b�^�[ ------------------
	// �L���ɂȂ��Ă��邩����
	inline bool isEnable() { return is_enable_; }

	// -------------- �Z�b�^�[ ------------------
	// �������̈ʒu���Z�b�g
	inline void setPos(const tnl::Vector3& pos) { pos_ = pos; }
	// �ڕW�̈ʒu
	inline void setTargetPos(const tnl::Vector3& target_pos) { target_pos_ = target_pos; }
	// �������̉摜���Z�b�g
	inline void setProjectileGpcHdl(int gpc_hdl) { projectile_gpc_hdl_ = gpc_hdl; }
	// �L���ɂ��邩�ݒ�
	inline void setEnable(bool is_enable) { is_enable_ = is_enable; }

private:
	// ���݈ʒu
	tnl::Vector3 pos_;
	// �ڕW�̈ʒu
	tnl::Vector3 target_pos_;
	// �L��������
	bool is_enable_;
	// �������̉摜
	int projectile_gpc_hdl_;

};