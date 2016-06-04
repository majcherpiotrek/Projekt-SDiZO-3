#include "Knapsack.h"
#include "RandDataGen.h"

int main() {

	RandDataGen gen = *(new RandDataGen(50, 28, 30, 100));
	gen.saveToFile("plecaczek");

	Knapsack plecak = *(new Knapsack());
	try {
		plecak.loadItemsSet("plecaczek");
	}
	catch (std::runtime_error err) {
		std::cout << "BLAD! - " << err.what();
	}

	if (plecak.greedy_pack(true))
	{
		std::cout << plecak;
		plecak.saveToFile("plecaczek");
	}
	try {
		plecak.loadItemsSet("plecaczek");
	}
	catch (std::runtime_error err) {
		std::cout << "BLAD! - " << err.what();
	}

	if (plecak.brute_force_pack())
	{
		std::cout << plecak;
		plecak.saveToFile("plecaczek");
	}
	

	std::cin.get();
	return 0;
}