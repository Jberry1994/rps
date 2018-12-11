#include "pch.h"
#include "Character.h"


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
		for (int j = 0; j <= 2; j++)
		{
			std::getline(inData, temp, '-');
			std::cout << temp << std::endl;
			rps[i][j] = temp;
		}
	}
	inData.close();
}

void Character::saveData()
{
	std::ofstream outData;
	outData.open("save-game.txt");
	outData << charName << "-" << charClass[0] << "-" << charClass[1] << "-"
		<< damage << "-" << currentHP << "-" 
		<< totalHP << "-" << level << "-" 
		<< experience << "-";
	for (int i = 0; i < choices; i++)
	{
		outData << rps[i][2] << "-";
	}
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
		std::getline(inData, charClass[0], '-');
		std::getline(inData, charClass[1], '-');
		std::getline(inData, damagetemp, '-');
		std::getline(inData, currentHPtemp, '-');
		std::getline(inData, totalHPtemp, '-');
		std::getline(inData, leveltemp, '-');
		std::getline(inData, exptemp, '-');
		for (int i = 3; i < choices; i++)
		{
			std::string temp;
			std::getline(inData, temp, '-');
			rps[i][2] = temp;
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

std::string Character::randomClass(Character* character)
{
	std::string randClass = " ";
	do
	{
		int randNum = dist(rng);
		if (character->rps[randNum][2] == "unlocked")
		{
			randClass = rps[randNum][0];
		}
	} while (randClass == " ");
	return randClass;
}

void Character::newChar()
{
	promptName();
	promptClass();
	setLevel();
	setStats();
	setExp();
}

void Character::newOpponent(Character* character)
{
	setName();
	setClass(character);
	setLevel(character);
	setStats();
}

void Character::setName()
{
	charName = "test name " + std::to_string(namesDist(rng));
}

void Character::setName(std::string name)
{
		charName = name;
}

void Character::setClass(Character* character)
{
	charClass[0] = randomClass(character);
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (charClass[0] == rps[i][0])
		{
			charClass[1] = rps[i][1];
			break;
		}
	}
}

void Character::setClass(std::string classChoice)
{
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (classChoice == rps[i][0] && rps[i][2] != "locked")
		{
			charClass[0] = classChoice;
			charClass[1] = rps[i][1];
		}
		else if (classChoice != rps[i][0] || rps[i][2] == "locked")
		{
			counter++;
		}
		if (counter == choices)
		{
			promptClass();
		}

	}
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
		charName << ", Class: " <<
		charClass[0] << ", Level: " <<
		level << ", Experience: " << 
		experience << ", Damage: " <<
		damage << ", Current HP: " <<
		currentHP << ",  Max HP: " <<
		totalHP << std::endl;
}

