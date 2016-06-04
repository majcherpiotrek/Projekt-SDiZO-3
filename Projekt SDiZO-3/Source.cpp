#include "Knapsack.h"
#include "RandDataGen.h"
#include "Towns.h"

int main() {

	RandDataGen gen = *(new RandDataGen(40, 10, 30, 100));
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
	
	Towns miasta = *(new Towns());
	miasta.loadTownsMap("mapa");

	std::cout << std::endl << std::endl;
	std::cout << miasta.toString();

	std::cin.get();
	return 0;
}