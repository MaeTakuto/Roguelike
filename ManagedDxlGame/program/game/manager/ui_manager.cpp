#include "../..//dxlib_ext/dxlib_ext.h"
#include "../common/enum.h"
#include "../ui/message_window.h"
#include "../ui/select_window.h"
#include "../ui/hp_bar.h"
#include "../common/chara_status.h"
#include "../base/character_base.h"
#include "../base/magic_base.h"
#include "ui_manager.h"

namespace {
	// メッセージウィンドウの通常の位置、サイズ
	const tnl::Vector2i	DEFAULT_MESS_WINDOW_POS = { 175, 500 };
	const tnl::Vector2i DEFAULT_MESS_WINDOW_SIZE = { 900, 200 };

	// 階段での選択時のメッセージウィンドウの位置、サイズ、メッセージ
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_POS = { 150, 450 };
	const tnl::Vector2i STAIR_SEL_MESS_WINDOW_SIZE = { 750, 200 };
	const std::string STAIR_SEL_MESSAGE = "穴がある。落ちますか？";

	// 階数の表示位置
	const tnl::Vector2i FLOOR_STR_POS = { 1000, 55 };
	const int FLOOR_STR_FONT_SIZE = 40;

	// 表示するコマンド名のフォントサイズ
	const int SELECT_WINDOW_CMD_FONT_SIZE = 30;
	// コマンドと次のコマンドとの表示間隔
	const int SELECT_WINDOW_CMD_SPACE = SELECT_WINDOW_CMD_FONT_SIZE + 5;

	// ================= 2択選択ウィンドウ（はい、いいえ）の設定 =================
	// 選択ウィンドウの表示位置
	const tnl::Vector2i TWO_SELECT_WINDOW_POS = { 950, 450 };
	// 選択ウィンドウの表示サイズ
	const tnl::Vector2i TWO_SELECT_WINDOW_SIZE = { 150, 70 };

	// ================= メインメニューの設定 ====================================
	// 選択ウィンドウの表示位置
	const tnl::Vector2i MAIN_MENU_WINDOW_POS = { 100, 100 };
	// 選択ウィンドウの表示位置
	const tnl::Vector2i MAIN_MENU_WINDOW_SIZE = { 300, 70 };

	// ================= サブメニューの設定 ======================================
	// 
	const tnl::Vector2i SUB_WINDOW_POS = tnl::Vector2i( MAIN_MENU_WINDOW_POS.x + MAIN_MENU_WINDOW_SIZE.x, MAIN_MENU_WINDOW_POS.y ) + tnl::Vector2i(50, 0);
	// 
	const tnl::Vector2i MAGIC_WINDOW_SIZE = { 300, 70 };

}

// =====================================================================================
// コンストラクタ
// =====================================================================================
UI_Manager::UI_Manager() : message_window_(std::make_shared<MessageWindow>()), view_status_window_(std::make_shared<MessageWindow>()),
	two_select_window_(std::make_shared<SelectWindow>()), main_menu_select_window_(std::make_shared<SelectWindow>()), magic_select_window_(std::make_shared<SelectWindow>()),
	hp_bar_(std::make_shared<HP_Bar>()), floor_(0)
{
	// ------------------------- メッセージウィンドウの初期化 -------------------------
	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize(DEFAULT_MESS_WINDOW_SIZE);

	// ------------------------- ステータス確認ウィンドウの初期化 ---------------------
	view_status_window_->setWindowPos( SUB_WINDOW_POS );
	view_status_window_->setWindowSize(tnl::Vector2i(300, 230));
	view_status_window_->setMessageLine(5);

	// ------------------------- 2択選択ウィンドウの初期化 ----------------------------
	std::vector<std::string> select_cmd_names( static_cast<size_t>( eTwoSelectCmd::MAX ));
	
	select_cmd_names[0] = "はい";
	select_cmd_names[1] = "いいえ";
	
	two_select_window_->setWindowPos( TWO_SELECT_WINDOW_POS );
	two_select_window_->setWindowSize( tnl::Vector2i( TWO_SELECT_WINDOW_SIZE.x, TWO_SELECT_WINDOW_SIZE.y + SELECT_WINDOW_CMD_FONT_SIZE * ( select_cmd_names.size() - 1) ) );
	two_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	two_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );
	two_select_window_->setSelectCmdMax( std::underlying_type<eTwoSelectCmd>::type( eTwoSelectCmd::MAX ) );
	two_select_window_->setCommandNames( select_cmd_names );

	// ------------------------- メインメニューウィンドウの初期化 ----------------------
	select_cmd_names.resize( std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::MAX) );

	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::MAGIC_SELECT) ]			= "魔法";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_CELL) ]		= "足元を確認";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CHECK_STATUS) ]	= "ステータスを表示";
	select_cmd_names[ std::underlying_type<eMainMenuCmd>::type(eMainMenuCmd::CLOSE) ]			= "とじる";
	
	main_menu_select_window_->setWindowPos( MAIN_MENU_WINDOW_POS );
	main_menu_select_window_->setWindowSize( tnl::Vector2i( MAIN_MENU_WINDOW_SIZE.x, MAIN_MENU_WINDOW_SIZE.y + SELECT_WINDOW_CMD_SPACE * ( select_cmd_names.size() - 1 ) ) );
	main_menu_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	main_menu_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );
	main_menu_select_window_->setSelectCmdMax( std::underlying_type<eMainMenuCmd>::type( eMainMenuCmd::MAX ) );
	main_menu_select_window_->setCommandNames( select_cmd_names );

	// ------------------------- 魔法一覧の選択ウィンドウの初期化 ----------------------
	magic_select_window_->setWindowPos( SUB_WINDOW_POS );
	magic_select_window_->setWindowSize( MAGIC_WINDOW_SIZE );
	magic_select_window_->setFontSize( SELECT_WINDOW_CMD_FONT_SIZE );
	magic_select_window_->setMessageSpace( SELECT_WINDOW_CMD_SPACE );

}

