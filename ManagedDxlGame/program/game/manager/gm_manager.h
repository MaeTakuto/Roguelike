#pragma once

class SceneBase;
class DungeonLog;

using CsvData = std::vector< std::vector<tnl::CsvCell> >;

// �e�����̃x�N�g��
const tnl::Vector3 DIR_POS[8]
	= { { 0, -1, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 1, 0 , 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { -1, 1, 0 }, { 1, 1, 0 } };

class GameManager {
public:
	// �f�X�g���N�^
	~GameManager();

	// ====== const ======
	// �摜�`�b�v�T�C�Y
	static constexpr int CHIP_SIZE = 32;
	// �I�u�W�F�N�g�̕\���T�C�Y
	static constexpr int DRAW_CHIP_SIZE = CHIP_SIZE * 2;
	// csv �̃Z���̓ǂݎ��̍ŏ��̍s
	static constexpr int CSV_CELL_ROW_START = 1;

	// �Q�[���t�H���g�̒ʏ�T�C�Y
	static constexpr int GM_DEFAULT_FONT_SIZE = 100;

	// �_���W�����̃T�C�Y
	static constexpr int FIELD_WIDTH = 64;
	static constexpr int FIELD_HEIGHT = 32;

	// �Q�[���}�l�[�W���[�̃A�h���X��Ԃ�
	static GameManager* GetInstance(std::shared_ptr<SceneBase> = nullptr);

	// �A�h���X�̍폜
	inline static void Destroy() { delete GetInstance(); }

	// �Q�[���}�l�[�W���[�̍X�V
	void update(float delta_time);

	// �V�[���̐؂�ւ����s��
	void changeScene(std::shared_ptr<SceneBase> next_scene, float fade_time = 1.0f);

	// �V�[���̃C���X�^���X��Ԃ��B
	inline const std::shared_ptr<SceneBase> getSceneInstance() const { return now_scene_; }	

	// �t�F�[�h���o�t���O��Ԃ��B
	inline bool isTransition() const { return is_transition_; }

	// �Q�[�����N���A�����������Ԃ�
	inline bool isGameClear() const { return is_game_clear_; }

	// �t�H���g�n���h����Ԃ��B
	inline int getDefaultFontHdl() const { return default_font_hdl_; }

	// "is_game_clear_" �̒l��ݒ肷��
	inline void setGameClear(bool is_game_clear) { is_game_clear_ = is_game_clear; }

	// �_���W�����L�^��ǉ�����i�����X�R�A�����X�g���Ⴂ�ꍇ�A�ǉ�����Ȃ��j
	void addDungeonLog(std::shared_ptr<DungeonLog> dungeon_log);
	// �_���W�����L�^�̃��X�g���擾
	const std::vector< std::shared_ptr<DungeonLog> >& getDungeonLogList() const;

private:
	// �R���X�g���N�^
	GameManager(std::shared_ptr<SceneBase> start_scene);

	// ���݂̃V�[��
	std::shared_ptr<SceneBase> now_scene_;
	// ���̃V�[��
	std::shared_ptr<SceneBase> next_scene_;

	// �_���W�����L�^���X�g
	std::vector< std::shared_ptr<DungeonLog> > dungeon_log_list_;

	// �ʏ�̃t�H���g
	int default_font_hdl_;

	// -------------------- �t�F�[�h���o�֘A -----------------------------

	// �t�F�[�h���o�摜�p�X
	std::string transition_gpc_hdl_path_;

	// �t�F�[�h���o�摜
	int transition_gpc_hdl_;

	// �t�F�[�h���o����
	float fade_time_;
	// �t�F�[�h���o�ҋ@����
	float fade_time_wait_;
	// �t�F�[�h���o�t���O
	bool is_transition_;

	// --------------------------------------------------------------------

	// �Q�[�����N���A����������
	bool is_game_clear_;

	// �Q�[���}�l�[�W���[�̃V�[�P���X
	tnl::Sequence<GameManager> sequence_;

	// �e�V�[�P���X
	bool seqRun(const float delta_time);									// �������Ȃ�
	bool seqTransOut(const float delta_time);								// �t�F�[�h�A�E�g
	bool seqWait(const float delta_time);									// �t�F�[�h���o�ҋ@
	bool seqTransIn(const float delta_time);								// �t�F�[�h�C��

};