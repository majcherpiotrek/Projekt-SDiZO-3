#include "Knapsack.h"



void Knapsack::quickSort(Item * tab, int left, int right, bool ratio_mode)
{
	if (left >= right)
		return;

	int m = partition(tab, left, right, ratio_mode);
	quickSort(tab, left, m, ratio_mode);
	quickSort(tab, m + 1, right, ratio_mode);

	return;
}

int Knapsack::partition(Item * tab, int left, int right, bool ratio_mode)
{
	Item pivot = tab[left];
	int l = left, r = right;
	while (true) 
	{
		
		while ( ((ratio_mode) ? tab[l].ratio : tab[l].value)  > ((ratio_mode) ? pivot.ratio : pivot.value) ) l++;
		while ( ((ratio_mode) ? tab[r].ratio : tab[r].value) < ((ratio_mode) ? pivot.ratio : pivot.value)) r--;

		if (l < r)
		{
			Item buf = tab[l];
			tab[l] = tab[r];
			tab[r] = buf;
			l++;
			r--;
		}
		else
			return r;
	}
}

void Knapsack::packItem(Item _it)
{
	if (sack == nullptr)
		sack = new sackElement(_it);
	else
	{
		sackElement* ptr = sack;

		while (ptr->next != nullptr)
			ptr = ptr->next;

		ptr->next = new sackElement(_it);
	}
	
	return;
}

Knapsack::Knapsack()
{
	Knapsack(0);
}

Knapsack::Knapsack(int _capacity)
{
	capacity = _capacity;
	packed_weight = 0;
	packed_value = 0;
	sack = nullptr;
	itemsSet = nullptr;
}


Knapsack::~Knapsack()
{
	if (sack != nullptr)
		delete sack;
	if (itemsSet != nullptr)
		delete[] itemsSet;
}

void Knapsack::loadItemsSet(std::string fileName)
{
	/* Sprawdzam, czy w nazwie pliku brakuje rozszerzenia ".txt", je�li tak to je dodaj�.*/
	if (fileName[fileName.length() - 1] != *"t" ||
		fileName[fileName.length() - 2] != *"x" ||
		fileName[fileName.length() - 3] != *"t" ||
		fileName[fileName.length() - 4] != *".")
		fileName.append(".txt");

	/* Tworz� strumie� do odczytu danych z pliku.
	Otwieram go w trybie "std::ios::in", czyli do odczytu.*/
	std::fstream input;
	input.open(fileName, std::ios::in);

	/* Sprawdzam, czy powiod�o si� otwarcie pliku.*/
	if (!input.good())
		throw FileNotFoundException("\nNie ma takiego pliku!");

	std::string inputLine;

	getline(input, inputLine);

	std::istringstream* iss;
	iss = new std::istringstream(inputLine);

	/* Odczytanie linii danych zawieraj�cej informacje
	o pojemno�ci plecaka i liczebno�ci zbioru przedmiot�w do spakowania*/

	try {
		*iss >> this->capacity; //wczytanie pojemno�ci plecaka
		*iss >> this->itemsSet_size; // oraz liczebno�ci zbioru
	}
	catch (std::exception ex) {
		std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
		return;
	}
	delete iss;

	if (sack != nullptr) 
	{
		delete sack;
		sack = nullptr;
	}
		
	if (itemsSet != nullptr)
	{
		delete[] itemsSet;
		itemsSet = nullptr;
	}
		

	itemsSet = new Item[itemsSet_size];

	packed_value = 0;
	packed_weight = 0;

	for (int i = 0; i < itemsSet_size; i++)
	{
		getline(input, inputLine);

		iss = new std::istringstream(inputLine);

		try {
			*iss >> itemsSet[i].weight;
			*iss >> itemsSet[i].value;
		}
		catch (std::exception ex) {
			std::cerr << "Blad podczas wczytywania danych z pliku:\n" << ex.what();
			return;
		}

		itemsSet[i].ratio = (double)itemsSet[i].value / (double)itemsSet[i].weight;

		delete iss;
	}
	return;
}

void Knapsack::loadItemsSet(Item * items, unsigned int n)
{
	if (sack != nullptr)
	{
		delete sack;
		sack = nullptr;
	}

	if (itemsSet != nullptr)
	{
		delete[] itemsSet;
		itemsSet = nullptr;
	}

	itemsSet_size = n;
	itemsSet = new Item[itemsSet_size];

	packed_value = 0;
	packed_weight = 0;

	memcpy(itemsSet, items, itemsSet_size*sizeof(Item));

	return;
}

