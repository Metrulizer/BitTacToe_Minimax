#include "BitTacToeEngine.h"

void BitTacToe::twoDeePrint() {
	std::cout << '\n';

	for (int i = 0; i <= 8; i++) {
		char c = (mask[i] & data[i]) ? 'X' : (mask[i] & ~data[i]) ? 'O' : '_';
		std::cout << c;
		if ((i + 1) % 3 == 0) std::cout << '\n';
	}
}

int BitTacToe::evaluate() {
	/*	Given that mask & data returns the cells marked 'x', and mask & ~data returns cells marked 'o'
		And that val is any one eval, where eval is the dataset of victory conditions
			The AND of val and marked cells returns val if marked cells contains a victory condition */
	for (auto& val : eval) {
		//	Return utility score given victory
		if (val == (val & mask & data)) return 100;
		else if (val == (val & mask & ~data)) return -100;
	}
	// Return utility score given zero victory
	return 0;
}

void BitTacToe::humanMove(bool isX, int cellToFlip) {
	std::cout << " for player " << (isX ? "X." : "O.");
	isX ? flip_X(cellToFlip) : flip_O(cellToFlip);
}

int BitTacToe::minimax(bool isMax, int depth, bool depthWeight, int alpha, int beta) {

	// ===== ======= ======= ======= ======= ======= ======= =======
	// If the game has been won or drawn, send the score back
	int score = evaluate();
	if (score != 0) return score;
	if (gameFull()) return 0;
	// ===== ======= ======= ======= ======= ======= ======= =======

	// ===== ======= ======= ======= ======= ======= ======= =======
	// Calculate the Minimax
	if (isMax)
	{
		// - ------- ------- ------- ------- ------- -------
		// The maximizer starts low and tries to find a best move. Both if bodies have the same structure, but redundancy removal is more effort than it's worth.
		int best = -1000;

		std::bitset<9> bitsX = 0b0000000001;
		while (bitsX.any())
		{
			// if that cell is unfilled
			if (cellFull(bitsX))
			{
				flip_X(bitsX);
				best = std::max(best, minimax(!isMax, depth + 1, depthWeight, alpha, beta));
				alpha = std::max(alpha, best);
				//twoDeePrint();
				//std::cout << best << '\n';
				flip_X(bitsX);
			}
			if (beta <= alpha) {
				//std::cout << beta << " under or equals " << alpha << '\n';
				break;
			}
			//else std::cout << "unbroken\n";

			bitsX <<= 1;
		}
		return best + (depthWeight ? -depth : 0);	// with depth, maximizer gets a penalty for delays
		// - ------- ------- ------- ------- ------- -------
	}
	else
	{
		// - ------- ------- ------- ------- ------- -------
		// inverse of above
		int best = 1000;

		std::bitset<9> bitsO = 0b0000000001;
		while (bitsO.any())
		{
			if (cellFull(bitsO))
			{
				flip_O(bitsO);
				best = std::min(best, minimax(!isMax, depth + 1, depthWeight, alpha, beta));
				beta = std::min(beta, best);
				flip_O(bitsO);
			}
			if (beta <= alpha) {
				//std::cout << beta << " under or equals " << alpha << '\n';
				break;
			}
			//else std::cout << "unbroken\n";

			bitsO <<= 1;
		}
		return best + (depthWeight ? 0 : 0);	// with depth, penalty isn't applied because that's the maximizer's perogative
	}

	return 0;
}

std::bitset<9> BitTacToe::findBestMove(bool depthWeight) {
	std::bitset<9> bits(0b000000001);
	//std::cout << "mask = " << mask << " data = " << data;
	twoDeePrint();
	std::bitset<9> bestBitSet;
	int bestValue = -1000;
	while (bits.any()) {
		if ((mask & bits) == 0) {
			// Get value at unfilled cell
			flip_X(bits);
			int value = minimax(false, 0, depthWeight);
			flip_X(bits);

			if (value > bestValue) {
				bestBitSet = bits;
				bestValue = value;
			}

			//std::cout << bits << '\n';
			std::cout << "bitset = " << bits << " value = " << value << '\n';
			//std::cout << "bbs = " << bestBitSet << " bv = " << bestValue << '\n';
		}
		bits <<= 1;
	}

	//std::cout << "best value is " << bestValue << "\n\n";
	return bestBitSet;
}


BitTacToe::BitTacToe()
{
	std::cout << "debug log, please ignore\n";
	mask = 0b001001001;
	data = 0b001001001;

	std::cout << "mask is " << mask << '\n';
	std::cout << "data is " << data << '\n';
	std::cout << "board value = " << evaluate() << '\n';

	data = 0b010010000;

	std::cout << "mask is " << mask << '\n';
	std::cout << "data is " << data << '\n';
	std::cout << "board value = " << evaluate() << '\n';


	mask = 0b111111110;
	std::cout << "gameover = " << gameFull() << " in " << mask << '\n';

	mask = 0b111111111;
	std::cout << "gameover = " << gameFull() << " in " << mask << '\n';

	std::cout << "gameover = " << gameFull() << " in " << mask << '\n';


	mask = 0b111111000;
	data = 0b101001000;

	twoDeePrint();
	flip_X(0b000000001);
	twoDeePrint();
	flip_X(0b000000001);
	twoDeePrint();

	flip_O(0b000000001);
	twoDeePrint();
	flip_O(0b000000001);
	twoDeePrint();

	std::cout << "flip test done, minimax time" << '\n';

	int bestVal = -1000;
	minimax(!false, 0, false);


	mask = 0b111111000;
	data = 0b101001000;

	std::cout << findBestMove(false) << '\n';

	mask = 0b000000000;
	data = 0b000000000;

	//std::cout << findBestMove() << '\n';


	reset();

	std::cout << "debug complete, ready to play\n\n";


}

BitTacToe::~BitTacToe()
{
}

