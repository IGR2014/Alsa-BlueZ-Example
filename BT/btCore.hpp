#pragma once
#ifndef RDM_BLUETOOTH_CORE_HPP
#define RDM_BLUETOOTH_CORE_HPP


#include <list>

#include "btAddress.hpp"
#include "btDevice.hpp"


// Bluetooth core operations class
class btCore {

	int			dev_id;

	std::list<btDevice>*	devices;


public:

	// C-tor
	btCore();
	// D-tor
	~btCore();

	// Setup device
	bool			setup(const char* name, const unsigned int nameLen, const unsigned int cod);

	// Inquiry devices
	std::list<btDevice>*	inquiry(const unsigned int time);

	// Connect to device
	bool			connect(const btAddress address);
	// Disconnect from device
	bool			disconnect();

	// Browse SDP services
	bool			serviceBrowse(const btAddress address, const unsigned int service);
	// Register service
	bool			serviceRegister(const unsigned int service);
	// Unregister service
	bool			serviceUnregister(const unsigned int service);


};


#endif  // RDM_BLUETOOTH_CORE_HPP

