#pragma once
#include <queue>
#include "../base/scene_base.h"
#include "../dungeon/dungeon_manager.h"
#include "../object/character/player.h"

class DungeonManager;
class Camera;
class Player;
class EnemyBase;
class EnemyManager;
class UI_Manager;

// プレイシーンクラス
class ScenePlay final : public SceneBase {
public:
	ScenePlay();
	~ScenePlay();

	inline bool isTransitionProcess() override { return is_transition_process_; }

	void update(float delta_time) override;
	void draw() override;

private:
	// シーン内のシーケンス
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqSceneStart);
	
	// ダンジョン中のターン制御シーケンス
	tnl::Sequence<ScenePlay> in_dungeon_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqPlayerAct);

	// シーンプレイ内で使用する各クラス
	std::shared_ptr<Camera> camera_ = nullptr;
	std::shared_ptr<Player> player_ = nullptr;
	std::shared_ptr<DungeonManager> dungeon_mgr_ = nullptr;
	std::shared_ptr<EnemyManager> enemy_mgr_ = nullptr;
	std::shared_ptr<UI_Manager> ui_mgr_ = nullptr;

	// 攻撃する敵
	std::queue< std::shared_ptr<EnemyBase> > atk_enemies_;

	// ------------------------- ダンジョン情報関連 ------------------------------------
	// 各エリアの情報（ 始点座標、サイズなど ）
	std::vector<Area> areas_;
	// 地形データ
	std::vector< std::vector<Cell> > field_;
	// 現在の階数
	int dungeon_floor_ = 1;
	// マップチップのCSVデータ
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;
	// マップチップの画像格納
	std::vector<int> mapchip_gpc_hdl_;
	// ダンジョンを生成したか
	bool is_created_dungeon_ = false;
	// ダンジョンを表示しているか
	bool is_drawing_dng_title_ = true;

	// --------------- フェード演出関連 --------------------
	// フェード演出の画像
	int fade_gpc_hdl_ = 0;
	// 透明度
	int alpha_ = 0;
	// フェード演出時間
	float fade_time_ = 0.5f;

	// ------------------ BGM, SE関連 ----------------------
	// ダンジョンのBGMパス
	std::string dungeon_bgm_hdl_path_;
	// ダンジョンのBGM
	int dungeon_bgm_hdl_;
	// BGM 終了地点のサンプリング周波数
	int bgm_end_freqency_ = 2105775;
	// ダメージSE
	std::string damage_se_hdl_path_;
	// 選択ウィンドウを開くときのSEパス
	std::string open_select_window_se_hdl_path_;
	// レベルアップSE
	std::string level_up_se_hdl_path_;
	// コマンド決定SEのパス
	std::string button_enter_se_hdl_path_;

public:
	// ==============================================
	// プロトタイプ宣言
	// ==============================================
	// 指定した位置の敵を返す。存在しない場合、"nullptr" で返す
	std::shared_ptr<EnemyBase> findEnemy(const tnl::Vector3& pos);
	// 指定した対象にダメージを与える
	void applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target);
	// レベルアップ処理をする
	void charaLevelUpProcess(std::shared_ptr<Character> chara);
	// 次のフロアに変える処理を行う
	void changeProcessNextFloor();
	// ゲームオーバー処理を実行
	void executeGameOverProcess();
	// メッセージウィンドウにメッセージをセット
	void setMessage(const std::string& message);

	// ==============================================
	//				インライン関数
	// ==============================================

	// 現在のフロアを取得
	inline int getFloor() const { return dungeon_floor_; }

	// 指定された座標のマップデータを返す
	inline eMapData getMapData(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return eMapData::NONE;
		if (x < 0 || x >= field_[y].size()) return eMapData::NONE;

		return field_[y][x].map_data;
	}

	// マップデータをセット
	inline void setMapData(const tnl::Vector3& pos, eMapData map_data) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		field_[y][x].map_data = map_data;
	}

	// 地形データを取得する
	inline eMapData getTerrainData(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].terrain_data;
	}

	// 部屋か通路かを返す。
	inline ePlace getPlace(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].place;
	}

	// エリア番号を返す
	inline int getAreaId(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return -1;
		if (x < 0 || x >= field_[y].size()) return -1;

		return field_[y][x].area_id;
	}

	// 部屋の入口の座標を返す
	inline const std::vector<Entrance>& getRoomEntrance(int area_id) const {
		return areas_[area_id].room.entrance;
	}

	// プレイヤーを取得
	inline const std::shared_ptr<Player> getPlayer() const { return player_; }

private:
	//// ========= デバッグ ============

	////  
	//void debugMapData() {

	//	tnl::DebugTrace("============== map_data_ ===============\n");

	//	for (int y = 0; y < field_.size(); y++) {
	//		for (int x = 0; x < field_[y].size(); x++) {
	//			tnl::DebugTrace("%d", static_cast<int>(field_[y][x].map_data));
	//		}
	//		tnl::DebugTrace("\n");
	//	}
	//}

	//void debugPlaceData() {

	//	tnl::DebugTrace("============== field_.place ===============\n");

	//	for (int y = 0; y < field_.size(); y++) {
	//		for (int x = 0; x < field_[y].size(); x++) {
	//			tnl::DebugTrace("%d", static_cast<int>(field_[y][x].place));
	//		}
	//		tnl::DebugTrace("\n");
	//	}
	//}

	// キャラクターアップデート
	void charaUpdate(float delta_time);
	// キャラクターを倒したときの処理
	void defeatCharacter(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target);

	// =========== メインシーケンス ===========
	bool seqSceneStart(const float delta_time);
	bool seqFadeIn(const float delta_time);
	bool seqGenerateDungeon(const float delta_time);
	bool seqDrawDungeonTitle(const float delta_time);
	bool seqFadeOut(const float delta_time);
	bool seqMain(const float delta_time);

	// =========== ダンジョン探索中のシーケンス ===========
	bool seqPlayerAct(const float delta_time);
	bool seqEnemyAct(const float delta_time);
	bool seqPlayerMove(const float delta_time);
	bool seqPlayerAttack(const float delta_time);
	bool seqEnemyAttack(const float delta_time);
	bool seqCharaMove(const float delta_time);
	bool seqActEndProcess(const float delta_time);
	bool seqStairSelect(const float delta_time);
	bool seqLevelUp(const float delta_time);
	bool seqGameOver(const float delta_time);

};