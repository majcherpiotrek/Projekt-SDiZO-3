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


	const int n = 6;
	const int b = 3;
	int tests = 100;

	long double bruteWynikiAvg[n][b];
	long double greedyWynikiAvg[n][b];

	int size_range = 20;
	int values_range = 20;
	int N[n] = { 5, 10, 15, 20, 25, 30 };
	double b_ratio[b] = { 0.25, 0.15, 0.10 };
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
			if (i > 3)
				tests = 2;
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


	const int n = 7;
	int tests = 10;

	long double bruteWynikiAvg[n];
	long double greedyWynikiAvg[n];

	int values_range = 20;
	int N_brute[n] = { 3, 5, 8, 10, 12, 14, 16 };
	int N[n] = { 5, 15, 25, 35, 40, 45, 50 };

	for (int i = 0; i < n; i++)
	{
			bruteWynikiAvg[i] = 0;
			greedyWynikiAvg[i] = 0;	
	}

	for (int i = 0; i < n; i++)
	{
		if (i > 4)
			tests = 2;
		std::cout << "Running tests for " << N_brute[i] << " towns for brute_force and " << N[i] <<" for greedy." << std::endl;
		for (int t = 0; t < tests; t++)
		{
			LARGE_INTEGER countStart, countEnd;
			//////// 100 tests //////////////
			RandGraphGen gen = *(new RandGraphGen());
			gen.generate(N[i], values_range);
			gen.saveToFile("voyager_greedy_input");
			
			gen.generate(N_brute[i], values_range);
			gen.saveToFile("voyager_greedy_input");


				Towns towns = *(new Towns());
				towns.loadTownsMap("voyager_greedy_input");

				/*pomiar*/
				countStart = startTimer();
				towns.greedy(0);
				countEnd = endTimer();
				/*pomiar*/

				greedyWynikiAvg[i] += (long double)(countEnd.QuadPart - countStart.QuadPart) / freq.QuadPart;

				towns.loadTownsMap("voyager_brute_input");
				/*pomiar*/
				countStart = startTimer();
				towns.brute_force();
				countEnd = endTimer();
				/*pomiar*/

				bruteWynikiAvg[i] += (long double)(countEnd.QuadPart - countStart.QuadPart) / freq.QuadPart;

				//////////////////////////////////
		}

			greedyWynikiAvg[i] = greedyWynikiAvg[i] / tests;
			bruteWynikiAvg[i] = greedyWynikiAvg[i]/ tests;
		

		std::fstream plik_greedy;
		std::fstream plik_brute;

		std::stringstream ssgreedy;
		std::stringstream ssbrute;

		std::string nazwa_greedy;
		std::string nazwa_brute;

		nazwa_greedy.append("voyager_greedy_");
		nazwa_brute.append("voyager_brute_");

		ssgreedy << "N_" << N[i] << ".txt";
		ssbrute << "N_" << N[i] << ".txt";

		nazwa_greedy.append(ssgreedy.str());
		nazwa_brute.append(ssbrute.str());

		plik_greedy.open(nazwa_greedy, std::ios::out);
		plik_brute.open(nazwa_brute, std::ios::out);

		for (int j = 0; j < n; j++)
		{
			plik_greedy << greedyWynikiAvg[j] << ";";
			plik_brute << bruteWynikiAvg[i] << ";";
		}

		plik_greedy.close();
		plik_brute.close();
	}
	///TESTING LOOP END
	return;
}

int main() {
	
	run_tests_knapsack();
	run_tests_voyager();
	return 0;
}