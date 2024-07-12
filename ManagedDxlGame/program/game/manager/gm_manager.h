#pragma once

class SceneBase;
class DungeonLog;

using CsvData = std::vector< std::vector<tnl::CsvCell> >;

// 各方向のベクトル
const tnl::Vector3 DIR_POS[8]
	= { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

class GameManager {
public:
	// デストラクタ
	~GameManager();

	// ====== const ======
	// 画像チップサイズ
	static constexpr int CHIP_SIZE = 32;
	// オブジェクトの表示サイズ
	static constexpr int DRAW_CHIP_SIZE = CHIP_SIZE * 2;
	// csv のセルの読み取りの最初の行
	static constexpr int CSV_CELL_ROW_START = 1;

	// ゲームフォントの通常サイズ
	static constexpr int GM_DEFAULT_FONT_SIZE = 100;

	// ダンジョンのサイズ
	static constexpr int FIELD_WIDTH = 64;
	static constexpr int FIELD_HEIGHT = 32;

	// ゲームマネージャーのアドレスを返す
	static GameManager* GetInstance(std::shared_ptr<SceneBase> = nullptr);

	// アドレスの削除
	inline static void Destroy() { delete GetInstance(); }

	// ゲームマネージャーの更新
	void update(float delta_time);

	// シーンの切り替えを行う
	void changeScene(std::shared_ptr<SceneBase> next_scene, float fade_time = 1.0f);

	// シーンのインスタンスを返す。
	inline const std::shared_ptr<SceneBase> getSceneInstance() const { return now_scene_; }	

	// フェード演出フラグを返す。
	inline bool isTransition() const { return is_transition_; }

	// ゲームをクリアしたか判定を返す
	inline bool isGameClear() const { return is_game_clear_; }

	// フォントハンドルを返す。
	inline int getDefaultFontHdl() const { return default_font_hdl_; }

	// "is_game_clear_" の値を設定する
	inline void setGameClear(bool is_game_clear) { is_game_clear_ = is_game_clear; }

	// ダンジョン記録を追加する（総合スコアがリストより低い場合、追加されない）
	void addDungeonLog(std::shared_ptr<DungeonLog> dungeon_log);
	// ダンジョン記録のリストを取得
	const std::vector< std::shared_ptr<DungeonLog> >& getDungeonLogList() const;

private:
	// コンストラクタ
	GameManager(std::shared_ptr<SceneBase> start_scene);

	// 現在のシーン
	std::shared_ptr<SceneBase> now_scene_;
	// 次のシーン
	std::shared_ptr<SceneBase> next_scene_;

	// ダンジョン記録リスト
	std::vector< std::shared_ptr<DungeonLog> > dungeon_log_list_;

	// 通常のフォント
	int default_font_hdl_;

	// -------------------- フェード演出関連 -----------------------------

	// フェード演出画像パス
	std::string transition_gpc_hdl_path_;

	// フェード演出画像
	int transition_gpc_hdl_;

	// フェード演出時間
	float fade_time_;
	// フェード演出待機時間
	float fade_time_wait_;
	// フェード演出フラグ
	bool is_transition_;

	// --------------------------------------------------------------------

	// ゲームをクリアしたか判定
	bool is_game_clear_;

	// ゲームマネージャーのシーケンス
	tnl::Sequence<GameManager> sequence_;

	// 各シーケンス
	bool seqRun(const float delta_time);									// 何もしない
	bool seqTransOut(const float delta_time);								// フェードアウト
	bool seqWait(const float delta_time);									// フェード演出待機
	bool seqTransIn(const float delta_time);								// フェードイン

};