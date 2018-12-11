#include "pch.h"
#include "Character.h"
#include <iostream>
#include <iomanip>

Character::Character()
	:
	rng(rd()),
	dist(0, choices - 1),
	namesDist(0,999),
	levelDist(-1 , 1)
{
}

Character::~Character()
{
}

void Character::rpsInitialize()
{
	std::ifstream inData;
	inData.open("rps-options.txt");
	std::string temp;
	for (int i = 0; i < choices; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			std::getline(inData, temp, '-');
			rps[i][j] = temp;
		}
	}
	inData.close();
}

void Character::saveData()
{
	std::ofstream outData;
	outData.open("save-game.txt");
	outData << charName << "-" 
		<< damage << "-" << currentHP << "-" 
		<< totalHP << "-" << level << "-" 
		<< experience << "-";
	std::cout << "Saving!" << std::endl;
	outData.close();
}

void Character::loadData()
{
	std::ifstream inData;
	std::string damagetemp, currentHPtemp, totalHPtemp, leveltemp, exptemp;
	inData.open("save-game.txt");
	if (inData.peek() <= 2)
	{	
		std::cout << "No save found, making new character!" << std::endl;
		inData.close();
		newChar();
	}
	else
	{
		std::getline(inData, charName, '-');
		std::getline(inData, damagetemp, '-');
		std::getline(inData, currentHPtemp, '-');
		std::getline(inData, totalHPtemp, '-');
		std::getline(inData, leveltemp, '-');
		std::getline(inData, exptemp, '-');
		damage = std::stoi(damagetemp);
		currentHP = std::stoi(currentHPtemp);
		totalHP = std::stoi(totalHPtemp);
		level = std::stoi(leveltemp);
		experience = std::stoi(exptemp);
		inData.close();
		levelUp();
		std::cout << "Loading!" << std::endl;
		std::cout << "Loaded stats are: ";
		print();
	}

}

std::string Character::randomAttack(Character* character)
{
	std::string randAttack = " ";
	do
	{
		int randNum = dist(rng);
		randAttack = rps[randNum][0];
	} while (randAttack == " ");
	return randAttack;
}

void Character::newChar()
{
	promptName();
	setLevel();
	setStats();
	setExp();
}

void Character::newOpponent(Character* character)
{
	setName();
	setLevel(character);
	setStats();
}

void Character::setName()
{
	int randNum = dist(rng);
	std::string tempname = rps[1][0];
	charName = tempname + " " + std::to_string(namesDist(rng));
	std::cout << "charName: " << charName << std::endl;
	std::cout << "randNum: " << randNum << std::endl;
	std::cout << "rps[randnum][0]: " << rps[randNum][0] << std::endl;
}

void Character::setName(std::string name)
{
		charName = name;
}

void Character::setLevel()
{
	level = 1;
}

void Character::setLevel(Character * character)
{
	level = character->level + levelDist(rng);
	if (level < 1)
	{
		level = 1;
	}
}

void Character::setStats()
{
	damage = 1 + (level - 1);
	totalHP = 10 + (5 * level - 5);
	currentHP = totalHP;
}

void Character::setExp()
{
	experience = 0;
}

void Character::print()
{
	std::cout << "Name: " <<
		charName << ", Level: " <<
		level << ", Experience: " << 
		experience << ", Damage: " <<
		damage << ", Current HP: " <<
		currentHP << ",  Max HP: " <<
		totalHP << std::endl;
}

void Character::listAttacks()
{
	const int attackWidth = 11;
	const int columns = 8;
	const std::string separator = " |";
	const int total_width = (attackWidth * columns) + separator.size() * columns;
	const std::string line = separator + std::string(total_width - 1, '-') + '|';
	std::cout << std::endl << line << std::endl;
	for (int i = 0; i < choices- 5; i += columns)
	{
		std::cout << separator;
		for (int j = i; j < columns + i; j++)
		{
			std::cout << std::setw(attackWidth) << rps[j][0] << separator;
		}
		std::cout << std::endl;
	}
	std::cout << separator;
	for (int i = 97; i < choices; i++) 
	{
		
		std::cout << std::setw(attackWidth) << rps[i][0] << separator;
	}
}

