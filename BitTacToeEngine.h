#pragma once

#include <iostream>
#include <bitset>
#include <array>
//#include <random>
//#include <functional>
#include <time.h>

class BitTacToe
{
public:
	BitTacToe();
	~BitTacToe();

	void twoDeePrint();
	int evaluate();
	void humanMove(bool isMax, int cellToFlip);

	//int minimax(bool isMax);
	int minimax(bool isMax, int depth, bool depthWeight, int alpha = -1000, int beta = 1000);

	std::bitset<9> findBestMove(bool depthWeight);



	// Below, in the private section, you can read the justification of using bitsets over 2d arrays

	// reset game data to zero
	void reset() { mask = 0; data = 0; }

	// Use XOR to flip the bits, which assigns or undoes a move
	void flip_X(std::bitset<9> b) { mask ^= b; data ^= b; };
	void flip_O(std::bitset<9> b) { mask ^= b; };
	void flip_X(int i) { std::bitset<9> b(1); b <<= i; flip_X(b); };	// ints must be zero indexed
	void flip_O(int i) { std::bitset<9> b(1); b <<= i; flip_O(b); };

	// Full gamespace condition (one liner, but readable)
	bool gameFull() { return (mask == 0b111111111); }
	bool cellFull(std::bitset<9> b) { return b == (~mask & b); };
	bool cellFull(int i) { std::bitset<9> b(1); b <<= i; return cellFull(b); };

private:
	/* Why use bits?
	Tic Tac Toe has a 3x3 array of 3 values, with 8 victory conditions of any one row, column or diagonal being of one player
	The victory condition is calculated by comparing the preset victory condition with the player data
		Here's an example:	111
							000
							000
		It can be flattened like so:	111 000 000
		And then converted into a bitset	0b111000000	(it can also be read as an int, '448')
	This format opens up the possibility of using boolean operations directly, eliminating complicated, error prone if statements*/

	// 8 victory conditions
	std::array<std::bitset<9>, 8> eval{ 0b000000111, 0b000111000, 0b111000000,
								0b001001001, 0b010010010, 0b100100100,
								0b100010001, 0b001010100 };

	/* Now there is an issue of the gamespace, the three values can't be a bitset.
	This is easily solved by having two bitsets, a mask and the player data
		0		0		Unfilled	1		0	0
		1		0		'O' entry	0		0	1
		0		1		null		1		0	0
		1		1		'X' entry	0		1	0
		mask	data				move	X	O
									!mask	m&d	m&~d
									evaluation
	*/

	// 2x bitmaps as bitsets
	std::bitset<9> mask, data;

};