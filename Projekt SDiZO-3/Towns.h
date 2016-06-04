#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include "FileNotFoundException.h"
#include "RandGraphGen.h"

class Towns
{
private:

	
	int towns_number;

	int** neighboursMatrix;
	
	

public:
	
	Towns();
	~Towns();

	void loadTownsMap(std::string fileName);
	void loadTownsMap(int** neigbours_matrix, int towns_num);
	void saveToFile(std::string fileName);

	int* greedy(int start);
	
	std::string toString();
	friend std::ostream & operator << (std::ostream & output, Towns & mapa);
};

