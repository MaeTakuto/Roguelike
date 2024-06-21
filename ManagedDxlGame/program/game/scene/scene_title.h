#pragma once
#include "../base/scene_base.h"

class DungeonLog;
class SelectWindow;

class SceneTitle final : public SceneBase {
public:
	SceneTitle();
	~SceneTitle();

	void update(float delta_time) override;
	void draw() override;

private:
	// タイトル画面シーケンス
	tnl::Sequence<SceneTitle> sequence_;

	// タイトルメニュー
	std::shared_ptr<SelectWindow> title_menu_;
	// ダンジョンログメニュー
	std::shared_ptr<SelectWindow> dungeon_log_menu_;
	// ダンジョン記録リスト
	std::vector<std::shared_ptr<DungeonLog>> dungeon_log_list_;

	// タイトル背景画像
	int back_ground_gpc_hdl_;
	// 日差しの画像
	int sunlight_gpc_hdl_;

	// タイトルBGMパス
	std::string title_bgm_hdl_path_;

	// タイトルBGM
	int title_bgm_hdl_;
	// BGMの終了地点のサンプリング周波数
	int bgm_end_freqency_;
	// タイトルメニューの透明度
	int title_menu_alpha_;
	// 日差し画像の透明度
	int sunlight_alpha_;
	// 透明度の中央
	int alpha_center_;
	// sin の範囲
	int sin_range_;

	// シーン経過時間
	float scene_elapsed_time_;

	// シーン遷移中シーケンス
	bool seqSceneTransition(const float);
	// タイトル画面での処理シーケンス
	bool seqRun(const float delta_time);
	// メニュー選択画面シーケンス
	bool seqSelectMenu(const float delta_time);
	// ダンジョン記録を選択するシーケンス
	bool seqSelectDungeonLogList(const float delta_time);

};