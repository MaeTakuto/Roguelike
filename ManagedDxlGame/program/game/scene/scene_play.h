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

// �v���C�V�[���N���X
class ScenePlay final : public SceneBase {
public:
	ScenePlay();
	~ScenePlay();

	inline bool isTransitionProcess() override { return is_transition_process_; }

	void update(float delta_time) override;
	void draw() override;

private:
	// �V�[�����̃V�[�P���X
	tnl::Sequence<ScenePlay> main_seq_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqSceneStart);
	
	// �_���W�������̃^�[������V�[�P���X
	tnl::Sequence<ScenePlay> dungeon_sequence_ = tnl::Sequence<ScenePlay>(this, &ScenePlay::seqPlayerAct);

	//// ���j���[��ʂ̐���V�[�P���X�N���X
	//tnl::Sequence<ScenePlay> menu_sequence_;

	// ---------------- �V�[���v���C���Ŏg�p����e�N���X -------------------
	// �J�����N���X
	std::shared_ptr<Camera> camera_ = nullptr;
	// �v���C���[
	std::shared_ptr<Player> player_ = nullptr;
	// �_���W�����̐����������s���N���X
	std::shared_ptr<DungeonManager> dungeon_mgr_ = nullptr;
	// �_���W�����ɐ��������G�̐����A�s������Ȃǂ��Ǘ�����N���X
	std::shared_ptr<EnemyManager> enemy_mgr_ = nullptr;
	// �V�[���v���C�̕\�������UI�̊Ǘ�������N���X
	std::shared_ptr<UI_Manager> ui_mgr_ = nullptr;

	// -------------------------- �퓬�֘A ---------------------------------
	// �U������L�����N�^�[
	std::queue< std::shared_ptr<Character> > attackers_;

	// �U���ڕW
	std::queue < std::shared_ptr<Character> > atk_targets_;

	// ���x���A�b�v����L�����N�^�[
	std::shared_ptr<Character> level_up_character_;

	// ------------------------- �_���W�������֘A ------------------------
	// �e�G���A�̏��i �n�_���W�A�T�C�Y�Ȃ� �j
	std::vector<Area> areas_;
	// �n�`�f�[�^
	std::vector< std::vector<Cell> > field_;
	// ���݂̊K��
	int dungeon_floor_ = 1;
	// �}�b�v�`�b�v��CSV�f�[�^
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;
	// �}�b�v�`�b�v�̉摜�i�[
	std::vector<int> mapchip_gpc_hdl_;
	// �_���W�����𐶐�������
	bool is_created_dungeon_ = false;
	// �_���W������\�����Ă��邩
	bool is_drawing_dng_title_ = true;

	// --------------------------- �t�F�[�h���o�֘A -------------------------
	// �t�F�[�h���o�̉摜
	int fade_gpc_hdl_ = 0;
	// �����x
	int alpha_ = 0;
	// �t�F�[�h���o����
	float fade_time_ = 0.5f;

	// --------------------------- BGM, SE�֘A ------------------------------
	// �_���W������BGM�p�X
	std::string dungeon_bgm_hdl_path_;
	// �_���W������BGM
	int dungeon_bgm_hdl_;
	// BGM �I���n�_�̃T���v�����O���g��
	int bgm_end_freqency_ = 2105775;
	// �_���[�WSE
	std::string damage_se_hdl_path_;
	// �I���E�B���h�E���J���Ƃ���SE�p�X
	std::string open_select_window_se_hdl_path_;
	// ���x���A�b�vSE
	std::string level_up_se_hdl_path_;
	// �R�}���h����SE�̃p�X
	std::string button_enter_se_hdl_path_;
	// �L�����Z��SE�̃p�X
	std::string cancel_se_hdl_path_;

