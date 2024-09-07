#ifndef MARKET_H
#define MARKET_H
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class market
{
public:
	std::ifstream inputfile;
	std::string line;   
	// std::vector<single_order<string,int>> list; 
	market(int argc, char** argv);
	void start();
private:
};
#endif