// =====================================================================================
// デストラクタ
// =====================================================================================
UI_Manager::~UI_Manager() {

	tnl::DebugTrace("UI_Managerのデストラクタが実行されました\n");
}

// =====================================================================================
// アップデート
// =====================================================================================
void UI_Manager::update(float delta_time) {

	if (message_window_->isEnable()) message_window_->update(delta_time);
	if (two_select_window_->isOperate()) two_select_window_->update(delta_time);
	if(main_menu_select_window_->isOperate()) main_menu_select_window_->update(delta_time);
	if (magic_select_window_->isOperate()) magic_select_window_->update(delta_time);
	if(view_status_window_->isEnable()) view_status_window_->update(delta_time);
}

// =====================================================================================
// 描画
// =====================================================================================
void UI_Manager::draw(const std::shared_ptr<Camera> camera) {

	if (message_window_->isEnable()) message_window_->draw(camera);
	if (two_select_window_->isDrawing()) two_select_window_->draw(camera);
	if (main_menu_select_window_->isDrawing()) main_menu_select_window_->draw(camera);
	if (magic_select_window_->isDrawing()) magic_select_window_->draw(camera);
	if (view_status_window_->isEnable()) view_status_window_->draw(camera);
	hp_bar_->draw(camera);
	SetFontSize(FLOOR_STR_FONT_SIZE);
	DrawStringEx(FLOOR_STR_POS.x, FLOOR_STR_POS.y, -1, "%dF", floor_);
}

// =====================================================================================
// メインメニューの現在選択中のインデックスを返す
// =====================================================================================
int UI_Manager::getSelectedMainMenuCmdIndex() { 
	return main_menu_select_window_->getSelectedCmdIndex();
}

// =====================================================================================
// メインメニューを開く
// =====================================================================================
void UI_Manager::openMainMenu() {
	main_menu_select_window_->setDrawing(true);
	main_menu_select_window_->setOperate(true);
}

// =====================================================================================
// メインメニューを閉じる
// =====================================================================================
void UI_Manager::closeMainMenu() {
	main_menu_select_window_->setDrawing(false);
	main_menu_select_window_->setOperate(false);
}

// =====================================================================================
// 魔法選択ウィンドウの一覧を更新
// =====================================================================================
void UI_Manager::updateMagicList() {

	std::shared_ptr<Character> target = ui_target_.lock();
	if (!target) {
		return;
	}

	auto& magic_list = target->getMagicList();

	std::vector<std::string> select_cmd_names(magic_list.size());

	for (int i = 0; i < magic_list.size(); ++i) {
		select_cmd_names[i] = magic_list[i]->getMagicName();
	}

	magic_select_window_->setWindowSize(tnl::Vector2i( MAGIC_WINDOW_SIZE.x, MAGIC_WINDOW_SIZE.y + SELECT_WINDOW_CMD_SPACE * ( select_cmd_names.size() - 1 ) ) );
	magic_select_window_->setSelectCmdMax(select_cmd_names.size());
	magic_select_window_->setCommandNames(select_cmd_names);

}

