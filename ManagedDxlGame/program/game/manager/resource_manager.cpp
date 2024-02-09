#include "../../dxlib_ext/dxlib_ext.h"
#include "resource_manager.h"


// �R���X�g���N�^
ResourceManager::ResourceManager() {

}

// �f�X�g���N�^
ResourceManager::~ResourceManager() {

	// �摜�f�[�^�̍폜
	for (auto it = gpc_hdl_container_.begin(); it != gpc_hdl_container_.end(); ++it) {
		DeleteGraph(it->second);
	}
	gpc_hdl_container_.clear();

	// �A�j���[�V�����f�[�^�̍폜
	for (auto it = anim_hdl_container_.begin(); it != anim_hdl_container_.end(); ++it) {
		for (int i = 0; i < it->second.size(); ++i) {
			DeleteGraph(it->second[i]);
		}
	}
	anim_hdl_container_.clear();

	// �T�E���h�̍폜
	for (auto it = snd_container_.begin(); it != snd_container_.end(); ++it) {
		DeleteSoundMem(it->second);
	}
	snd_container_.clear();
}

// �C���X�^���X��Ԃ�
ResourceManager* ResourceManager::getInstance() {
	static ResourceManager* instance = nullptr;

	if (!instance) {
		instance = new ResourceManager();
	}
	return instance;
}

// �C���X�^���X���폜
void ResourceManager::destroy() {
	delete getInstance();
}

// �摜���[�h
int ResourceManager::loadGraph(const std::string& gpc_hdl_path) {

	auto it = gpc_hdl_container_.find(gpc_hdl_path);
	if (it != gpc_hdl_container_.end()) return gpc_hdl_container_[gpc_hdl_path];
	int gpc_hdl = LoadGraph(gpc_hdl_path.c_str());
	gpc_hdl_container_.insert(std::make_pair(gpc_hdl_path, gpc_hdl));
	return gpc_hdl;
}

// �摜�̍폜
void ResourceManager::deleteGraph(const std::string& gpc_hdl_path) {

	DeleteGraph(gpc_hdl_container_[gpc_hdl_path]);
	gpc_hdl_container_.erase(gpc_hdl_path);
}

// �A�j���[�V�����̃��[�h�A�A�j���[�V�������̃p�X��Ԃ��B
std::vector<int>& ResourceManager::loadAnimation(const std::string& anim_hdl_path,
	int all_size, int x_size, int y_size, int anim_hdl_width, int anim_hdl_height) {

	auto it = anim_hdl_container_.find(anim_hdl_path);

	// ���łɃA�j���[�V�����̃p�X�����݂����ꍇ�A�p�X��Ԃ��B
	if (it != anim_hdl_container_.end()) return anim_hdl_container_[anim_hdl_path];

	// �A�j���[�V�������쐬���A�R���e�i�Ɋi�[�E�p�X��Ԃ��B
	int* p_anim_hdl = new int[all_size];

	LoadDivGraph(
		anim_hdl_path.c_str(),
		all_size,
		x_size,
		y_size,
		anim_hdl_width,
		anim_hdl_height,
		p_anim_hdl
	);

	std::vector<int> anim_hdl(all_size);

	for (int i = 0; i < all_size; ++i)
		anim_hdl[i] = p_anim_hdl[i];
	
	// �摜�p�X���R���e�i�Ɋi�[
	anim_hdl_container_.insert(make_pair(anim_hdl_path, anim_hdl));
	tnl::DebugTrace("anim_hdl = %x\n", anim_hdl_container_[anim_hdl_path]);

	return anim_hdl_container_[anim_hdl_path];
}

// �A�j���[�V�����̍폜
void ResourceManager::deleteAnimation(const std::string& anim_hdl_path, int all_size) {

	auto it = anim_hdl_container_.find(anim_hdl_path);

	if (it != anim_hdl_container_.end()) {
		for (int i = 0; i < all_size; ++i)
			DeleteGraph(it->second[i]);
	}

	anim_hdl_container_.erase(anim_hdl_path);
}

// �T�E���h�̃��[�h�A�p�X��Ԃ��B
int ResourceManager::loadSound(const std::string& snd_path) {

	auto it = snd_container_.find(snd_path);
	if (it != snd_container_.end()) return snd_container_[snd_path];
	int gpc_hdl = LoadSoundMem(snd_path.c_str());
	snd_container_.insert(std::make_pair(snd_path, gpc_hdl));
	return gpc_hdl;
}

// �T�E���h���폜
void ResourceManager::deleteSound(const std::string& snd_path) {

	DeleteSoundMem(snd_container_[snd_path]);
	snd_container_.erase(snd_path);
}