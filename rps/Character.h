#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

class Character
{
public:
	Character();
	~Character();
	void saveData();
	void loadData();
	std::string randomClass(Character* character);
	void newChar();
	void newOpponent(Character* character);
	void setName();
	void setName(std::string name);
	void setClass(Character* character);
	void setClass(std::string classChoice);
	void setLevel();
	void setLevel(Character* character);
	void setStats();
	void setExp();
	void print();
	void listClasses();
	void promptName();
	void promptClass();
	void capsLock(std::string& input);
	std::string formatText(std::string input);
	void resting();
	void levelUp();
	void rewards(Character* player, Character* opponent);
	void battle(Character* player, Character* opponent);
	void results(int result, std::string playerChoice, std::string opponentChoice, Character* player, Character* opponent);
	int comparingChoices(int userChoice, int opponentChoice);
	int comparingClasses(Character* player, Character* opponent);
	int attackType(std::string choice);
	int classType(std::string charClass);
	bool checkClass(Character* character, std::string choice);
	std::string selectAttack();
	static constexpr int choices = 6;
	//std::string rps[choices][3];
	std::string rps[choices][3] = { {"Rock", "0", "unlocked"}, {"Paper", "1", "unlocked"}, {"Scissors", "2", "unlocked"}, {"Dragonscim", "0", "locked"}, {"Ancientmagics", "1", "locked"}, {"Magicshortbow", "2", "locked"} };

private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
	std::uniform_int_distribution<int> namesDist;
	std::uniform_int_distribution<int> levelDist;
	int damage, currentHP, totalHP, level, experience;
	static constexpr int dmgMult = 2;
	std::string charName, charClass[2];

	
	

};

