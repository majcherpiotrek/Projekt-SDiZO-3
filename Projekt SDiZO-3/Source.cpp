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

	LARGE_INTEGER countStart, countEnd;

	const int n = 5;
	const int b = 3;
	int tests = 100;

	long double bruteWynikiAvg[n][b];
	long double greedyWynikiAvg[n][b];

	int size_range = 50;
	int values_range = 50;
	int N[n] = { 5, 10, 20, 25, 30 };
	double b_ratio[b] = { 0.50, 0.25, 0.10 };
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
			RandKnapsackDataGen gen = *(new RandKnapsackDataGen(B[i][j], N[i], size_range, values_range));
			gen.saveToFile("knapsack_tests_input");

			Knapsack knapsack = *(new Knapsack());
			
			if (i == 4)
				tests = 5;
			std::cout << "\nRunning tests for " << N[i] << " elements and capacity " << B[i][j] << std::endl;
			for (int t = 0; t < tests; t++)
			{
				std::cout << ". ";
				//////// 100 tests //////////////
				
				knapsack.loadItemsSet("knapsack_tests_input");
				bool wynik;
				/*pomiar*/
				countStart = startTimer();
				wynik = knapsack.greedy_pack(true);
				countEnd = endTimer();
				/*pomiar*/
				if (!wynik) {
					throw new IndexOutOfBoundsException("Incorrect result for knapsack greedy.");
				}
				else
					greedyWynikiAvg[i][j] += (long double)(countEnd.QuadPart - countStart.QuadPart);

				knapsack.loadItemsSet("knapsack_tests_input");
				/*pomiar*/
				countStart = startTimer();
				wynik = knapsack.brute_force_pack();
				countEnd = endTimer();
				/*pomiar*/
				if (!wynik) {
					throw new IndexOutOfBoundsException("Incorrect result for salesman brute.");
				}
				else
					bruteWynikiAvg[i][j] += (long double)(countEnd.QuadPart - countStart.QuadPart);
				
				//////////////////////////////////
			}

			greedyWynikiAvg[i][j] = greedyWynikiAvg[i][j] / (tests*freq.QuadPart);
			bruteWynikiAvg[i][j] = bruteWynikiAvg[i][j] / (tests*freq.QuadPart);
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
	
	LARGE_INTEGER countStart, countEnd;

	const int n = 6;
	int tests = 1;

	long double bruteWynikiAvg[n];
	long double greedyWynikiAvg[n];
	long double greedyWynikiAvg2[n];

	int values_range = 20;
	int N_brute[n] = {  5, 8, 10, 11, 12, 13 };
	int N[n] = { 5, 50, 100, 500, 1000, 10000 };

	for (int i = 0; i < n; i++)
	{
			bruteWynikiAvg[i] = 0;
			greedyWynikiAvg[i] = 0;	
			greedyWynikiAvg2[i] = 0;
	}

	RandGraphGen gen = *(new RandGraphGen());
	Towns towns = *(new Towns());

	for (int i = 0; i < n; i++)
	{
		if (i  == 4)
			tests = 10;
		if (i == 5)
			tests = 1;
		
		std::cout << "\nRunning tests for " << N_brute[i] << " towns for brute_force and " << N[i] <<" for greedy." << std::endl;
		
		gen.generate(N[i], values_range);
		gen.saveToFile("voyager_greedy_input");

		gen.generate(N_brute[i], values_range);
		gen.saveToFile("voyager_brute_input");
		
		

		for (int t = 0; t < tests; t++)
		{
				std::cout << ". ";
			//////// 100 tests //////////////
				towns.loadTownsMap("voyager_greedy_input");
				int* wynik;
				/*pomiar*/
				countStart = startTimer();
				wynik = towns.greedy(0);
				countEnd = endTimer();
				/*pomiar*/
				if (!wynik) {
					throw new IndexOutOfBoundsException("Incorrect result for salesman greedy.");
				}
				else
					greedyWynikiAvg[i] += (long double)(countEnd.QuadPart - countStart.QuadPart);

				delete[] wynik;

				/*pomiar*/
				countStart = startTimer();
				wynik = towns.greedy(0);
				countEnd = endTimer();
				/*pomiar*/
				if (!wynik) {
					throw new IndexOutOfBoundsException("Incorrect result for salesman greedy.");
				}
				else
					greedyWynikiAvg2[i] += (long double)(countEnd.QuadPart - countStart.QuadPart);

				delete[] wynik;
				towns.loadTownsMap("voyager_brute_input");
				/*pomiar*/
				countStart = startTimer();
				wynik = towns.brute_force();
				countEnd = endTimer();
				
				/*pomiar*/
				if (!wynik) {
					throw new IndexOutOfBoundsException("Incorrect result for salesman brute.");
				}
				else
					bruteWynikiAvg[i] += (long double)(countEnd.QuadPart - countStart.QuadPart);

				//////////////////////////////////
		}

			greedyWynikiAvg[i] = greedyWynikiAvg[i] /( tests * freq.QuadPart);
			greedyWynikiAvg2[i] = greedyWynikiAvg2[i] / (tests * freq.QuadPart);

			bruteWynikiAvg[i] = bruteWynikiAvg[i]/ ( tests * freq.QuadPart );
			
	}
		
		std::fstream plik_greedy;
		std::fstream plik_brute;

		std::string nazwa_greedy;
		std::string nazwa_brute;

		nazwa_greedy.append("voyager_greedy.txt");
		nazwa_brute.append("voyager_brute.txt.");

		plik_greedy.open(nazwa_greedy, std::ios::out);
		plik_brute.open(nazwa_brute, std::ios::out);

		for (int j = 0; j < n; j++)
		{
			plik_greedy << greedyWynikiAvg[j] << ";";
			plik_brute << bruteWynikiAvg[j] << ";";
		}

		plik_greedy << std::endl;
		for (int j = 0; j < n; j++)
			plik_greedy << greedyWynikiAvg2[j] << ";";

		plik_greedy.close();
		plik_brute.close();
	///TESTING LOOP END
	return;
}

int main() {

	try {
		run_tests_knapsack();
	}
	catch (std::runtime_error err) {
		std::cout << "Knapsack problem tests failed...\n";
	}
	std::cout << "\n\ndone!\n";

	std::cin.get();
	return 0;
}