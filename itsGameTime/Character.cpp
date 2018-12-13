#include "pch.h"
#include "Character.h"
#include <iostream>
#include <iomanip>

Character::Character()
	:
	rng(rd()),
	dist(0, choices - 1),
	namesDist(0,999),
	levelDist(-1, 1),
	deckDist(0,deckSize - 1)
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
			if (j == 0)
			{
				std::getline(inData, temp, '-');
				rpsChoices[i] = temp;
			}
			else
			{
				std::getline(inData, temp, '-');
				rpsValues[i] = std::stoi(temp);
			}
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
	for (int i = 0; i < deckSize; i++)
	{
		outData << deck[i] << "-";
	}
	std::cout << "Saving!" << std::endl;
	outData.close();
}//jb-1-10-10-1-0-10-61--858993460-

void Character::loadData()
{
	std::ifstream inData;
	std::string damagetemp, currentHPtemp, totalHPtemp, leveltemp, exptemp, decktemp;
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
		for (int i = 0; i < deckSize; i++)
		{
			std::getline(inData, decktemp, '-');
			deck[i] = std::stoi(decktemp);
		}
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

int Character::randomAttack(Character* character)
{
	int randAttack = -1;
	do
	{
		int randNum = deckDist(rng);
		randAttack = deck[randNum];
	} while (randAttack == -1);
	return randAttack;
}

void Character::randomDeck(Character * character)
{
	for (int i = 0; i < deckSize; i++)
	{
		this->deck[i] = rpsValues[dist(rng)];
	}
}

void Character::newChar()
{
	promptName();
	startingDeck();
	setLevel();
	setStats();
	setExp();
}

void Character::newOpponent(Character* character)
{
	setName();
	setLevel(character);
	if (level == 1)
	{
		startingDeck();
	}
	else
	{
		randomDeck(this);
	}
	setStats();
}

void Character::setName()
{
	std::string randName = "tempname ";
	charName = randName + std::to_string(namesDist(rng));
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

int Character::getLevel()
{
	return level;
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
	std::cout << "Current deck: ";
	listDeck();
	std::cout << std::endl;
}	

void Character::listAttacks()
{
	static constexpr int attackWidth = 11;
	static constexpr int columns = 8;
	const std::string separator = " |";
	std::cout << std::endl;
	for (int i = 0; i < choices- 5; i += columns)
	{
		std::cout << separator;
		for (int j = i; j < columns + i; j++)
		{
			std::cout << std::setw(attackWidth) << rpsChoices[j] << separator;
		}
		std::cout << std::endl;
	}
	std::cout << separator;
	for (int i = 97; i < choices; i++) 
	{
		
		std::cout << std::setw(attackWidth) << rpsChoices[i] << separator;
	}
	std::cout << std::setw(0);
}

void Character::listDeck()
{
	for (int i = 0; i < deckSize; i++)
	{
		std::cout << rpsChoices[deck[i] - 1] << " ";
	}
	std::cout << std::endl;
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
		if (level == 2)
		{
			std::cout << "You can now change your deck loadout by typing 'deck' anytime outside of battle!" << std::endl;
		}
	}
}

void Character::rewards(Character* opponent)
{
	int exp;
	exp = (10 * opponent->level);
	std::cout << "You gained " << exp << " exp!" << std::endl;
	this->experience += exp;
	levelUp();
}

void Character::battle(Character* opponent)
{
	std::cout << "Your opponent is: " << opponent->charName << std::endl;
	while (this->currentHP > 0 && opponent->currentHP > 0)
	{
		int result, opponentChoice;
		int playerChoice = selectAttack();
		opponentChoice = randomAttack(opponent); // rolls a number between 0- amount of choices
		std::cout << "Opponent chose: " << convertToString(opponentChoice) << std::endl;
		result = comparingChoices(playerChoice, opponentChoice);
		results(result, playerChoice, opponentChoice, opponent);
	}
	if (this->currentHP > 0)
	{
		std::cout << "You won the battle!" << std::endl;
		rewards(opponent);
	}
	else
	{
		std::cout << "You lost the battle!" << std::endl;
	}
}

void Character::results(int result, int playerChoice, int opponentChoice, Character* opponent)
{
	if (result == 0)
	{
		std::cout << "It's a Draw!" << std::endl;
	}
	else if (result == 1)
	{
		std::cout << convertToString(opponentChoice) << " beats " << convertToString(playerChoice)
		<< ", You lose!" << std::endl;
		this->currentHP -= opponent->damage;
		std::cout << "You take " << opponent->damage << " damage" << std::endl;	
	}
	else if (result == 2)
	{
		std::cout << convertToString(playerChoice) << " beats " << convertToString(opponentChoice)
		<< ", You win!" << std::endl;
		opponent->currentHP -= this->damage;
		std::cout << "You deal " << this->damage << " damage to your opponent!" << std::endl;
		
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

int Character::attackType(std::string choice) //maybe more to change
{
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (choice == rpsChoices[i])
		{
			return rpsValues[i];
		}
		else if (choice != rpsChoices[i])
		{
			counter++;
		}
		if (counter == choices)
		{
			selectAttack();
			break;
		}

	}
}

int Character::selectAttack()
{
	std::string choice = "";
	std::cout << "Choose your attack: ";
	listDeck();
	std::cout << std::endl;
	std::cin >> choice;
	return attackType(formatText(choice));
}

int Character::selectfromAll()
{
	std::string choice = "";
	std::cout << "Choose your deck attacks: ";
	listAttacks();
	std::cout << std::endl;

	std::cin >> choice;
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (formatText(choice) == rpsChoices[i])
		{
			std::cout << "stored " << rpsChoices[i] << std::endl;
			return rpsValues[i];
		}
		else if (choice != rpsChoices[i])
		{
			counter++;
		}
		if (counter == choices)
		{
			selectfromAll();
			break;
		}

	}
}

void Character::startingDeck()
{
	deck[0] = 10;
	deck[1] = 61;
	deck[2] = 18;
}

void Character::changeDeck()
{
	for (int i = 0; i < deckSize; i++)
	{
		deck[i] = selectfromAll();
	}
	
}

std::string Character::convertToString(int number)
{
	return rpsChoices[number - 1];
}