void Character::promptName()
{
	std::string tempName;
	std::cout << "Enter a Name for your character: ";
	std::cin.ignore();
	std::getline(std::cin, tempName);
	setName(tempName);
}

std::string Character::formatText(std::string input)
{
	for (size_t i = 0; i < input.length(); i++)
	{
		if (i == 0)
		{
			input[0] = toupper(input[0]);
		}
		else
		{
			input[i] = tolower(input[i]);
		}
	}
	return input;
}

void Character::resting()
{
	for (currentHP; currentHP < totalHP; currentHP++)
	{
		std::cout << "Resting!" << std::endl;
	}
	std::cout << "Full HP!" << std::endl;
}

void Character::levelUp()
{
	if (experience >= level * 100)
	{
		int overage = 0;
		if (experience > level * 100)
		{
			overage = experience - (level * 100);
		}
		level++;
		setStats();
		setExp();
		experience += overage;
		std::cout << "Level up!" << std::endl;
	}
}

void Character::rewards(Character* player, Character* opponent)
{
	int exp;
	exp = (10 * opponent->level);
	std::cout << "You gained " << exp << " exp!" << std::endl;
	player->experience += exp;
	levelUp();
}

void Character::battle(Character* player, Character* opponent)
{
	std::cout << "Your opponent is: " << opponent->charName << std::endl;
	while (player->currentHP > 0 && opponent->currentHP > 0)
	{
		std::string playerChoice, opponentChoice;
		int result;
		playerChoice = selectAttack();
		opponentChoice = randomAttack(opponent); // rolls a number between 0- amount of choices
		std::cout << "Opponent chose: " << opponentChoice << std::endl;
		result = comparingChoices(attackType(playerChoice), attackType(opponentChoice));
		results(result, playerChoice, opponentChoice, player, opponent);
	}
	if (player->currentHP > 0)
	{
		std::cout << "You won the battle!" << std::endl;
		rewards(player, opponent);
	}
	else
	{
		std::cout << "You lost the battle!" << std::endl;
	}
}

void Character::results(int result, std::string playerChoice, std::string opponentChoice, Character* player, Character* opponent)
{
	if (result == 0)
	{
		std::cout << "It's a Draw!" << std::endl;
	}
	else if (result == 1)
	{
		std::cout << opponentChoice << " beats " << playerChoice
		<< ", You lose!" << std::endl;
		player->currentHP -= opponent->damage;
		std::cout << "You take " << opponent->damage << " damage" << std::endl;	
	}
	else if (result == 2)
	{
		std::cout << playerChoice << " beats " << opponentChoice
		<< ", You win!" << std::endl;
		opponent->currentHP -= player->damage;
		std::cout << "You deal " << player->damage << " damage to your opponent!" << std::endl;
		
	}
	else if (result == 3)
	{
		std::cout << "Something went wrong!" << std::endl;
	}
}

int Character::comparingChoices(int userChoice, int opponentChoice) const
{
	if (userChoice == opponentChoice)
		return 0;
	int userRange = userChoice + 50;
	int userOverage;
	userOverage = (userRange > choices ? userRange - choices : 0);
	if (opponentChoice <= userOverage || (opponentChoice <= userRange && opponentChoice >= userChoice))
		return 2;
	else
		return 1;
}

int Character::attackType(std::string choice)
{
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (choice == rps[i][0])
		{
			return stoi(rps[i][1]);
		}
		else if (choice != rps[i][0])
		{
			counter++;
		}
		if (counter == choices)
		{
			attackType(selectAttack());
		}

	}
}

std::string Character::selectAttack()
{
	std::string choice = "";
	std::cout << "Choose your attack: ";
	listAttacks();
	std::cout << std::endl;
	std::getline(std::cin, choice);
	return formatText(choice);
}
