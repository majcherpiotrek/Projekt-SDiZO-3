#include "Knapsack.h"

#include "Towns.h"

#include <Windows.h>

LARGE_INTEGER startTimer()
{
	LARGE_INTEGER start;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return
		start;
}
LARGE_INTEGER endTimer()
{
	LARGE_INTEGER stop;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return
		stop;
}


void run_tests_knapsack() {

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);


	const int n = 5;
	const int b = 3;
	const int tests = 100;

	long double bruteWynikiAvg[n][b];
	long double greedyWynikiAvg[n][b];

	int size_range = 20;
	int values_range = 20;
	int N[n] = { 4, 18, 12, 16, 20 };
	double b_ratio[b] = { 0.30, 0.20, 0.10 };
	int B[n][b];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < b; j++)
		{
			B[i][j] = (int)N[i] * size_range * b_ratio[j];
			bruteWynikiAvg[i][j] = 0;
			greedyWynikiAvg[i][j] = 0;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < b; j++)
		{
			std::cout << "Running tests for " << N[i] << " elements and capacity " << B[i][j] << std::endl;
			for (int t = 0; t < tests; t++)
			{
				LARGE_INTEGER countStart, countEnd;
				//////// 100 tests //////////////
				RandKnapsackDataGen gen = *(new RandKnapsackDataGen(B[i][j], N[i], size_range, values_range));
				gen.saveToFile("knapsack_tests_input");

				Knapsack knapsack = *(new Knapsack());
				knapsack.loadItemsSet("knapsack_tests_input");

				/*pomiar*/
				countStart = startTimer();
				knapsack.greedy_pack(true);
				countEnd = endTimer();
				/*pomiar*/

				greedyWynikiAvg[i][j] += (long double)(countEnd.QuadPart - countStart.QuadPart)/freq.QuadPart;

				knapsack.loadItemsSet("knapsack_tests_input");

				/*pomiar*/
				countStart = startTimer();
				knapsack.brute_force_pack();
				countEnd = endTimer();
				/*pomiar*/

				bruteWynikiAvg[i][j] += (long double)(countEnd.QuadPart - countStart.QuadPart) / freq.QuadPart;
				//////////////////////////////////
			}

			greedyWynikiAvg[i][j] = greedyWynikiAvg[i][j] / tests;
			bruteWynikiAvg[i][j] = greedyWynikiAvg[i][j] / tests;
		}

		std::fstream plik_greedy;
		std::fstream plik_brute;

		std::stringstream ssgreedy;
		std::stringstream ssbrute;

		std::string nazwa_greedy;
		std::string nazwa_brute;

		nazwa_greedy.append("knapsack_greedy_");
		nazwa_brute.append("knapsack_brute_");

		ssgreedy << "N_" << N[i] << ".txt";
		ssbrute << "N_" << N[i] << ".txt";

		nazwa_greedy.append(ssgreedy.str());
		nazwa_brute.append(ssbrute.str());

		plik_greedy.open(nazwa_greedy, std::ios::out);
		plik_brute.open(nazwa_brute, std::ios::out);

		for (int j = 0; j < b; j++)
		{
			plik_greedy << greedyWynikiAvg[i][j] << ";";
			plik_brute << bruteWynikiAvg[i][j] << ";";
		}

		plik_greedy.close();
		plik_brute.close();
	}
	///TESTING LOOP END
	return;
}
void run_tests_voyager() {
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);


	const int n = 5;
	const int tests = 100;

	//long double bruteWynikiAvg[n];
	long double greedyWynikiAvg[n];

	int values_range = 20;
	int N[n] = { 4, 8, 12, 16, 20 };

	for (int i = 0; i < n; i++)
	{
			//bruteWynikiAvg[i] = 0;
			greedyWynikiAvg[i] = 0;	
	}

	for (int i = 0; i < n; i++)
	{
		
		std::cout << "Running tests for " << N[i] << " towns" << std::endl;
		for (int t = 0; t < tests; t++)
		{
			LARGE_INTEGER countStart, countEnd;
			//////// 100 tests //////////////
			RandGraphGen gen = *(new RandGraphGen());
			gen.generate(N[i], values_range);
			gen.saveToFile("voyager_tests_input");

				Towns towns = *(new Towns());
				towns.loadTownsMap("voyager_tests_input");

				/*pomiar*/
				countStart = startTimer();
				towns.greedy(0);
				countEnd = endTimer();
				/*pomiar*/

				greedyWynikiAvg[i] += (long double)(countEnd.QuadPart - countStart.QuadPart) / freq.QuadPart;

				//////////////////////////////////
		}

			greedyWynikiAvg[i] = greedyWynikiAvg[i] / tests;
			//bruteWynikiAvg[i] = greedyWynikiAvg[i]/ tests;
		

		std::fstream plik_greedy;
		//std::fstream plik_brute;

		std::stringstream ssgreedy;
		//std::stringstream ssbrute;

		std::string nazwa_greedy;
		//std::string nazwa_brute;

		nazwa_greedy.append("voyager_greedy_");
		//nazwa_brute.append("voyager_brute_");

		ssgreedy << "N_" << N[i] << ".txt";
		//ssbrute << "N_" << N[i] << ".txt";

		nazwa_greedy.append(ssgreedy.str());
		//nazwa_brute.append(ssbrute.str());

		plik_greedy.open(nazwa_greedy, std::ios::out);
		//plik_brute.open(nazwa_brute, std::ios::out);

		for (int j = 0; j < n; j++)
		{
			plik_greedy << greedyWynikiAvg[j] << ";";
			//plik_brute << bruteWynikiAvg[i][j] << ";";
		}

		plik_greedy.close();
		//plik_brute.close();
	}
	///TESTING LOOP END
	return;
}
int main() {
	/*
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
	*/
	run_tests_knapsack();
	run_tests_voyager();
	return 0;
}