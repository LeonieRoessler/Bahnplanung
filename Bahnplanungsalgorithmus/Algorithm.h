#pragma once
#include <iostream>
#include "ParseJson.h"

using namespace std; 
class Algorithm
{
public:
	Algorithm(const std::string& name, const std::string& language);
	const std::string& getName() const;
	const std::string& getLanguage() const;

private:
	string name;
	string language;
};

