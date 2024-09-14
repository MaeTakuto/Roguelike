#pragma once
#include "../common/chara_status.h"

class MessageWindow;
class CharaStatus;

// =====================================================================================
// ダンジョン探索終了時のダンジョンでの記録を保存し、表示するクラス
// =====================================================================================
class DungeonLog {
public:
	DungeonLog();
	~DungeonLog();

	// "DungeonLog"クラスの描画をする
	void draw();

	// -------------------- ゲッター -----------------------------

	// 総合スコアを取得する
	inline int getOverallScore() const { return overall_score_; }
	// ウィンドウの位置を取得
	const tnl::Vector2i& getWindowPos() const;
	// ウィンドウサイズを取得
	const tnl::Vector2i& getWindowSize() const;

	// -------------------- セッター -----------------------------

	// ダンジョン記録ウィンドウの位置を設定
	void setWindowPos(const tnl::Vector2i& pos);
	// 描画するか設定する
	void setDrawing(bool is_drawing);
	// ダンジョンクリアのフラグをセットする
	inline void setDungeonClear(bool is_clear) { is_dungeon_clear_ = is_clear; }
	// 終了時のレベルをセットする
	inline void setEndFloor(int floor) { end_floor_ = floor; };
	// 終了時のメッセージをセットする。
	inline void setEndMessage(std::string message) { end_message_ = message; }
	// 終了時のレベルをセットする
	void setEndStatus(const CharaStatus& status);

	// -------------------- その他関数 ---------------------------

	// 敵の撃退数を加算する
	inline void additionRepellingEnemy() { ++repelling_enemy_count_; }

	// ダンジョン記録のデータ更新する。
	void updateDungeonLogData();

private:
	// ダンジョン記録ウィンドウ
	std::shared_ptr<MessageWindow> dungeon_log_window_;

	// 終了時レベル
	CharaStatus end_status_;
	// 終了時のメッセージ
	std::string end_message_;
	// ダンジョン記録のメッセージ一覧
	std::vector<std::string> log_messages_;

	// 描画しているか判定
	bool is_drawing_;
	// ダンジョンをクリアしたか判定
	bool is_dungeon_clear_;

	// 到達フロア
	int end_floor_;
	// 敵の撃退数
	int repelling_enemy_count_;
	// 総合スコア
	int overall_score_;

private:
	// 総合スコアを計算する
	void calculateOverallScore();

};