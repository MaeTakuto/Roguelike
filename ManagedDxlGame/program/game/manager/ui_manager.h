#pragma once

class Camera;
class MessageWindow;

class UI_Manager {
public:
	UI_Manager();
	~UI_Manager();

	void update(float delta_time);
	void draw(const std::shared_ptr<Camera> camera);

	std::shared_ptr<MessageWindow> getMessageWindow() { return message_window_; }

private:
	std::shared_ptr<MessageWindow> message_window_ = nullptr;

};