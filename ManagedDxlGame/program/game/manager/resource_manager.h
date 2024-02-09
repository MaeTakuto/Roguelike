#pragma once



// �摜�A�A�j���[�V�����A�T�E���h�̃p�X���Ǘ�����N���X
class ResourceManager {
public:
	// �C���X�^���X��Ԃ�
	static ResourceManager* getInstance();

	// �C���X�^���X���폜
	static void destroy();

	// �摜�̃��[�h�A�摜�̃A�h���X��n���B
	int loadGraph(const std::string& gpc_hdl_path);

	// �摜�̍폜
	void deleteGraph(const std::string& gpc_hdl_path);

	// �A�j���[�V�����̃��[�h
	std::vector<int>& loadAnimation(const std::string& anim_hdl_path,
		int all_size, int x_size, int y_size, int anim_hdl_width, int anim_hdl_height);

	// �A�j���[�V�����̍폜
	void deleteAnimation(const std::string& anim_hdl_path, int all_size);

	// �T�E���h�̃A�h���X��n���B
	int loadSound(const std::string& snd_path);

	// �T�E���h�̍폜
	void deleteSound(const std::string& snd_path);

private:
	ResourceManager();
	~ResourceManager();

	// �摜�f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, int > gpc_hdl_container_;

	// �A�j���[�V�����f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, std::vector<int> >
		anim_hdl_container_;

	// �T�E���h�f�[�^�̊i�[�p�R���e�i
	std::unordered_map< std::string, int > snd_container_;

};