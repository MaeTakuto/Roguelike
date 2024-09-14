#pragma once
#include "../base/scene_base.h"

class MessageWindow;

// =====================================================================================
// �Q�[���N���A��̃V�[���N���X
// =====================================================================================
class SceneGameClear final : public SceneBase {
public:
	SceneGameClear();
	~SceneGameClear();

	void update(float delta_time) override;
	void draw() override;

private:
	tnl::Sequence<SceneGameClear> sequence_;

	std::shared_ptr<MessageWindow> message_window_;

	// �\�����Ă��郁�b�Z�[�W�̃y�[�W
	int now_page_;

	// �G���f�B���O�C�x���g�̃��b�Z�[�W
	std::vector< std::vector< std::string > > event_messages_;

	// SE���Đ���������
	bool is_playing_se_;

	// ���ݕ\�����̃��b�Z�[�W�ɍĐ�������ʉ��̃p�X�i �Đ�����s�ASE�̃p�X �j
	std::queue<std::pair<int, std::string>> se_path_to_message_page_;

	// �^�C�g���w�i�摜
	int back_ground_gpc_hdl_;

private:
	// SE���Đ����邩�m�F
	void checkToPlaySE();

	// �Q�[���N���A�V�[���̃V�[�P���X
	bool seqSceneStart(const float delta_time);
	bool seqMessageEvent(const float delta_time);
	bool seqSceneEnd(const float delta_time);

};

