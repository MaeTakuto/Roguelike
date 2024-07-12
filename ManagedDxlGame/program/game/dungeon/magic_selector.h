#pragma once

class MagicBase;
class MessageWindow;
class UI_Manager;
class Character;
class Camera;
class Animation;

// =====================================================================================
// 覚えられる魔法をランダムに選びし、選択した魔法を習得できるクラス
// 
// 選択できる魔法は、"magic_selector.cpp"内にある"SELECT_MAGIC_MAX"の数から設定します。
// =====================================================================================
class MagicSelector {
public:
	MagicSelector();
	~MagicSelector();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

private:
	// マジックセレクターのシーケンス
	tnl::Sequence<MagicSelector> sequence_;

	// UI_Managerのポインタ取得用
	std::weak_ptr<UI_Manager> ui_manager_;

	// 魔法を習得するキャラクター
	std::weak_ptr<Character> learning_character_;

	// 魔法のリスト
	std::vector< std::shared_ptr<MagicBase> > magic_list_;

	// 選択できる魔法リスト
	std::vector< std::shared_ptr<MagicBase> > select_magic_list_;

	// 魔法の説明ウィンドウ
	std::vector<std::shared_ptr<MessageWindow> > magic_explanation_window_;

	// 魔法説明ウィンドウの位置
	std::vector<tnl::Vector2i> window_pos_;

	// カーソルアニメーション
	std::shared_ptr<Animation> cursor_anim_;

	// 選択結果を表示
	std::string selected_result_log_;

	// 選択終了したか判定
	bool is_select_end_;

	// 描画するか判定
	bool is_drawing_;

	// 魔法を学ぶが判定
	bool is_learn_magic_;

	// 選択中の魔法ウィンドウ
	int selected_magic_window_;

public:
	// 選択終了したか判定
	inline bool isSelectEnd() const { 
		return is_select_end_; 
	}

	// 魔法を学んだか判定
	inline bool isLearnMagic() const {
		return is_learn_magic_;
	}

	// UI_Managerをセット
	inline void setUI_Manager(std::shared_ptr<UI_Manager> ui_manager) { 
		ui_manager_ = ui_manager; 
	}

	// 魔法を覚える"Character"クラスを設定する
	inline void setLearningCharacter(std::shared_ptr<Character> learning_character) {
		learning_character_ = learning_character;
	}

	// 魔法選択を実行
	void beginMagicSelect();

private:

	// 覚えられる魔法をランダムに決定する
	void getRandomMagic();
	// 魔法選択を終了する
	void endToSelectMagic();
	// 魔法のレベルを上げる
	void levelUpMagic(std::shared_ptr<MagicBase> magic);

	// メッセージを表示するシーケンス
	bool seqDisplayMessage(const float delta_time);
	// 覚える魔法を選択するシーケンス
	bool seqMagicSelect(const float delta_time);
	// 「はい」か「いいえ」を選択するシーケンス
	bool seqSelectYesOrNo(const float delta_time);
	// 魔法を強化、習得するシーケンス
	bool seqLearnMagic(const float delta_time);

};