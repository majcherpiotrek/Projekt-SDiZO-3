#pragma once
#include <iostream>
#include <sstream>

struct Item {
	unsigned int weight;
	unsigned int value;
	double ratio;
};
class Knapsack
{
private:

	class sackElement {
	public:
		Item it;
		sackElement* next;

		sackElement(Item _it) {
			it = _it;
			next = nullptr;
		}
		~sackElement() {}
	};
	unsigned int capacity;
	int packed_weight;
	int packed_value;
	sackElement* sack;

	void quickSort(Item* tab, int left, int right);
	int partition(Item* tab, int left, int right);
	void packItem(Item _it);
public:
	Knapsack();
	Knapsack(int _capacity);
	~Knapsack();

	bool greedy_pack(Item *items, unsigned int n);
	
	std::string toString();
	friend std::ostream & operator << (std::ostream & output, Knapsack & knapsack);

};

