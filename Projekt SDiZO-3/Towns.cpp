#include "Towns.h"



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
	neighboursMatrix = new unsigned int*[towns_number];

	for (int i = 0; i < towns_number; i++)
	{
		neighboursMatrix[i] = new unsigned int[towns_number];

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

