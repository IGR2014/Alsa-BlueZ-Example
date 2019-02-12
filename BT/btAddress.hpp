#pragma once
#ifndef RDM_BLUETOOTH_ADDRESS_HPP
#define RDM_BLUETOOTH_ADDRESS_HPP


#include <cstdint>


// Bluetooth address class
class btAddress {

	union {

		struct {

			uint8_t	b1;
			uint8_t	b2;
			uint8_t	b3;
			uint8_t	b4;
			uint8_t	b5;
			uint8_t	b6;

                };

		uint8_t	b[6];

	};


public:

	// C-tor
	btAddress();
	// C-tor
	btAddress(const char* _buffer, unsigned int _size);
	// D-tor
	~btAddress();

	// Set BT address
	void		set(const uint8_t _b1, const uint8_t _b2, const uint8_t _b3, const uint8_t _b4, const uint8_t _b5, const uint8_t _b6);
	// Set BT address
	void		set(const char* _buffer, unsigned int _size);

	// Get BT address
	const char*	get(char* _buffer, unsigned int _size) const;


};


#endif  // RDM_BLUETOOTH_ADDRESS_HPP

