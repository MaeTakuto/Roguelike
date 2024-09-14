#include <random>
#include "../../dxlib_ext/dxlib_ext.h"
//#include <algorithm>
#include "../manager/resource_manager.h"
#include "../manager/ui_manager.h"
#include "../base/character_base.h"
#include "../common/animation.h"
#include "../magic/heal_magic.h"
#include "../magic/fire_magic.h"
#include "../magic/magnetism_magic.h"
#include "../common/magic_status.h"
#include "../ui/message_window.h"
#include "../common/camera.h"
#include "magic_selector.h"

namespace {
	// 魔法の数
	const int MAGIC_MAX = 3;

	// 選択できる魔法の最大値
	const int SELECT_MAGIC_MAX = 2;

	// 魔法選択ウィンドウの位置
	const tnl::Vector2i WINDOW_TOP_POS = { 200, 220 };

	// 魔法選択ウィンドウの間隔
	const int WINDOW_SPACE = 40;

	// 
	const int FONT_SIZE = 25;

}

MagicSelector::MagicSelector() : sequence_(tnl::Sequence<MagicSelector>(this, &MagicSelector::seqDisplayMessage)), 
	magic_selector_explanation_window_(std::make_shared<MessageWindow>()), magic_status_window_(std::make_shared<MessageWindow>()), 
	cursor_anim_(std::make_shared<Animation>()), is_select_end_(false), is_drawing_(false), is_learn_magic_(false), selected_magic_window_(0)
{

	magic_list_.resize(MAGIC_MAX);

	magic_list_[0] = std::make_shared<HealMagic>();
	magic_list_[1] = std::make_shared<FireMagic>();
	magic_list_[2] = std::make_shared<MagnetismMagic>();

	select_magic_list_.resize(SELECT_MAGIC_MAX);

	for (int i = 0; i < select_magic_list_.size(); ++i) {
		select_magic_list_[i] = nullptr;
	}

	magic_explanation_window_.resize(SELECT_MAGIC_MAX);
	window_pos_.resize(SELECT_MAGIC_MAX);

	tnl::Vector2i window_size = tnl::Vector2i(500, 0);

	magic_selector_explanation_window_->setWindowPos(tnl::Vector2i(450, 140));
	magic_selector_explanation_window_->setMessageLine(1);
	magic_selector_explanation_window_->setWindowSize(tnl::Vector2i(350, 0));
	magic_selector_explanation_window_->setMessageTopPos(tnl::Vector2i(10, 10));
	magic_selector_explanation_window_->setFontSize(FONT_SIZE);
	magic_selector_explanation_window_->calculateWindowSize();
	magic_selector_explanation_window_->setMessgae("魔法の習得・強化ができます");
	magic_selector_explanation_window_->setEnable(true);

	for (int i = 0; i < magic_explanation_window_.size(); ++i) {
		magic_explanation_window_[i] = std::make_shared<MessageWindow>();
		magic_explanation_window_[i]->setWindowSize(window_size);
		magic_explanation_window_[i]->setMessageLine(3);
		magic_explanation_window_[i]->setFontSize(FONT_SIZE);
		magic_explanation_window_[i]->setMessageTopPos(tnl::Vector2i(10, 10));
		magic_explanation_window_[i]->calculateWindowSize();

		window_pos_[i] = WINDOW_TOP_POS + tnl::Vector2i(0, (magic_explanation_window_[i]->getWindowSize().y + WINDOW_SPACE) * i);

		magic_explanation_window_[i]->setWindowPos(window_pos_[i]);

		magic_explanation_window_[i]->setDrawGpcHdlPos(magic_explanation_window_[i]->getWindowPos() + window_size - tnl::Vector2i(80, -10));
		magic_explanation_window_[i]->setDrawGpcHdlSize(tnl::Vector2i( 95, 95 ));

		magic_explanation_window_[i]->setEnable(true);
	}

	tnl::Vector2i pos = WINDOW_TOP_POS;
	pos.x += magic_explanation_window_[0]->getWindowSize().x + 50;

	magic_status_window_->setWindowPos(pos);
	magic_status_window_->setWindowSize(tnl::Vector2i(400, 0));
	magic_status_window_->setMessageTopPos(tnl::Vector2i(10, 10));
	magic_status_window_->setFontSize(FONT_SIZE);
	magic_status_window_->setMessageLine(4);
	magic_status_window_->calculateWindowSize();
	magic_status_window_->setEnable(true);

	cursor_anim_->setAnimGraphicHandle(
		ResourceManager::getInstance()->loadAnimation(
			"graphics/cursor.png",
			6,
			6,
			1,
			16,
			30
		)
	);

	cursor_anim_->calculateChipSize();
	cursor_anim_->setFrameChangeInterval(0.1f);
	cursor_anim_->setLoopAnimation(true);
}

