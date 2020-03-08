// BitTacToe2.cpp
//

#include <iostream>
#include <bitset>
#include <array>
//#include <random>
//#include <functional>
#include <time.h>

#include "BitTacToeEngine.h"
/*	 ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= ======= =======
*/

char cellInputFilter();
int gameTypeFilter();
void gameLoop_Human(BitTacToe b, bool player);
void gameLoop_Synth(BitTacToe b, bool player, bool depthWeight = false);

int main()
{
	BitTacToe b;
	srand(time(NULL));

	std::cout << "Play Versus:	0 = Human\n\t\t"
		<< "1 = AI\n\t\t"
		<< "2 = AI w/ depth weighting (experimental)\n\t\t"
		<< "q = Quit\n\n";
	int gametype = gameTypeFilter();

	while (gametype != 99) {
		switch (gametype)
		{
		case 0:
			gameLoop_Human(b, (rand() & 1));
			break;
		case 1:
			gameLoop_Synth(b, (rand() & 1), false);
			break;
		case 2:
			gameLoop_Synth(b, (rand() & 1), true);
			break;
		case 99:
		default:
			break;
		}
		//(gametype == 0) ? :
		gametype = gameTypeFilter();
	}

}

// Get a cell value, drop anything else.
char cellInputFilter() {
	char c = 0;
	std::cout << "Pick a cell (1-9): ";
	while ((c >= '1') ^ (c <= '9')) {
		std::cin >> c;
		//std::cout << c;
		//std::cout << ((c >= '1') & (c <= '9')) << '\n';
	};
	std::cout << "Entered " << c;

	return c - '0';
}

int gameTypeFilter() {
	char c = 0;
	bool cond = true;
	std::cout << "Gametype Choice: ";
	while (cond) {
		std::cin >> c;
		cond ^= true;
		switch (c) {
		case '0':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case 'q':
			return 99;
			//case 'y':
			//case 'Y':
			//	return 10;
			//case 'n':
			//case 'N':
			//	return 11;
			//case 'r':
			//case 'R':
			//	return 12;

		default:
			std::cout << "Invalid input.\n";
			cond ^= true;
		}
	};
	std::cout << "Entered " << c;

	return -1;
}

void gameLoop_Human(BitTacToe b, bool playerX) {
	int cellChoice;
	std::cout << "HUMAN VS HUMAN\n\n";
	std::cout << (playerX ? "X" : "O") << " plays first.\n";

	while (!b.evaluate()) {

		// Filter out any already taken cells
		do {
			cellChoice = cellInputFilter() - 1;
			if (!b.cellFull(cellChoice)) std::cout << " but that cell is full.\n";
		} while (!b.cellFull(cellChoice));

		// Apply and print changes
		b.humanMove(playerX, cellChoice);
		b.twoDeePrint();

		// Next player
		playerX ^= true;
	}
	std::cout << "\nPlayer " << (playerX ? 'X' : 'O') << " wins!\n";
}

void gameLoop_Synth(BitTacToe b, bool player, bool depthWeight) {
	int cellChoice;
	std::cout << "HUMAN VS AI\n\n";
	std::cout << (player ? "X" : "O") << " plays first.\n";

	while (!b.evaluate() & !b.gameFull()) {
		if (player) {
			b.flip_X(b.findBestMove(depthWeight));
			// Print changes
			b.twoDeePrint();
		}
		else {
			// Filter out any already taken cells
			do {
				cellChoice = cellInputFilter() - 1;
				if (!b.cellFull(cellChoice)) std::cout << " but that cell is full.\n";
			} while (!b.cellFull(cellChoice));

			// Apply changes
			b.humanMove(player, cellChoice);

		}

		// Next player
		player ^= true;
	}
	if (!b.evaluate()) {
		b.twoDeePrint();
		std::cout << "It's a tie!\n";
	}
	else {
		b.twoDeePrint();
		std::cout << "\nPlayer " << (~player ? 'X' : 'O') << " wins!\n";
	}
}