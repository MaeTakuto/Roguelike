#pragma once

class Camera;

// =====================================================================================
// �A�j���[�V�����̍Đ����s���N���X
// =====================================================================================
class Animation final {
public:
	Animation();
	~Animation();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// �G�t�F�N�g�̕\���ʒu
	tnl::Vector2i draw_pos_;
	// �G�t�F�N�g�̃T�C�Y
	tnl::Vector2i draw_size_;

	// �A�j���[�V�����`�b�v�̃T�C�Y
	tnl::Vector2i chip_size_;

	// �G�t�F�N�g�摜
	std::vector<int> anim_gpc_hdl_;
	// �u�����h���[�h
	int blend_mode_;

	// �L��������
	bool is_enable_;
	// �A�j���[�V���������[�v�����邩����
	bool is_loop_;

	// ���݃t���[��
	int anim_frame_;

	// �t���[���̌o�ߎ���
	float frame_elapsed_;
	// �t���[���Ԋu
	float frame_change_interval_;

public:
	// �L�������肷��
	inline bool isEnable() { return is_enable_; }

	// 1�`�b�v�̉摜�T�C�Y���擾
	const tnl::Vector2i& getAnimChipSize() { return chip_size_; };

	// �A�j���[�V���������[�v�����邩�ݒ肷��
	inline void setLoopAnimation(bool is_loop) { is_loop_ = is_loop; };

	// �A�j���[�V�����t���[���؂�ւ��Ԋu��ݒ�
	inline void setFrameChangeInterval(float frame_change_interval) { frame_change_interval_ = frame_change_interval; }

	// �摜�̕\���ʒu��ݒ肷��
	inline void setDrawPos(const tnl::Vector2i& pos) { draw_pos_ = pos; }

	// �摜�̕\���T�C�Y��ݒ肷��
	inline void setDrawSize(const tnl::Vector2i& size) {
		draw_size_ = size;
	}

	// �摜�̃u�����h���[�h��ݒ肷��
	inline void setBlendMode(int blend_mode) { blend_mode_ = blend_mode; }

	// �A�j���[�V�����摜��ݒ�
	void setAnimGraphicHandle(const std::vector<int>& anim_gpc_hdl) {
		anim_gpc_hdl_ = anim_gpc_hdl;
	}
	
	// 
	void calculateChipSize();

	// �A�j���[�V�������Đ�
	void startAnimation();

	// �A�j���[�V�������~
	void stopAnimation();

};