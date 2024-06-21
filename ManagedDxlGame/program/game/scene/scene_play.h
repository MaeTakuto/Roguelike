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


// �v���C�V�[���N���X
class ScenePlay final : public SceneBase {
public:
	ScenePlay();
	~ScenePlay();

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
	std::shared_ptr<Camera> camera_;
	// �v���C���[
	std::shared_ptr<Player> player_;
	// �_���W�����̐����������s���N���X
	std::shared_ptr<DungeonManager> dungeon_mgr_;
	// �_���W�����ł̏o�������L�^����N���X
	std::shared_ptr<DungeonLog> dungeon_log_;
	// �_���W�����ɐ��������G�̐����A�s������Ȃǂ��Ǘ�����N���X
	std::shared_ptr<EnemyManager> enemy_mgr_ ;
	// �V�[���v���C�̕\�������UI�̊Ǘ�������N���X
	std::shared_ptr<UI_Manager> ui_mgr_;

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
	std::vector< std::vector<DungeonCell> > field_;
	// �~�j�}�b�v�f�[�^
	std::vector< std::vector<MiniMapCell> > mini_map_;
	// ���݂̊K��
	int dungeon_floor_;

	// �}�b�v�`�b�v��CSV�f�[�^
	std::vector< std::vector<tnl::CsvCell> > gpc_hdl_data_;
	// �}�b�v�`�b�v�̉摜�i�[
	std::vector<int> mapchip_gpc_hdl_;
	
	// �_���W�����𐶐�������
	bool is_created_dungeon_;
	// �_���W��������\�����Ă��邩
	bool is_drawing_dng_title_;
	// �Q�[���N���A������
	bool is_game_clear_;
	
	// �~�j�}�b�v�̕\���ʒu
	tnl::Vector2i mini_map_pos_;
	// �~�j�}�b�v��\�����Ă��邩����
	bool is_display_mini_map_;
	// �~�j�}�b�v�̕\���T�C�Y
	int mini_map_size_;
	// �~�j�}�b�v�̉摜
	int mini_map_cell_gpc_hdl_;
	// ���j���[���J���Ă��邩����
	bool is_opened_menu_;

	// �����������Ă��邩
	bool is_hide_explanation_;

	// --------------------------- �t�F�[�h���o�֘A -------------------------
	// �t�F�[�h���o�̉摜
	int fade_gpc_hdl_;
	// �����x
	int alpha_;
	// �t�F�[�h���o����
	float fade_time_;

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
	// �G�̍s�����e���C������
	void modifyEnemyAction();
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
	// �~�j�}�b�v�̃}�X���J��
	void openMiniMapCell();
	// �~�j�}�b�v���X�V
	void updateMiniMap();
	// �~�j�}�b�v��`�悷��
	void drawMiniMap();
	// �L�����N�^�[�A�b�v�f�[�g
	void charaUpdate(float delta_time);
	// ���j���[��ʂ����
	void closeMainMenu();
	// �L�����N�^�[��|�����Ƃ��̏���
	void defeatCharacter(std::shared_ptr<Character> attacker, std::shared_ptr<Character> target);
	// �U���L������؂�ւ���B���Ȃ���΁A"dungeon_sequence_"��"seqCharaMove" �ɕύX�B
	void changeAttacker();
	// ���x���A�b�v����������
	void executeLevelUpProcess(std::shared_ptr<Character> chara);
	// ���@���g���邩�m�F����
	void checkToUseMagic();
	// ���̃t���A�ɕς��鏈�����s��
	void changeProcessNextFloor();
	// �_���W�������O�̏������݂��s��
	void writeDungeonLog();
	// �Q�[���I�[�o�[���������s����
	void executeGameOverProcess();
	// �Q�[���N���A�̏��������s����
	void executeGameClearProcess();
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