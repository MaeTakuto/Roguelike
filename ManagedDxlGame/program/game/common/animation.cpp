#include "../dxlib_ext/dxlib_ext.h"
#include "../common/camera.h"
#include "animation.h"

// =====================================================================================
// コンストラクタ
// =====================================================================================
Animation::Animation() : pos_(0, 0), anim_frame_(0), blend_mode_(DX_BLENDMODE_NOBLEND), is_enable_(false), frame_elapsed_(0.0f), frame_change_interval_(3.0f)
{

}

// =====================================================================================
// デストラクタ
// =====================================================================================
Animation::~Animation() {

}

// =====================================================================================
// アップデート
// =====================================================================================
void Animation::update(float delta_time) {

	if (!is_enable_) {
		return;
	}

	frame_elapsed_ += delta_time;

	if ( frame_elapsed_ > frame_change_interval_ ) {
		frame_elapsed_ = 0.0f;
		++anim_frame_;

		// アニメーションの終わりまでいった場合
		if (anim_frame_ >= anim_gpc_hdl_.size()) {
			is_enable_ = false;
		}
	}

}

// =====================================================================================
// 描画
// =====================================================================================
void Animation::draw(const std::shared_ptr<Camera> camera) {

	if (!is_enable_) {
		return;
	}

	tnl::Vector2i draw_pos = pos_ - tnl::Vector2i(static_cast<int>(camera->getPos().x), static_cast<int>(camera->getPos().y));

	SetDrawBlendMode(blend_mode_, 255);
	DrawExtendGraph(draw_pos.x, draw_pos.y, draw_pos.x + width_, draw_pos.y + height_, anim_gpc_hdl_[anim_frame_], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

// =====================================================================================
// エフェクト画像をセット
// =====================================================================================
void Animation::setAnimGraphicHandle(const std::vector<int>& effect_gpc_hdl) {

	anim_gpc_hdl_.resize(effect_gpc_hdl.size());

	for (int i = 0; i < effect_gpc_hdl.size(); ++i) {
		anim_gpc_hdl_[i] = effect_gpc_hdl[i];
	}

}

// =====================================================================================
// アニメーション開始
// =====================================================================================
void Animation::startAnimation() {

	anim_frame_ = 0;
	is_enable_ = true;
	frame_elapsed_ = 0.0f;
}
