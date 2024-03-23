#pragma once

class Character;

class SkillBase {
public:
	SkillBase();
	~SkillBase();

	void useSkill(std::shared_ptr<Character> user);

private:

};