void Character::listClasses()
{
	for (int i = 0; i < choices; i++)
	{
		if (rps[i][2] == "unlocked")
		{
			std::cout << rps[i][0];
			if (i < choices - 1)
			{
				std::cout << ", ";
			}
			else
				std::cout << " ";
		}

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

void Character::promptClass()
{
	std::string tempClass;
	std::cout << "Pick a class for your character: ";
	listClasses();
	std::cin >> tempClass;
	setClass(formatText(tempClass));

}

void Character::capsLock(std::string& input)
{
	int i = 0;
	char c;
	while (input[i])
	{
		c = input[i];
		input[i] = (toupper(c));
		i++;
	}
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
		if (level >= 2)
		{
			if (classType(charClass[1]) == 0)
			{
				rps[3][2] = "unlocked";
			}
			else if (classType(charClass[1]) == 1)
			{
				rps[4][2] = "unlocked";
			}
			else if (classType(charClass[1]) == 2)
			{
				rps[5][2] = "unlocked";
			}
			std::cout << "You have unlocked a new attack!" << std::endl;
		}
	}
}

void Character::rewards(Character* player, Character* opponent)
{
	int exp;
	exp = (10 * opponent->level) * comparingClasses(player, opponent);
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
		opponentChoice = randomClass(opponent); // rolls a number between 0- amount of choices
		std::cout << "Opponent chose: " << opponentChoice << std::endl;
		result = comparingChoices(attackType(playerChoice), attackType(opponentChoice));
		results(result, playerChoice, opponentChoice, player, opponent);
	}
	if (player->currentHP > 0)
	{
		std::cout << "You won the battle!" << std::endl;
		rewards(player, opponent);
		//opponent->newOpponent();
		
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
		std::cout << opponentChoice << " beats " << playerChoice;
		std::cout << ", You lose!" << std::endl;
		if (checkClass(opponent, opponentChoice))
		{
			player->currentHP -= opponent->damage * dmgMult;
			std::cout << "You take " << opponent->damage  * dmgMult << " damage" << std::endl;
		}
		else
		{
			player->currentHP -= opponent->damage;
			std::cout << "You take " << opponent->damage << " damage" << std::endl;
		}
		
	}
	else if (result == 2)
	{
		std::cout << playerChoice << " beats " << opponentChoice;
		std::cout << ", You win!" << std::endl;
		if (checkClass(player, playerChoice))
		{
			opponent->currentHP -= player->damage * dmgMult;
			std::cout << "You deal " << player->damage * dmgMult << " damage to your opponent!" << std::endl;
		}
		else
		{
			opponent->currentHP -= player->damage;
			std::cout << "You deal " << player->damage << " damage to your opponent!" << std::endl;
		}
		
	}
	else if (result == 3)
	{
		std::cout << "Something went wrong!" << std::endl;
	}
}

int Character::comparingChoices(int userChoice, int opponentChoice)
{
	if (userChoice == opponentChoice)
		return 0; // 0 for Draw
	else if ((userChoice == 0 && opponentChoice == 1) || (userChoice == 1 && opponentChoice == 0))
		return (userChoice == 0 ? 1 : 2);
		
	else if ((userChoice == 0 && opponentChoice == 2) || (userChoice == 2 && opponentChoice == 0))
		return (userChoice == 0 ? 2 : 1);
		
	else if ((userChoice == 1 && opponentChoice == 2) || (userChoice == 2 && opponentChoice == 1))
		return (userChoice == 1 ? 1 : 2);

	else
		return 3;
}

int Character::comparingClasses(Character * player, Character * opponent)
{
	int playerclass = classType(player->charClass[1]);
	int opponentclass = classType(opponent->charClass[1]);
	if (playerclass == opponentclass)
	{
		return 2; // 2 is normal
	}
	if ((playerclass == 0 && opponentclass == 1 ) || (playerclass == 1 && opponentclass == 0))
	{
		if (playerclass == 0)
		{
			return 3; // 3 is hard
		}
		else
		{
			return 1; // 1 is easy
		}
	}
	if ((playerclass == 0 && opponentclass == 2) || (playerclass == 2 && opponentclass == 0))
	{
		if (playerclass == 0)
		{
			return 1;
		}
		else
		{
			return 3;
		}
	}
	if ((playerclass == 1 && opponentclass == 2) || (playerclass == 2 && opponentclass == 1))
	{
		if (playerclass == 1)
		{
			return 3;
		}
		else
		{
			return 1;
		}
	}
}

int Character::attackType(std::string choice)
{
	int counter = 0;
	for (int i = 0; i < choices; i++)
	{
		if (choice == rps[i][0] && rps[i][2] == "unlocked")
		{
			return stoi(rps[i][1]);
		}
		else if (choice != rps[i][0])
		{
			counter++;
		}
		if (counter == choices)
		{
			selectAttack();
		}

	}
}

int Character::classType(std::string charClass)
{
			return stoi(charClass);
}

bool Character::checkClass(Character* character, std::string choice)
{
	//std::cout << charClass << " " << choice << std::endl;
	return (stoi(character->charClass[1]) == attackType(choice));
}

std::string Character::selectAttack()
{
	std::string choice;
	std::cout << "Choose your attack: ";
	listClasses();
	std::cin >> choice;
	return formatText(choice);
}


