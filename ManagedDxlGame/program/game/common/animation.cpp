#include "../dxlib_ext/dxlib_ext.h"
#include "animation.h"

// =====================================================================================
// コンストラクタ
// =====================================================================================
Animation::Animation() : pos_(0, 0), anim_frame_(0), is_enable_(false), frame_elapsed_(0.0f), frame_change_interval_(3.0f)
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

	DrawExtendGraph(pos_.x, pos_.y, pos_.x + width_, pos_.y + height_, anim_gpc_hdl_[anim_frame_], true);
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
