#include "../../dxlib_ext/dxlib_ext.h"
#include "../gm_main.h"
#include "../common/enum.h"
#include "../base/scene_base.h"
#include "../dungeon/dungeon_log.h"
#include "gm_manager.h"
#include "resource_manager.h"

// コンストラクタ
GameManager::GameManager(std::shared_ptr<SceneBase> start_scene) : now_scene_(nullptr), next_scene_(nullptr), fade_time_(1.0f), fade_time_wait_(0.25f),
	is_transition_(false), is_game_clear_(false), sequence_(tnl::Sequence<GameManager>(this, &GameManager::seqTransIn))
{
	now_scene_ = start_scene;
	transition_gpc_hdl_path_ = "graphics/black.bmp";

	transition_gpc_hdl_ = LoadGraph(transition_gpc_hdl_path_.c_str());

	dungeon_log_list_.resize(10);

	for (int i = 0; i < dungeon_log_list_.size(); ++i) {
		dungeon_log_list_[i] = nullptr;
	}

}

GameManager::~GameManager() {

}

// ゲームマネージャーのアドレスを返す
GameManager* GameManager::GetInstance(std::shared_ptr<SceneBase> start_scene) {

	static GameManager* instance = nullptr;

	if (!instance) {
		instance = new GameManager(start_scene);
	}
	return instance;
}

// アップデート
void GameManager::update(float delta_time) {

	if (now_scene_) now_scene_->update(delta_time);
	if (now_scene_) now_scene_->draw();

	sequence_.update(delta_time);
}

// シーンの切り替えをセット
void GameManager::changeScene(std::shared_ptr<SceneBase> next_scene, float fade_time) {

	if (is_transition_) return;

	next_scene_ = next_scene;
	fade_time_ = fade_time;
	sequence_.change(&GameManager::seqTransOut);
	is_transition_ = true;
}

// ダンジョン記録を追加する（総合スコアがリストより低い場合、追加されない）
void GameManager::addDungeonLog(std::shared_ptr<DungeonLog> dungeon_log) {

	for (int i = 0; i < dungeon_log_list_.size(); ++i) {
		// ダンジョン記録がないとき
		if (!dungeon_log_list_[i]) {
			dungeon_log_list_[i] = dungeon_log;
			break;
		}

		// スコアが高いとき
		if ( dungeon_log_list_[i]->getOverallScore() < dungeon_log->getOverallScore() ) {

			std::shared_ptr<DungeonLog> temp;

			for (int j = dungeon_log_list_.size() - 1; j > i; --j) {
				dungeon_log_list_[j] = dungeon_log_list_[j - 1];
			}
			dungeon_log_list_[i] = dungeon_log;
			break;
		}
	}
}

// ダンジョン記録リストを取得
const std::vector<std::shared_ptr<DungeonLog>>& GameManager::getDungeonLogList() const {
	return dungeon_log_list_;
}


// 何もしない
bool GameManager::seqRun(const float delta_time) {

	return true;
}

// フェードアウト
bool GameManager::seqTransOut(const float delta_time) {
	int alpha = (sequence_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha >= 255) {
		sequence_.change(&GameManager::seqWait);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	return true;
}

// 画面をフェード画像で塗りつぶし、シーンを切り替える
bool GameManager::seqWait(const float delta_time) {

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (sequence_.getProgressTime() <= fade_time_wait_) return true;

	now_scene_ = nullptr;
	now_scene_ = next_scene_;

	sequence_.change(&GameManager::seqTransIn);

	return true;
}

// フェードイン
bool GameManager::seqTransIn(const float delta_time) {
	int alpha = 255 - (sequence_.getProgressTime() / fade_time_ * 255.0f);
	if (alpha <= 0) {
		sequence_.change(&GameManager::seqRun);
		is_transition_ = false;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawExtendGraph(0, 0, DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT, transition_gpc_hdl_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	return true;
}