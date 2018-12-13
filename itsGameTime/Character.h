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
	int randomAttack(Character* character);
	void randomDeck(Character* character);
	void newChar();
	void newOpponent(Character* character);
	void setName();
	void setName(std::string name);
	void setLevel();
	void setLevel(Character* character);
	void setStats();
	void setExp();
	int getLevel();
	void print();
	void listAttacks();
	void listDeck();
	void promptName();
	std::string formatText(std::string input);
	void resting();
	void levelUp();
	void rewards(Character* opponent);
	void battle(Character* opponent);
	void results(int result, int playerChoice, int opponentChoice, Character* opponent);
	int comparingChoices(int userChoice, int opponentChoice) const;
	int attackType(std::string choice);
	int selectAttack();
	int selectfromAll();
	void startingDeck();
	void changeDeck();
	std::string convertToString(int number);
private:
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
	std::uniform_int_distribution<int> namesDist;
	std::uniform_int_distribution<int> levelDist;
	std::uniform_int_distribution<int> deckDist;
	int damage, currentHP, totalHP, level, experience;
	static constexpr int dmgMult = 2;
	static constexpr int deckSize = 3;
	static constexpr int choices = 101;
	std::string charName;
	std::string rpsChoices[choices];
	int rpsValues[choices];

	int deck[deckSize];
};

