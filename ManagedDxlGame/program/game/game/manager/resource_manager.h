#pragma once
#include "gm_manager.h"

// =====================================================================================
// �摜�A�A�j���[�V�����A�T�E���h�̃p�X���Ǘ�����N���X�i Flyweight �p�^�[�� �j
// =====================================================================================
class ResourceManager final {
public:
	// �f�X�g���N�^
	~ResourceManager();

	// �C���X�^���X��Ԃ�
	static ResourceManager* getInstance();

	// �C���X�^���X���폜
	static void destroy();

	// ============================================================
	// �摜�A�A�j���[�V�����֘A 
	// ============================================================
	// �摜�̃��[�h�A�摜�̃A�h���X��n���B
	int loadGraph(const std::string& gpc_hdl_path);

	// �摜�̍폜
	void deleteGraph(const std::string& gpc_hdl_path);

	// �A�j���[�V�����̃��[�h
	std::vector<int>& loadAnimation(const std::string& anim_hdl_path,
		int all_size, int x_size, int y_size, int anim_hdl_width, int anim_hdl_height);

	// �A�j���[�V�����̍폜
	void deleteAnimation(const std::string& anim_hdl_path);

	// ============================================================
	// �T�E���h�֘A 
	// ============================================================
	// �T�E���h�̃A�h���X��n���B
	int loadSound(const std::string& snd_path);

	// �T�E���h���Đ�
	void playSound(const std::string& snd_path, int play_type, int top_position_flag = 1);

	// �T�E���h���Đ�����Ă��邩����
	bool checkSound(const std::string& snd_path);

	// �T�E���h���~
	void stopSound(const std::string& snd_path);

	// �T�E���h�̍폜
	void deleteSound(const std::string& snd_path);

	// ============================================================
	// CSV�֘A 
	// ============================================================

	// csv��ǂݍ��ݕԂ�
	CsvData& loadCsvData(const std::string& csv_path);

	// csv�f�[�^���폜
	void deleteCsvData(const std::string& csv_path);

	// �S�Ẵf�[�^���폜
	void clearAllResources();

private:
	// �R���X�g���N�^
	ResourceManager();

	// �摜�f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, int > gpc_hdl_container_;

	// �A�j���[�V�����f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, std::vector<int> >
		anim_hdl_container_;

	// �T�E���h�f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, int > snd_container_;

	// CSV�f�[�^�̃R���e�i
	std::unordered_map< std::string, CsvData > csv_data_container_;

};