public:
	// ==============================================
	// �v���g�^�C�v�錾
	// ==============================================
	// �w�肵���ʒu�̓G��Ԃ��B���݂��Ȃ��ꍇ�A"nullptr" �ŕԂ�
	std::shared_ptr<EnemyBase> findEnemy(const tnl::Vector3& pos);
	// �U�����s���L�����N�^�[��ǉ�����
	void addAttacker(std::shared_ptr<Character> attacker);
	// �U���Ώۂ�ǉ�
	void addAttackTarget(std::shared_ptr<Character> target);
	// �w�肵���ΏۂɃ_���[�W��^����
	void applyDamage(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target, int damage);
	// ���b�Z�[�W�E�B���h�E�Ƀ��b�Z�[�W���Z�b�g
	void setMessage(const std::string& message);

	// ==============================================
	//				�C�����C���֐�
	// ==============================================

	// ���݂̃t���A���擾
	inline int getFloor() const { return dungeon_floor_; }

	// �w�肳�ꂽ���W�̃}�b�v�f�[�^��Ԃ�
	inline eMapData getMapData(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return eMapData::NONE;
		if (x < 0 || x >= field_[y].size()) return eMapData::NONE;

		return field_[y][x].map_data;
	}

	// �}�b�v�f�[�^���Z�b�g
	inline void setMapData(const tnl::Vector3& pos, eMapData map_data) {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		field_[y][x].map_data = map_data;
	}

	// �n�`�f�[�^���擾����
	inline eMapData getTerrainData(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].terrain_data;
	}

	// �������ʘH����Ԃ��B
	inline ePlace getPlace(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		return field_[y][x].place;
	}

	// �G���A�ԍ���Ԃ�
	inline int getAreaId(const tnl::Vector3& pos) const {
		int x = static_cast<int>(pos.x);
		int y = static_cast<int>(pos.y);

		if (y < 0 || y >= field_.size()) return -1;
		if (x < 0 || x >= field_[y].size()) return -1;

		return field_[y][x].area_id;
	}

	// �����̓����̍��W��Ԃ�
	inline const std::vector<Entrance>& getRoomEntrance(int area_id) const {
		return areas_[area_id].room.entrance;
	}

	// �v���C���[���擾
	inline const std::shared_ptr<Player> getPlayer() const { return player_; }

private:
	//// ========= �f�o�b�O ============

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

	// �L�����N�^�[�A�b�v�f�[�g
	void charaUpdate(float delta_time);
	// �L�����N�^�[��|�����Ƃ��̏���
	void defeatCharacter(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target);
	// �U���L������؂�ւ���B���Ȃ���΁A"dungeon_sequence_"��"seqCharaMove" �ɕύX�B
	void changeAttacker();
	// ���x���A�b�v����������
	void executeLevelUpProcess(std::shared_ptr<Character> chara);
	//// ���̍U���҂��m�F����
	//void checkNextAttacker(std::queue< std::shared_ptr<Character> >& attacker);
	// ���̃t���A�ɕς��鏈�����s��
	void changeProcessNextFloor();
	// �Q�[���I�[�o�[���������s
	void executeGameOverProcess();
	// �v���C���[������Z�����m�F����
	bool checkPlayerCell();


	// =========== ���C���V�[�P���X ===========
	bool seqSceneStart(const float delta_time);
	bool seqFadeIn(const float delta_time);
	bool seqGenerateDungeon(const float delta_time);
	bool seqDrawDungeonTitle(const float delta_time);
	bool seqFadeOut(const float delta_time);
	bool seqMain(const float delta_time);

	// =========== �_���W�����T�����̃V�[�P���X ===========
	// �v���C���[�̓��̓V�[�P���X
	bool seqPlayerAct(const float delta_time);
	// �G�̍s�������߂�V�[�P���X
	bool seqEnemyAct(const float delta_time);
	// �v���C���[�݈̂ړ�����V�[�P���X
	bool seqPlayerAction(const float delta_time);
	// �U���V�[�P���X�i "attacker_"�ɓo�^����Ă���L���� �j
	bool seqCharacterAttack(const float delta_time);
	// �_���[�W�����V�[�P���X
	bool seqTargetDamaged(const float delta_time);
	// ���x���A�b�v�����V�[�P���X
	bool seqCharaLevelUp(const float delta_time);
	// �S�L�����N�^�[�̈ړ��V�[�P���X
	bool seqCharaMove(const float delta_time);
	// �^�[���I���V�[�P���X
	bool seqActEndProcess(const float delta_time);
	// �K�i�I���V�[�P���X
	bool seqStairSelect(const float delta_time);
	// �Q�[���I�[�o�[�V�[�P���X
	bool seqGameOver(const float delta_time);
	// ���j���[�I���V�[�P���X
	bool seqSelectMainMenu(const float delta_time);
	// ���@�I���V�[�P���X
	bool seqSelectMagicList(const float delta_time);
	// �v���C���[�X�e�[�^�X�\���V�[�P���X
	bool seqDrawStatusWindow(const float delta_time);

};