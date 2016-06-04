#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

#include "FileNotFoundException.h"

class Towns
{
private:

	
	unsigned int towns_number;

	unsigned int** neighboursMatrix;
	
	

public:
	
	Towns();
	~Towns();

	void loadTownsMap(std::string fileName);
	std::string toString();
};

