#include "CCoup.h"

CCoup::CCoup(CCoup::ETypeCoup type, int depuis) {
	this->type = type;
	this->depuis = depuis;
}

const CCoup::ETypeCoup& CCoup::getType(void) {
	return type;
}

const int& CCoup::getDepuis(void) {
	return depuis;
}
