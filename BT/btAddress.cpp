#include <cstdio>
#include "btAddress.hpp"


// C-tor
btAddress::btAddress() :
	b1(0),
	b2(0),
	b3(0),
	b4(0),
	b5(0),
	b6(0) {}

// C-tor
btAddress::btAddress(const char* _buffer, unsigned int _size) {

	set(_buffer, _size);

}

// D-tor
btAddress::~btAddress() {}


// Set BT address
void btAddress::set(const uint8_t _b1, const uint8_t _b2, const uint8_t _b3, const uint8_t _b4, const uint8_t _b5, const uint8_t _b6) {

	b[0]	= _b1;
	b[1]	= _b2;
	b[2]	= _b3;
	b[3]	= _b4;
	b[4]	= _b5;
	b[5]	= _b6;

}

// Set BT address
void btAddress::set(const char* _buffer, unsigned int _size) {

	sscanf(_buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]);

}


// Get BT address
const char* btAddress::get(char* _buffer, unsigned int _size) const {

	if (_size < 18) {

		return _buffer;

	} else {

		sprintf(_buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", b[0], b[1], b[2], b[3], b[4], b[5]);

		return _buffer;

	}

}

