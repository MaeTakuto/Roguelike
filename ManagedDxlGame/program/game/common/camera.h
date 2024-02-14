#pragma once
#include "../manager/gm_manager.h"


class Camera {
public:
	Camera(const tnl::Vector3& target = { 0, 0, 0 });
	~Camera();

	void update(const tnl::Vector3& target = { 0, 0, 0 });
	void control();

	inline const tnl::Vector3& getPos() { return pos_; }
	inline void setPos(const tnl::Vector3& target) { 
		pos_ = tnl::Vector3(target.x * GameManager::DRAW_CHIP_SIZE, target.y * GameManager::DRAW_CHIP_SIZE, 0);

		if (target.x < MAX_CAMERA_POS_X)								pos_.x = MAX_CAMERA_POS_X * GameManager::DRAW_CHIP_SIZE;
		if (target.x > GameManager::FIELD_WIDTH - MAX_CAMERA_POS_X)		pos_.x = ( GameManager::FIELD_WIDTH - MAX_CAMERA_POS_X ) * GameManager::DRAW_CHIP_SIZE;
		if (target.y < MAX_CAMERA_POS_Y)								pos_.y = MAX_CAMERA_POS_Y * GameManager::DRAW_CHIP_SIZE;
		if (target.y > GameManager::FIELD_HEIGHT - MAX_CAMERA_POS_Y)	pos_.y = ( GameManager::FIELD_HEIGHT - MAX_CAMERA_POS_Y ) * GameManager::DRAW_CHIP_SIZE;
	}

private:
	const int MAX_CAMERA_POS_Y = 7;
	const int MAX_CAMERA_POS_X = 12;

	tnl::Vector3 pos_;
	tnl::Vector3 target_;

};