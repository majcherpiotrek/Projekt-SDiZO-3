#include "Towns.h"



long long int Towns::silnia(int n)
{
	long long int result = n;
	
	for (int i = 0; i < n;i++)
		result = result*(n - i);
	
	return result;
}

Towns::Towns()
{
	neighboursMatrix = nullptr;
	towns_number = 0;
}

Towns::~Towns()
{
	if (neighboursMatrix != nullptr)
	{
		for (int i = 0; i < towns_number; i++)
		{
			if (neighboursMatrix[i] != nullptr)
				delete[] neighboursMatrix[i];
		}

		delete[] neighboursMatrix;
	}
}

void Towns::loadTownsMap(std::string fileName)
{
	/* Sprawdzam, czy w nazwie pliku brakuje rozszerzenia ".txt", jeœli tak to je dodajê.*/
	if (fileName[fileName.length() - 1] != *"t" ||
		fileName[fileName.length() - 2] != *"x" ||
		fileName[fileName.length() - 3] != *"t" ||
		fileName[fileName.length() - 4] != *".")
		fileName.append(".txt");

	/* Tworzê strumieñ do odczytu danych z pliku.
	Otwieram go w trybie "std::ios::in", czyli do odczytu.*/
	std::fstream input;
	input.open(fileName, std::ios::in);

	/* Sprawdzam, czy powiod³o siê otwarcie pliku.*/
	if (!input.good())
		throw FileNotFoundException("\nNie ma takiego pliku!");

	std::string inputLine;

	getline(input, inputLine);

	std::istringstream* iss;
	iss = new std::istringstream(inputLine);

	if (neighboursMatrix != nullptr)
	{
		for (int i = 0; i < towns_number; i++)
		{
			if (neighboursMatrix[i] != nullptr)
				delete[] neighboursMatrix[i];
		}

		delete[] neighboursMatrix;
	}

	/* Odczytanie linii danych zawieraj¹cej informacje
	o liczbie miast*/

	try {
		*iss >> this->towns_number; 
	}
	catch (std::exception ex) {
		std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
		return;
	}
	delete iss;

	

	/*Utworzenie nowej macierzy s¹siedztwa*/
	neighboursMatrix = new int*[towns_number];

	for (int i = 0; i < towns_number; i++)
	{
		neighboursMatrix[i] = new int[towns_number];

		getline(input, inputLine);

		iss = new std::istringstream(inputLine);

		/*Ka¿da linia zawiera informacje o odleg³oœciach z i-tego miasta
		do pozosta³ych miast */
		for (int k = 0; k < towns_number; k++)
		{
			
			try {
				*iss >> neighboursMatrix[i][k];
			}
			catch (std::exception ex) {
				std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
				return;
			}
		}

		delete iss;
	}
	return;
}

void Towns::loadTownsMap(int ** neigbours_matrix, int towns_num)
{
	if (neigbours_matrix == nullptr)
		return;
	
	if (neighboursMatrix != nullptr)
	{
		for (int i = 0; i < towns_number; i++)
		{
			if (neighboursMatrix[i] != nullptr)
				delete[] neighboursMatrix[i];
		}

		delete[] neighboursMatrix;
	}

	towns_number = towns_num;
	neighboursMatrix = new int*[towns_number];

	for (int i = 0; i < towns_number; i++)
	{
		neighboursMatrix[i] = new int[towns_num];
		memcpy(neighboursMatrix[i], neigbours_matrix[i], towns_number*sizeof(int));
	}
	
}

void Towns::saveToFile(std::string fileName)
{
	if (fileName[fileName.length() - 1] != *"t" ||
		fileName[fileName.length() - 2] != *"x" ||
		fileName[fileName.length() - 3] != *"t" ||
		fileName[fileName.length() - 4] != *".")
		fileName.append(".txt");
	std::fstream output;
	output.open(fileName, std::ios::app);

	output << *this;
	output.close();
	return;
}

int* Towns::greedy(int start)
{
	if (start < 0 || start >= towns_number)
		throw new IndexOutOfBoundsException("\nNie ma miasta o takim numerze!\n");

	/*Tablica odwiedzonych*/
	bool* visited = new bool[towns_number];
	for (int i = 0; i < towns_number; i++)
		visited[i] = false;

	/*Tablica na wynik*/
	int* result = new int[towns_number];
	for (int i = 0; i < towns_number; i++)
		result[i] = -1;

	visited[start] = true;
	result[0] = start;

	int currentTown = start;

	/*W co najwy¿ej N-1 krokach dostaniemy rozwi¹zanie*/
	for (int i = 1; i < towns_number; i++)
	{
		
		int shortest_dist = INT_MAX;
		int next_town = -1;

		for (int k = 0; k < towns_number; k++)
		{
			if (currentTown == k || visited[k] == true)
				continue;

			if (neighboursMatrix[currentTown][k] < shortest_dist)
			{
				next_town = k;
				shortest_dist = neighboursMatrix[currentTown][k];
			}
		}

		visited[next_town] = true;
		currentTown = next_town;
		result[i] = currentTown;
	}

	/*Sprawdzenie, czy wszystkie miasta odwiedzone*/
	for (int i = 0; i < towns_number; i++)
	{
		if (!visited[i])
			return nullptr;
	}

	return result;
}

int * Towns::brute_force()
{
	/*Liczba wszystkich permutacji zbioru miast*/
	long long permutations = silnia(towns_number);

	int* cities = new int[towns_number];
	for (int i = 0; i < towns_number; i++)
		cities[i] = i;
	int counter = 0;
	for (int i = 0; i < towns_number; i++)
	{
		;
	}

	
	return nullptr;
}


std::string Towns::toString()
{
	std::string text = "";
	std::ostringstream oss;

	if (neighboursMatrix == nullptr)
	{
		text.append("\nMapa odleglosci miedzy miastami jest pusta!\n");
		return text;
	}

	text.append("\nLiczba miast : ");
	oss << towns_number << "\nMapa odleglosci miedzy miastami :\n";
	
	text.append(oss.str());
	oss.str(std::string());

	for (int i = 0; i < towns_number+1; i++)
	{

		for (int k = 0; k < towns_number + 1; k++)
		{
			if (k == 0 && i == 0)
			{
				oss.width(4);
				oss << "nr\\nr" << std::right;
				continue;
			}

			if (i == 0 && k > 0)
			{
				oss.width(4);
				oss << k - 1 << ":" << std::right;
				continue;
			}

			if (k == 0)
			{
				oss.width(4);
				oss << i-1 << ":" << std::right;
				continue;
			}

			oss.width(5);
			oss << neighboursMatrix[i-1][k - 1] << std::right;

		}

		oss << std::endl;	
	}

	text.append(oss.str());
	return text;
}

std::ostream & operator<<(std::ostream & output, Towns & mapa)
{
	output << mapa.toString();
	return output;
}
