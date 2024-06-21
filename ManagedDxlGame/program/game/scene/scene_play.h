#pragma once
#include <queue>
#include "../base/scene_base.h"
#include "../dungeon/dungeon_manager.h"
#include "../object/character/player.h"

class DungeonManager;
class DungeonLog;
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

	void update(float delta_time) override;
	void draw() override;

private:
	// シーン内のシーケンス
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqSceneStart);
	
	// ダンジョン中のターン制御シーケンス
	tnl::Sequence<ScenePlay> dungeon_sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqPlayerAct);

	//// メニュー画面の制御シーケンスクラス
	//tnl::Sequence<ScenePlay> menu_sequence_;

	// ---------------- シーンプレイ内で使用する各クラス -------------------
	// カメラクラス
	std::shared_ptr<Camera> camera_;
	// プレイヤー
	std::shared_ptr<Player> player_;
	// ダンジョンの生成処理を行うクラス
	std::shared_ptr<DungeonManager> dungeon_mgr_;
	// ダンジョンでの出来事を記録するクラス
	std::shared_ptr<DungeonLog> dungeon_log_;
	// ダンジョンに生成される敵の生成、行動制御などを管理するクラス
	std::shared_ptr<EnemyManager> enemy_mgr_ ;
	// シーンプレイの表示されるUIの管理をするクラス
	std::shared_ptr<UI_Manager> ui_mgr_;

	// -------------------------- 戦闘関連 ---------------------------------
	// 攻撃するキャラクター
	std::queue< std::shared_ptr<Character> > attackers_;

	// 攻撃目標
	std::queue < std::shared_ptr<Character> > atk_targets_;

	// レベルアップするキャラクター
	std::shared_ptr<Character> level_up_character_;

	// ------------------------- ダンジョン情報関連 ------------------------
	// 各エリアの情報（ 始点座標、サイズなど ）
	std::vector<Area> areas_;
	// 地形データ
	std::vector< std::vector<DungeonCell> > field_;
	// ミニマップデータ
	std::vector< std::vector<MiniMapCell> > mini_map_;
	// 現在の階数
	int dungeon_floor_;

	// マップチップのCSVデータ
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;
	// マップチップの画像格納
	std::vector<int> mapchip_gpc_hdl_;
	
	// ダンジョンを生成したか
	bool is_created_dungeon_;
	// ダンジョン名を表示しているか
	bool is_drawing_dng_title_;
	// ゲームクリアか判定
	bool is_game_clear_;
	
	// ミニマップの表示位置
	tnl::Vector2i mini_map_pos_;
	// ミニマップを表示しているか判定
	bool is_display_mini_map_;
	// ミニマップの表示サイズ
	int mini_map_size_;
	// ミニマップの画像
	int mini_map_cell_gpc_hdl_;
	// メニューを開いているか判定
	bool is_opened_menu_;

	// 説明を消しているか
	bool is_hide_explanation_;

	// --------------------------- フェード演出関連 -------------------------
	// フェード演出の画像
	int fade_gpc_hdl_;
	// 透明度
	int alpha_;
	// フェード演出時間
	float fade_time_;

	// --------------------------- BGM, SE関連 ------------------------------
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
	// キャンセルSEのパス
	std::string cancel_se_hdl_path_;

public:
	// ==============================================
	// プロトタイプ宣言
	// ==============================================
	// 敵の行動内容を修正する
	void modifyEnemyAction();
	// 指定した位置の敵を返す。存在しない場合、"nullptr" で返す
	std::shared_ptr<EnemyBase> findEnemy(const tnl::Vector3& pos);
	// 攻撃を行うキャラクターを追加する
	void addAttacker(std::shared_ptr<Character> attacker);
	// 攻撃対象を追加
	void addAttackTarget(std::shared_ptr<Character> target);
	// 指定した対象にダメージを与える
	void applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target, int damage);
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
	// ミニマップのマスを開く
	void openMiniMapCell();
	// ミニマップを更新
	void updateMiniMap();
	// ミニマップを描画する
	void drawMiniMap();
	// キャラクターアップデート
	void charaUpdate(float delta_time);
	// メニュー画面を閉じる
	void closeMainMenu();
	// キャラクターを倒したときの処理
	void defeatCharacter(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target);
	// 攻撃キャラを切り替える。いなければ、"dungeon_sequence_"を"seqCharaMove" に変更。
	void changeAttacker();
	// レベルアップ処理をする
	void executeLevelUpProcess(std::shared_ptr<Character> chara);
	// 魔法が使えるか確認する
	void checkToUseMagic();
	// 次のフロアに変える処理を行う
	void changeProcessNextFloor();
	// ダンジョンログの書き込みを行う
	void writeDungeonLog();
	// ゲームオーバー処理を実行する
	void executeGameOverProcess();
	// ゲームクリアの処理を実行する
	void executeGameClearProcess();
	// プレイヤーがいるセルを確認する
	bool checkPlayerCell();



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
	bool seqPlayerAction(const float delta_time);
	bool seqCharacterAttack(const float delta_time);
	bool seqTargetDamaged(const float delta_time);
	bool seqCharaLevelUp(const float delta_time);
	bool seqCharaMove(const float delta_time);
	bool seqActEndProcess(const float delta_time);
	bool seqStairSelect(const float delta_time);
	bool seqDrawGameOverMessage(const float delta_time);
	bool seqGameOver(const float delta_time);
	bool seqSelectMainMenu(const float delta_time);
	bool seqSelectMagicList(const float delta_time);
	bool seqSelectToUseMagic(const float delta_time);
	bool seqDrawStatusWindow(const float delta_time);

};