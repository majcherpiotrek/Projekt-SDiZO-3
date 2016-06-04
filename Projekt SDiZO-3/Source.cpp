#include "Knapsack.h"

#include "Towns.h"

int main() {

	RandKnapsackDataGen gen = *(new RandKnapsackDataGen(40, 10, 30, 100));
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
	
	RandGraphGen graphgen = *(new RandGraphGen());
	Towns miasta = *(new Towns());
	miasta.loadTownsMap(graphgen.generate(5, 10), 5);
	graphgen.saveToFile("mapa1");
	
	std::cout << std::endl << std::endl;
	std::cout << miasta;
	miasta.saveToFile("mapa");

	miasta.loadTownsMap("mapa1");
	std::cout << std::endl << std::endl;
	std::cout << miasta;
	miasta.saveToFile("mapa1");

	int* droga = miasta.greedy(0);

	if (droga)
	{
		std::cout << std::endl;
		for (int i = 0; i < 5; i++)
			std::cout << droga[i] << std::endl;
	}

	std::cin.get();
	return 0;
}