MagicSelector::~MagicSelector() {
	tnl::DebugTrace("MagicSelectorのデストラクタが実行されました\n");
}

// =====================================================================================
// アップデート
// =====================================================================================
void MagicSelector::update(float delta_time) {

	sequence_.update(delta_time);

}

// =====================================================================================
// 描画
// =====================================================================================
void MagicSelector::draw(const std::shared_ptr<Camera> camera) {

	if (!is_drawing_) {
		return;
	}

	magic_selector_explanation_window_->draw();

	for (int i = 0; i < magic_explanation_window_.size(); ++i) {
		if (!magic_explanation_window_[i]) {
			continue;
		}
		magic_explanation_window_[i]->draw();
	}

	magic_status_window_->draw();

	tnl::Vector2i draw_pos = window_pos_[selected_magic_window_] + tnl::Vector2i( 0, ( magic_explanation_window_[selected_magic_window_]->getWindowSize().y / 2 )) ;
	draw_pos -= tnl::Vector2i( 50, 0 );
	draw_pos += tnl::Vector2i( static_cast<int>(camera->getPos().x), static_cast<int>(camera->getPos().y) );

	tnl::Vector2i draw_size = cursor_anim_->getAnimChipSize();

	cursor_anim_->setDrawPos(draw_pos);
	cursor_anim_->setDrawSize(draw_size);
	cursor_anim_->draw(camera);

}

// =====================================================================================
// 魔法選択を開始する
// =====================================================================================
void MagicSelector::beginMagicSelect() {
	is_select_end_ = false;
}

// =====================================================================================
// 選択できる魔法をランダムで取得
// =====================================================================================
void MagicSelector::getRandomMagic() {

	auto list = magic_list_;

	std::random_device rd;
	std::mt19937 mt(rd());

	if (list.size() <= 1) {

	}

	std::shuffle(list.begin(), list.end(), mt);

	for (int i = 0; i < select_magic_list_.size(); ++i) {
		select_magic_list_[i] = list[i];
	}

	std::vector < std::string > message;

	for (int i = 0; i < magic_explanation_window_.size(); ++i) {
		message.emplace_back( (select_magic_list_[i]->getMagicStatus()->getMagicName() ));

		auto itr_begin = select_magic_list_[i]->getMagicExplantion().begin() + 3;
		message.insert(message.end(), itr_begin, select_magic_list_[i]->getMagicExplantion().end());
		magic_explanation_window_[i]->setAllLineMessage(message);
		magic_explanation_window_[i]->setGpcHdl(select_magic_list_[i]->getMagicIconGpcHdl());
		message.clear();
	}

}

// =====================================================================================
// 魔法選択を終了する
// =====================================================================================
void MagicSelector::endToSelectMagic() {

	is_select_end_ = true;
	is_drawing_ = false;
	sequence_.change(&MagicSelector::seqDisplayMessage);
}

// =====================================================================================
// 魔法のレベルを上げる
// =====================================================================================
void MagicSelector::levelUpMagic(std::shared_ptr<MagicBase> magic) {

	auto sp_character = learning_character_.lock();

	if (!sp_character) {
		return;
	}


	for (int i = 0; i < magic_list_.size(); ++i) {
		if (magic_list_[i] != magic) {
			continue;
		}

		sp_character->levelUpMagic(magic_list_[i]);
		magic_list_[i]->levelUpMagic();

		if (magic_list_[i]->getMagicStatus()->getNowLevel() < magic_list_[i]->getMagicStatus()->getMaxLevel()) {
			return;
		}

		tnl::DebugTrace("%sはレベルが最大になりました。", magic_list_[i]->getMagicStatus()->getMagicName().c_str());
		magic_list_.erase(magic_list_.begin() + i);
		return;
	}
}

// =======================================================================================
// 魔法ステータスウィンドウの更新
// =======================================================================================
void MagicSelector::updateMagicStatusWindow() {

	auto sp_character = learning_character_.lock();

	std::vector<std::string> text = sp_character->checkMagicList(select_magic_list_[selected_magic_window_]) ?
		select_magic_list_[selected_magic_window_]->getLevelUpStatusComparisonText() : select_magic_list_[selected_magic_window_]->getStatusComparisonText();

	magic_status_window_->clearMessage();
	magic_status_window_->setEnable(true);
	magic_status_window_->setAllLineMessage(text);

}

