#include "../../dxlib_ext/dxlib_ext.h"
#include "../common/camera.h"
#include "animation.h"

// =====================================================================================
// �R���X�g���N�^
// =====================================================================================
Animation::Animation() : pos_(0, 0), size_(0, 0), anim_frame_(0), blend_mode_(DX_BLENDMODE_NOBLEND), is_enable_(false), is_loop_(false),
	frame_elapsed_(0.0f), frame_change_interval_(0.5f)
{

}

// =====================================================================================
// �f�X�g���N�^
// =====================================================================================
Animation::~Animation() {

}

// =====================================================================================
// �A�b�v�f�[�g
// =====================================================================================
void Animation::update(float delta_time) {

	if (!is_enable_) {
		return;
	}

	frame_elapsed_ += delta_time;

	if ( frame_elapsed_ > frame_change_interval_ ) {
		frame_elapsed_ = 0.0f;
		++anim_frame_;

		// �A�j���[�V�����̏I���܂ł������ꍇ
		if (anim_frame_ < anim_gpc_hdl_.size()) {
			return;
		}

		// ���[�v������ꍇ
		if (is_loop_) {
			anim_frame_ = 0;
			return;
		}
		
		is_enable_ = false;
	}

}

// =====================================================================================
// �`��
// =====================================================================================
void Animation::draw(const std::shared_ptr<Camera> camera) {

	if (!is_enable_) {
		return;
	}

	tnl::Vector2i draw_pos = pos_ - tnl::Vector2i(static_cast<int>(camera->getPos().x), static_cast<int>(camera->getPos().y));

	SetDrawBlendMode(blend_mode_, 255);
	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + size_.x, draw_pos.y + size_.y, anim_gpc_hdl_[anim_frame_], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

// =====================================================================================
// �A�j���[�V�����J�n
// =====================================================================================
void Animation::startAnimation() {

	anim_frame_ = 0;
	is_enable_ = true;
	frame_elapsed_ = 0.0f;
}
