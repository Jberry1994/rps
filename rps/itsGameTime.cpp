#include "pch.h"
#include <iostream>
#include "Character.h"

void gameChoice(Character* player, Character* opponent);
void gamePlay(Character* player, Character* opponent);


int main()
{
	Character one;
	Character two;
	Character *player;
	Character *opponent;
	player = &one;
	opponent = &two;
	
	gameChoice(player, opponent);
}



void gameChoice(Character* player, Character* opponent)
{
	std::string gameChoice = " ";
	std::cout << "Type 'New' or 'Load' to start the game." << std::endl;

	std::cin >> gameChoice;
	if (gameChoice == "load" || gameChoice == "Load")
	{
		player->loadData();
	}
	if (gameChoice == "new" || gameChoice == "New")
	{
		player->newChar();
	}
	if (gameChoice != "exit" && gameChoice != "Exit")
	{
	}
	gamePlay(player, opponent);
}


void gamePlay(Character* player, Character* opponent)
{
	std::string action = " ";
	
	while (action != "exit" && action != "Exit")
	{
		std::cout << "Options: Save, Print, Play, Rest, Menu, or Exit" << std::endl;
		std::cin >> action;
		if (action == "save" || action == "Save")
		{
			player->saveData();
		}
		if (action == "print" || action == "Print")
		{
			player->print();
		}
		if (action == "play" || action == "Play")
		{
			opponent->newOpponent(player);
			player->battle(player, opponent);

		}
		if (action == "rest" || action == "Rest")
		{
			player->resting();
		}
		if (action == "menu" || action == "Menu")
		{
			gameChoice(player, opponent);
			break;
		}

	}

}




/*
class specialties, bonus damage when winning with that attack
extra class unlocks ---  









things implimented -> Saving/Loading, leveling up/experience, classes, random enemies, bonus damage, resting, menu,






*/