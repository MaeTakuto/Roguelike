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
	tnl::Vector2i pos_;
	// �\���̕�
	int width_;
	// �\���̍���
	int height_;

	// �G�t�F�N�g�摜
	std::vector<int> anim_gpc_hdl_;
	// �u�����h���[�h
	int blend_mode_;

	// �L��������
	bool is_enable_;

	// ���݃t���[��
	int anim_frame_;

	// �t���[���̌o�ߎ���
	float frame_elapsed_;
	// �t���[���Ԋu
	float frame_change_interval_;

public:
	// �L�������肷��
	inline bool isEnable() { return is_enable_; }

	// �A�j���[�V�����t���[���؂�ւ��Ԋu��ݒ�
	inline void setFrameChangeInterval(float frame_change_interval) { frame_change_interval_ = frame_change_interval; }

	// �摜�̕\���ʒu��ݒ肷��
	inline void setDrawPos(const tnl::Vector2i& pos) { pos_ = pos; }

	// �摜�̕\���T�C�Y��ݒ肷��
	inline void setDrawSize(const tnl::Vector2i& size) {
		width_ = size.x;
		height_ = size.y;
	}

	// �摜�̃u�����h���[�h��ݒ肷��
	inline void setBlendMode(int blend_mode) { blend_mode_ = blend_mode; }

	// �A�j���[�V�����摜��ݒ�
	void setAnimGraphicHandle(const std::vector<int>& anim_gpc_hdl);
	
	// �A�j���[�V�������Đ�
	void startAnimation();

};