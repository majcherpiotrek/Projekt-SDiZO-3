#pragma once
#include <time.h>
#include <string>
#include<fstream>
class RandDataGen
{
private:

	unsigned int* weights;
	unsigned int * values;

	unsigned int knapsack_capacity;

	unsigned int items_number;
	unsigned int weights_range;
	unsigned int values_range;
public:
	RandDataGen(unsigned int _knapsack_capacity, unsigned int _items_number, unsigned int _weights_range, unsigned int _values_range);
	~RandDataGen();

	void generateNewItemsSet();
	void generateNewItemsSet(unsigned int _knapsack_capacity);
	void generateNewItemsSet(unsigned int _knapsack_capacity, unsigned int _items_number);
	void generateNewItemsSet(unsigned int _knapsack_capacity, unsigned int _items_number, unsigned int _weights_range, unsigned int _values_range);
	void saveToFile(std::string fileName);

};

