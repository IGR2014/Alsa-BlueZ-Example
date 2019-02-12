#pragma once
#ifndef RDM_BLUETOOTH_DEVICE_HPP
#define RDM_BLUETOOTH_DEVICE_HPP


#include "btAddress.hpp"


// Bluetooth device class
class btDevice {

	btAddress		address;	// Device address
	char*			name;		// Device name
	unsigned int		COD;		// Device COD


public:

	// C-tor
	btDevice();
	// C-tor
	btDevice(const btAddress _address, const char* _name, const unsigned int nameLen, const unsigned int _cod);
	// D-tor
	~btDevice();

	// Set address, name and COD
	void			set(const btAddress _address, const char* _name, const unsigned int nameLen, const unsigned int _cod);

	// Set address
	void			setAddress(const btAddress _address);
	// Set name
	void			setName(const char* _name, const unsigned int nameLen);
	// Set address
	void			setCOD(const unsigned int _cod);

	// Get address
	btAddress		getAddress() const;
	// Get name
	const char*		getName() const;
	// Get COD
	const unsigned int	getCOD() const;


};


#endif  // RDM_BLUETOOTH_DEVICE_HPP

