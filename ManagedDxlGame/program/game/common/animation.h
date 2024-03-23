#pragma once

class Camera;

// =====================================================================================
// アニメーションの再生を行うクラス
// =====================================================================================
class Animation final {
public:
	Animation();
	~Animation();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// エフェクトの表示位置
	tnl::Vector2i pos_;
	// 表示の幅
	int width_;
	// 表示の高さ
	int height_;

	// エフェクト画像
	std::vector<int> anim_gpc_hdl_;
	// ブレンドモード
	int blend_mode_;

	// 有効か判定
	bool is_enable_;

	// 現在フレーム
	int anim_frame_;

	// フレームの経過時間
	float frame_elapsed_;
	// フレーム間隔
	float frame_change_interval_;

public:
	// 有効か判定する
	inline bool isEnable() { return is_enable_; }

	// アニメーションフレーム切り替え間隔を設定
	inline void setFrameChangeInterval(float frame_change_interval) { frame_change_interval_ = frame_change_interval; }

	// 画像の表示位置を設定する
	inline void setDrawPos(const tnl::Vector2i& pos) { pos_ = pos; }

	// 画像の表示サイズを設定する
	inline void setDrawSize(const tnl::Vector2i& size) {
		width_ = size.x;
		height_ = size.y;
	}

	// 画像のブレンドモードを設定する
	inline void setBlendMode(int blend_mode) { blend_mode_ = blend_mode; }

	// アニメーション画像を設定
	void setAnimGraphicHandle(const std::vector<int>& anim_gpc_hdl);
	
	// アニメーションを再生
	void startAnimation();

};