// =======================================================================================
// レベルアップのメッセージを表示する
// =======================================================================================
bool MagicSelector::seqDisplayMessage(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		ResourceManager::getInstance()->playSound("sound/se/button_enter.mp3", DX_PLAYTYPE_BACK);

		auto sp_ui_manager = ui_manager_.lock();

		if (!sp_ui_manager) {
			return true;
		}

		sp_ui_manager->clearMessage();
		sp_ui_manager->changeCtrlExplanationWindowType(2);
		sequence_.change(&MagicSelector::seqMagicSelect);
		getRandomMagic();
		is_drawing_ = true;
		cursor_anim_->startAnimation();
		updateMagicStatusWindow();
	}

	return true;
}

bool MagicSelector::seqMagicSelect(const float delta_time) {

	cursor_anim_->update(delta_time);

	// 上キー
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		ResourceManager::getInstance()->playSound("sound/se/button_select.mp3", DX_PLAYTYPE_BACK);
		--selected_magic_window_;
		if (selected_magic_window_ < 0) {
			selected_magic_window_ = magic_explanation_window_.size() - 1;
		}
		updateMagicStatusWindow();
	}

	// 下キー
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		ResourceManager::getInstance()->playSound("sound/se/button_select.mp3", DX_PLAYTYPE_BACK);
		++selected_magic_window_;
		if (selected_magic_window_ >= magic_explanation_window_.size()) {
			selected_magic_window_ = 0;
		}
		updateMagicStatusWindow();
	}

	// Enterキー
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		ResourceManager::getInstance()->playSound("sound/se/button_enter.mp3", DX_PLAYTYPE_BACK);

		auto sp_ui_manager = ui_manager_.lock();
		auto sp_character = learning_character_.lock();

		sequence_.change(&MagicSelector::seqSelectYesOrNo);

		if (!sp_ui_manager || !sp_character) {
			return true;
		}

		is_learn_magic_ = true;
		std::string message = "";

		if (sp_character->checkMagicList(select_magic_list_[selected_magic_window_])) {
			message = select_magic_list_[selected_magic_window_]->getMagicStatus()->getMagicName() + "を強化しますか？";
		}
		else {
			message = select_magic_list_[selected_magic_window_]->getMagicStatus()->getMagicName() + "を覚えますか？";
		}

		sp_ui_manager->executeSelectYesOrNoWindow(message);
		cursor_anim_->stopAnimation();

	}

	// BackSapceキーを押したとき
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_BACK)) {

		ResourceManager::getInstance()->playSound("sound/se/cancel.mp3", DX_PLAYTYPE_BACK);
		auto sp_ui_manager = ui_manager_.lock();

		sequence_.change(&MagicSelector::seqSelectYesOrNo);

		if (!sp_ui_manager) {
			return true;
		}

		is_learn_magic_ = false;
		sp_ui_manager->executeSelectYesOrNoWindow("魔法を覚えないで終了しますか？");
		cursor_anim_->stopAnimation();

	}

	return true;
}

bool MagicSelector::seqSelectYesOrNo(const float delta_time) {

	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {

		ResourceManager::getInstance()->playSound("sound/se/button_enter.mp3", DX_PLAYTYPE_BACK);
		auto sp_ui_manager = ui_manager_.lock();

		if (!sp_ui_manager) {
			return true;
		}

		if (sp_ui_manager->getSelectedIndexFromTwoSelectCmd() == 0) {
			is_learn_magic_ ? sequence_.change(&MagicSelector::seqLearnMagic) : endToSelectMagic();
		}
		else {
			sequence_.change(&MagicSelector::seqMagicSelect);
			cursor_anim_->startAnimation();
		}

		sp_ui_manager->finishSelectYesOrNoWindow();
	}

	return true;
}

bool MagicSelector::seqLearnMagic(const float delta_time) {
	
	auto sp_ui_manager = ui_manager_.lock();
	auto sp_character = learning_character_.lock();

	if (!sp_ui_manager || !sp_character) {
		return true;
	}

	if (sp_character->checkMagicList(select_magic_list_[selected_magic_window_])) {
		levelUpMagic(select_magic_list_[selected_magic_window_]);
		sp_ui_manager->setMessage(select_magic_list_[selected_magic_window_]->getMagicStatus()->getMagicName() + "のレベルが上がった", sp_ui_manager->getDrawMessageWindowTime());
	}
	else {
		sp_character->addMagic(select_magic_list_[selected_magic_window_]->createClone());
		sp_ui_manager->setMessage(select_magic_list_[selected_magic_window_]->getMagicStatus()->getMagicName() + "を覚えた", sp_ui_manager->getDrawMessageWindowTime());
	}

	endToSelectMagic();

	return false;
}
