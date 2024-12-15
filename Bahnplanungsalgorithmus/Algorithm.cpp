#include "Algorithm.h"

Algorithm::Algorithm(const std::string & name, const std::string & language): name(name), language(language) {
	

}

const std::string& Algorithm::getName() const { return name; }
const std::string& Algorithm::getLanguage() const { return language; }

	// check if json exist
	// check for json with algorithm = name
	// for json with algorithm = name: compare maps? compare paths?
	// if maps / paths dont match: error
	// if maps / paths match: copy map into class, copy path into class
