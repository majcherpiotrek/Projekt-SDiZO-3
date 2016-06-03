#include "Knapsack.h"



void Knapsack::quickSort(Item * tab, int left, int right)
{
	if (left >= right)
		return;

	int m = partition(tab, left, right);
	quickSort(tab, left, m);
	quickSort(tab, m + 1, right);

	return;
}

int Knapsack::partition(Item * tab, int left, int right)
{
	Item pivot = tab[left];
	int l = left, r = right;
	while (true) 
	{
		
		while ( tab[l].ratio > pivot.ratio ) l++;
		while (tab[r].ratio < pivot.ratio) r--;

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
}


Knapsack::~Knapsack()
{
	if (sack != nullptr)
		delete[] sack;
}

/*
Metoda realizuj¹ca algorytm zach³anny dla problemu plecakowego.
Wylicza wartoœæ jednostkow¹ dla ka¿dego elementu i wed³ug niej
ustawia je w kolejkê. Pakuje do plecaka kolejne elementy z kolejki,
dopóki plecak nie zostanie zape³niony.*/
bool Knapsack::greedy_pack(Item * items, unsigned int n)
{
	Item* toPack = new Item[n];

	memcpy(toPack, items, n*sizeof(Item));

	quickSort(toPack, 0, n - 1);

	
	bool elements_packed = false;
	
	for (int i = 0; i < n; i++)
	{
		if (packed_weight == capacity)
			break;
		
		if (toPack[i].weight <= capacity - packed_weight)
		{
			packItem(toPack[i]);
			packed_weight += toPack[i].weight;
			packed_value += toPack[i].value;
			elements_packed = true;
		}
	}
	delete[] toPack;
	return elements_packed;
}

std::string Knapsack::toString()
{
	std::string text = "";
	std::ostringstream oss;
	
	sackElement* ptr = sack;
	if (ptr == nullptr)
	{
		text.append("Plecak jest pusty!");
		return text;
	}
	else {	
		
		text.append("Plecak (Wartosc = ");
		
		oss << packed_value << ", " << packed_weight << "/" << capacity <<"):\n";
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


std::ostream & operator<<(std::ostream & output, Knapsack & knapsack)
{
	output << knapsack.toString();
	return output;
}