/*
Metoda realizuj�ca algorytm zach�anny dla problemu plecakowego.
Wylicza warto�� jednostkow� dla ka�dego elementu i wed�ug niej
ustawia je w kolejk�. Pakuje do plecaka kolejne elementy z kolejki,
dop�ki plecak nie zostanie zape�niony.*/
bool Knapsack::greedy_pack(bool ratio_mode)
{
	if (itemsSet == nullptr)
	{
		std::cout << "Blad! Najpierw musisz zaladowac dane elementow do zapakowania!";
		return false;
	}

	quickSort(itemsSet, 0, itemsSet_size-1, ratio_mode);

	
	bool elements_packed = false;
	
	for (int i = 0; i < itemsSet_size; i++)
	{
		if (packed_weight == capacity)
			break;
		
		if (itemsSet[i].weight <= capacity - packed_weight)
		{
			packItem(itemsSet[i]);
			packed_weight += itemsSet[i].weight;
			packed_value += itemsSet[i].value;
			elements_packed = true;
		}
	}
	return elements_packed;
}

bool Knapsack::brute_force_pack()
{
	if (itemsSet == nullptr)
	{
		std::cout << "Blad! Najpierw musisz zaladowac dane elementow do zapakowania!";
		return false;
	}

	if (itemsSet_size >= 64)
	{
		std::cout << "Blad! Zbyt wiele przedmiotow do zapakowania. Wiecej niz 2^63 - 1 mozliwosci dla algorytmu brute_force\nprzekracza zakres long long int'a!\n";
		return false;
	}
	unsigned long long int permutations = (1 << itemsSet_size);
	
	unsigned long long int currentBest = 0;
	int current_best_value = 0;
	int current_best_weight = 0;
	
	bool anything_fits = false;
	

	for (unsigned long long int i  = 0; i < permutations; i++) {
	/*
	Iterujemy po wszystkich mo�liwo�ciach. Ka�dy bit zmiennej
	permutations odpowiada jednemu elementowi zbioru przedmiot�w.
	Jedynka oznacza, �e do��czamy dany przedmiot, zero, �e nie.
	*/
		int sum_weight = 0;
		int sum_value = 0;
		
		bool fits = false;
		
		/*Przechodzimy po wszystkich przedmiotach, �eby sprawdzi�, czy nale�� do danej kombinacji*/
		for (int k = 0; k < itemsSet_size; k++)
		{
			unsigned long long int current_perm = i;
			/*
			Sprawdzamy przynale�no��
			-> musimy dosta� si� do k-tego bitu permutations, 
			wi�c trzeba wykona� k razy przesuni�cie bitowe 
			w prawo, aby otrzyma� ten bit na najni�szej pozycji
	
			Za pomoc� AND sprawdzamy, czy ten bit to 0 czy 1, por�wnuj�c go z 1
			(je�li b�dzie jeden to w wyniku bitowego iloczynu logicznego dostaniemy
			1, w innym wypadku zero)
			1000010101000000001
			0000000000000000001
			-------------------
			0000000000000000001
		
			Je�li r�wne 0 to znaczy, �e przedmiot nie nale�y do kombinacji i kontynuujemy p�tle*/
			if (((current_perm >> k) & 1) != 1)
				continue;

				

			sum_weight += itemsSet[k].weight;
			sum_value += itemsSet[k].value;
			
			if (sum_weight > this->capacity) 
			{
				fits = false;
				break;
			}

			fits = true;
		}
		
		/*Je�li suma wag przedmiot�w kombinacji wi�ksza ni� pojemno�� plecaka to sprawdzamy kolejn�*/
		if (!fits)
			continue;
		
		anything_fits = true;

		if (sum_value > current_best_value)
		{
			current_best_value = sum_value;
			current_best_weight = sum_weight;
			currentBest = i;
		}
	}

	if (!anything_fits)
		return false;

	/*Przechodzimy po wszystkich przedmiotach, �eby sprawdzi�, czy nale�� do danej kombinacji*/
	for (int k = 0; k < itemsSet_size; k++)
	{
		unsigned long long int current_perm = currentBest;
		
		if (((current_perm >> k) & 1) != 1)
			continue;

		/*Dodajemy przedmiot do rozwi�zania*/

		this->packItem(itemsSet[k]);
		packed_weight += itemsSet[k].weight;
		packed_value += itemsSet[k].value;
	}
	
	return true;
}

std::string Knapsack::toString()
{
	std::string text = "";
	std::ostringstream oss;
	
	sackElement* ptr = sack;
	if (ptr == nullptr)
	{
		text.append("\nPlecak jest pusty!");
		return text;
	}
	else {	
		
		text.append("\nPlecak (wartosc = ");
		
		oss << packed_value << ", wypelnienie " << packed_weight << "/" << capacity <<"):\n";
		text.append(oss.str());
		
		oss.str(std::string());


		while (ptr != nullptr)
		{
			oss << "{waga " << ptr->it.weight << ", wartosc " << ptr->it.value << "}\n";
			text.append(oss.str());

			oss.str(std::string());

			ptr = ptr->next;
		}
	}

	return text;
}

void Knapsack::saveToFile(std::string fileName)
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


std::ostream & operator<<(std::ostream & output, Knapsack & knapsack)
{
	output << knapsack.toString();
	return output;
}
