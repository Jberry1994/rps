#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <random>

class Character
{
public:
	Character();
	~Character();
	void rpsInitialize();
	void saveData();
	void loadData();
	std::string randomAttack(Character* character);
	void newChar();
	void newOpponent(Character* character);
	void setName();
	void setName(std::string name);
	void setLevel();
	void setLevel(Character* character);
	void setStats();
	void setExp();
	void print();
	void listAttacks();
	void promptName();
	std::string formatText(std::string input);
	void resting();
	void levelUp();
	void rewards(Character* player, Character* opponent);
	void battle(Character* player, Character* opponent);
	void results(int result, std::string playerChoice, std::string opponentChoice, Character* player, Character* opponent);
	int comparingChoices(int userChoice, int opponentChoice) const;
	int attackType(std::string choice);
	std::string selectAttack();
	static constexpr int choices = 101;
	std::string rps[choices][2];

private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
	std::uniform_int_distribution<int> namesDist;
	std::uniform_int_distribution<int> levelDist;
	int damage, currentHP, totalHP, level, experience;
	static constexpr int dmgMult = 2;
	std::string charName;

	
	

};

