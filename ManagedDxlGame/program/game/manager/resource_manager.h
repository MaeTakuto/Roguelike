#pragma once
#include "gm_manager.h"

// =====================================================================================
// 画像、アニメーション、サウンドのパスを管理するクラス（ Flyweight パターン ）
// =====================================================================================
class ResourceManager final {
public:
	// デストラクタ
	~ResourceManager();

	// インスタンスを返す
	static ResourceManager* getInstance();

	// インスタンスを削除
	static void destroy();

	// ============================================================
	// 画像、アニメーション関連 
	// ============================================================
	// 画像のロード、画像のアドレスを渡す。
	int loadGraph(const std::string& gpc_hdl_path);

	// 画像の削除
	void deleteGraph(const std::string& gpc_hdl_path);

	// アニメーションのロード
	std::vector<int>& loadAnimation(const std::string& anim_hdl_path,
		int all_size, int x_size, int y_size, int anim_hdl_width, int anim_hdl_height);

	// アニメーションの削除
	void deleteAnimation(const std::string& anim_hdl_path, int all_size);

	// ============================================================
	// サウンド関連 
	// ============================================================
	// サウンドのアドレスを渡す。
	int loadSound(const std::string& snd_path);

	// サウンドを再生
	void playSound(const std::string& snd_path, int play_type, int top_position_flag = 1);

	// サウンドが再生されているか判定
	bool checkSound(const std::string& snd_path);

	// サウンドを停止
	void stopSound(const std::string& snd_path);

	// サウンドの削除
	void deleteSound(const std::string& snd_path);

	// ============================================================
	// CSV関連 
	// ============================================================

	// csvを読み込み返す
	CsvData& loadCsvData(const std::string& csv_path);

	// csvデータを削除
	void deleteCsvData(const std::string& csv_path);

	// 全てのデータを削除
	void clearAllResources();

private:
	// コンストラクタ
	ResourceManager();

	// 画像データの格納用コンテナ
	std::unordered_map< std::string, int > gpc_hdl_container_;

	// アニメーションデータの格納用コンテナ
	std::unordered_map< std::string, std::vector<int> >
		anim_hdl_container_;

	// サウンドデータの格納用コンテナ
	std::unordered_map< std::string, int > snd_container_;

	// CSVデータのコンテナ
	std::unordered_map< std::string, CsvData > csv_data_container_;

};