// =====================================================================================
// メインメニューの現在選択中のインデックスを返す
// =====================================================================================
int UI_Manager::getSelectedMagicListCmdIndex() {
	return magic_select_window_->getSelectedCmdIndex();
}

// =====================================================================================
// 魔法選択ウィンドウの一覧を更新
// =====================================================================================
void UI_Manager::openMagicListWindow() {
	updateMagicList();
	magic_select_window_->setDrawing(true);
	magic_select_window_->setOperate(true);
	main_menu_select_window_->setOperate(false);
}

// =====================================================================================
// 魔法選択ウィンドウの一覧を更新
// =====================================================================================
void UI_Manager::closeMagicListWindow() {
	magic_select_window_->setDrawing(false);
	magic_select_window_->setOperate(false);
	main_menu_select_window_->setOperate(true);
}

// =====================================================================================
// ステータスウィンドウを表示する
// =====================================================================================
void UI_Manager::displayStatusWindow() {
	view_status_window_->clearMessage();

	// 現在ステータスの設定
	std::shared_ptr<Character> target = ui_target_.lock();
	if (target) {

		const CharaStatus& status = target->getStatus();
		view_status_window_->setMessgae( "レベル：" + std::to_string( status.getLevel() ) );
		view_status_window_->setMessgae( "HP    ：" + std::to_string( status.getHP()) + " / " + std::to_string(status.getMaxHP()));
		view_status_window_->setMessgae( "MP    ：" + std::to_string( status.getMP()) + " / " + std::to_string(status.getMaxMP()));
		view_status_window_->setMessgae( "攻撃力：" + std::to_string( status.getAtk() ) );
		view_status_window_->setMessgae( "経験値：" + std::to_string( status.getExp() ) );
	}

	view_status_window_->setEnable(true);
	main_menu_select_window_->setOperate(false);
}

// =====================================================================================
// ステータスウィンドウを非表示にする
// =====================================================================================
void UI_Manager::hideStatusWindow() {
	view_status_window_->setEnable(false);
	main_menu_select_window_->setOperate(true);
}

// =====================================================================================
// 階段選択の現在選択中のインデックスを返す
// =====================================================================================
int UI_Manager::getSelectedStairSelectCmdIndex() { 
	return two_select_window_->getSelectedCmdIndex(); 
}


// =====================================================================================
// 階段での選択処理の実行
// =====================================================================================
void UI_Manager::executeStairSelect() {
	// 二択の選択ウィンドウの有効にする
	two_select_window_->setDrawing(true);
	two_select_window_->setOperate(true);

	// メッセージウィンドウ関連の処理
	message_window_->cancelTimeLimit();
	message_window_->clearMessage();
	message_window_->setWindowPos(STAIR_SEL_MESS_WINDOW_POS);
	message_window_->setWindowSize(STAIR_SEL_MESS_WINDOW_SIZE);
	message_window_->setMessgae(STAIR_SEL_MESSAGE);
	message_window_->setEnable(true);
}

// =====================================================================================
// 階段での選択終了の処理の実行
// =====================================================================================
void UI_Manager::executeStairSelectEnd() {
	// メッセージウィンドウ関連の処理
	two_select_window_->setDrawing(false);
	two_select_window_->setOperate(false);
	
	// メッセージウィンドウ関連の処理
	message_window_->setWindowPos(DEFAULT_MESS_WINDOW_POS);
	message_window_->setWindowSize(DEFAULT_MESS_WINDOW_SIZE);
	message_window_->clearMessage();
	message_window_->setEnable(false);
}

// =====================================================================================
// メッセージウィンドウにメッセージをセット
// =====================================================================================
void UI_Manager::setMessage(const std::string& message, float draw_time) {

	// 表示時間を制限する場合
	if (draw_time > 0.0f) {
		message_window_->setMessgae(message);
		message_window_->setTimeLimit(draw_time);
		return;
	}
	// 通常の場合
	message_window_->setMessgae(message);
	message_window_->setEnable(true);
}

// =====================================================================================
// メッセージウィンドウのメッセージを全削除する
// =====================================================================================
void UI_Manager::clearMessage() {
	message_window_->clearMessage();
}

// =====================================================================================
// HPバーにステータスをセット
// =====================================================================================
void UI_Manager::setHP_BarStatus() {

	std::shared_ptr<Character> target = ui_target_.lock();
	
	if (!target) {
		tnl::DebugTrace("targetの参照が無効です\n");
		return;
	}
		
	const CharaStatus& status = target->getStatus();

	hp_bar_->setMaxHP(status.getMaxHP());
	hp_bar_->setHP(status.getHP());
	hp_bar_->updateHP_Text();
}
