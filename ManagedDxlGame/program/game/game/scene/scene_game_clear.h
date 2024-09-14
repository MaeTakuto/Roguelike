#pragma once
#include "../base/scene_base.h"

class MessageWindow;

// =====================================================================================
// ゲームクリア後のシーンクラス
// =====================================================================================
class SceneGameClear final : public SceneBase {
public:
	SceneGameClear();
	~SceneGameClear();

	void update(float delta_time) override;
	void draw() override;

private:
	tnl::Sequence<SceneGameClear> sequence_;

	std::shared_ptr<MessageWindow> message_window_;

	// 表示しているメッセージのページ
	int now_page_;

	// エンディングイベントのメッセージ
	std::vector< std::vector< std::string > > event_messages_;

	// SEを再生中か判定
	bool is_playing_se_;

	// 現在表示中のメッセージに再生する効果音のパス（ 再生する行、SEのパス ）
	std::queue<std::pair<int, std::string>> se_path_to_message_page_;

	// タイトル背景画像
	int back_ground_gpc_hdl_;

private:
	// SEを再生するか確認
	void checkToPlaySE();

	// ゲームクリアシーンのシーケンス
	bool seqSceneStart(const float delta_time);
	bool seqMessageEvent(const float delta_time);
	bool seqSceneEnd(const float delta_time);

};

