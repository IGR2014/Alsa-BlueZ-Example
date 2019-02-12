#include <cstring>

#include "btDevice.hpp"


// C-tor
btDevice::btDevice() :
        address(),
        name(nullptr),
        COD(0) {}

// C-tor
btDevice::btDevice(const btAddress _address, const char* _name, const unsigned int nameLen, const unsigned int _cod) {

        set(_address, _name, nameLen, _cod);

}

// D-tor
btDevice::~btDevice() {

        if (name) {

                delete [] name;
                name = nullptr;

        }

}


// Set address, name and COD
void btDevice::set(const btAddress _address, const char* _name, const unsigned int nameLen, const unsigned int _cod) {

        address = _address;

	if (name) {

        	delete [] name;

	}
        name = new char[nameLen];

        memcpy(name, _name, nameLen);

        COD     = _cod;

}


// Set address
void btDevice::setAddress(const btAddress _address) {

        address = _address;

}

// Set name
void btDevice::setName(const char* _name, const unsigned int nameLen) {

        if (name) {

        	delete [] name;

	}
        name = new char[nameLen];

        memcpy(name, _name, nameLen);

}

// Set address
void btDevice::setCOD(const unsigned int _cod) {

        COD = _cod;

}


// Get address
btAddress btDevice::getAddress() const {

        return address;

}

// Get name
const char* btDevice::getName() const {

        return name;

}

// Get COD
const unsigned int btDevice::getCOD() const {

        return COD;

}

