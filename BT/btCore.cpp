#include <iostream>

#include <unistd.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/l2cap.h>

#include "btCore.hpp"
#include "btAddress.hpp"
#include "btDevice.hpp"


// C-tor
btCore::btCore() {

	dev_id = hci_get_route(nullptr);
	devices = new std::list<btDevice>;

}

// D-tor
btCore::~btCore() {

	hci_close_dev(dev_id);

	if (devices) {

		delete devices;
		devices = nullptr;

	}

}


// Setup device
bool btCore::setup(const char* name, const unsigned int nameLen, const unsigned int cod) {

	// Open local HCI device
	int sk		= hci_open_dev(dev_id);
	if (sk < 0) {

		std::cerr << "HCI open device:\t\t" << strerror(errno) << std::endl;
		return false;

	}

	// Set our device name
	hci_write_local_name(sk, name, nameLen);
	// Setup our device as Audio rendering device (source)
	hci_write_class_of_dev(sk, cod, 0);

	/*
	std::cout << std::endl;
	std::cout << "HCI Device name set to:\t\"" << localName << "\"" << std::endl;
	*/

	close(sk);

        return true;

}


// Inquiry devices
std::list<btDevice>* btCore::inquiry(const unsigned int _time) {

        int		max_rsp		= 255;
        int		flags		= IREQ_CACHE_FLUSH;

	char		addr[19]	= {0};
	char		name[248]	= {0};
	uint8_t		cod[3]		= {0};

        devices->clear();

        inquiry_info* ii = new inquiry_info[max_rsp * sizeof(inquiry_info)];

	/*
	std::cout << std::endl;
	std::cout << "HCI Inquiry started..." << std::endl;
	*/

       	// Open local HCI device
	int sk		= hci_open_dev(dev_id);
	if (sk < 0) {

		std::cerr << "HCI open device:\t\t" << strerror(errno) << std::endl;

	}

	// Search devices
	int num_rsp = hci_inquiry(dev_id, _time, max_rsp, nullptr, &ii, flags);
	if (num_rsp < 0) {

		std::cerr << "HCI inquiry: " << strerror(errno) << std::endl;
		//delete [] ii;
		//ii = nullptr;
		//close(sk);

	}

	/*
	std::cout << std::endl;
	std::cout << "=================================================================================" << std::endl;
	std::cout << "| #\t" << "| BTA\t\t\t" << "| Name\t\t\t\t" << "| COD\t\t|" << std::endl;
	std::cout << "---------------------------------------------------------------------------------" << std::endl;
	*/

    	for (int i = 0; i < num_rsp; ++i) {

		ba2str(&ii[i].bdaddr, addr);
		memset(name, 0, sizeof(name));

		// Get device name
		if (hci_read_remote_name(sk, &ii[i].bdaddr, sizeof(name), name, 0) < 0) {

			strcpy(name, "[unknown]");

		}

		// Get class of device
		hci_read_class_of_dev(sk, cod, 0);

		/*
		std::cout << "| " << std::setw(2) << std::setfill(' ') << std::left << i + 1 << std::right << "\t";
		std::cout << "| " << addr << "\t";
		std::cout << "| " << std::setw(30) << std::left << name << std::right;
		std::cout << "| " << "0x";
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(cod[2]);
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(cod[1]);
		std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(cod[0]);
		std::cout << "\t|" << std::endl;
		*/

                devices->emplace_back(btAddress(addr, 18), name, strlen(name), (cod[2] << 16) | cod[1] << 8 | cod[0]);

	}

	/*
	std::cout << "=================================================================================" << std::endl;
	std::cout << std::endl;
	*/

	close(sk);

        delete [] ii;
        ii = nullptr;

        return devices;

}


// Connect to device
bool btCore::connect(const btAddress address) {

	/*
        std::cout << "Connecting to device\t" << userChoise << " ..." << std::endl;
	std::cout << std::endl;
	*/

        uint16_t	handle;
	unsigned int	ptype           = HCI_DM1 | HCI_DM3 | HCI_DM5 | HCI_DH1 | HCI_DH3 | HCI_DH5;

        char		addr[19]	= {0};

        bdaddr_t        bdaddr;

        str2ba(address.get(addr, 18), &bdaddr);

	// Open local HCI device
	int sk		= hci_open_dev(dev_id);
	if (sk < 0) {

		std::cerr << "HCI open device:\t\t" << strerror(errno) << std::endl;
		return false;

	}

	if (hci_create_connection(sk, &bdaddr, htobs(ptype), 0, 0, &handle, 0) < 0) {

		std::cerr << "HCI create connection:\t" << strerror(errno) << std::endl;
		close(sk);
                return false;

	} else {

		std::cout << "Connection:\t\tOK" << std::endl;
		
	}

	if (hci_authenticate_link(sk, handle, 0) < 0) {

		std::cerr << "HCI authenticate connection:\t" << strerror(errno) << std::endl;
		close(sk);
                return false;
		
	} else {

		std::cout << "Authentication:\t\tOK" << std::endl;
		
	}

	if (hci_encrypt_link(sk, handle, 1, 0) < 0) {

		std::cerr << "HCI encrypt connection:\t" << strerror(errno) << std::endl;
		close(sk);
                return false;
		
	} else {

		std::cout << "Encryption:\t\tOK" << std::endl;
		
	}

	close(sk);

        return true;

}

// Disconnect from device
bool btCore::disconnect() {}


// Browse SDP services
bool btCore::serviceBrowse(const btAddress address, const unsigned int service) {

        bdaddr_t        my_bdaddr_any	= {0, 0, 0, 0, 0, 0};
	bdaddr_t	target;

        char		addr[19]	= {0};

        str2ba(address.get(addr, 18), &target);

	uuid_t		svc_uuid_1;

	sdp_list_t*	response_list	= nullptr;
	sdp_list_t*	search_list	= nullptr;
	sdp_list_t*	attrid_list	= nullptr;
	sdp_session_t*	session		= nullptr;

	uint32_t	range		= 0x0000ffff;

	uint8_t		l2capPort	= 0;

	// Connect to the SDP server running on the remote machine
	session = sdp_connect(&my_bdaddr_any, &target, SDP_RETRY_IF_BUSY);
	sdp_uuid32_create(&svc_uuid_1, AUDIO_SINK_PROFILE_ID);
	search_list = sdp_list_append(search_list, &svc_uuid_1);
	attrid_list = sdp_list_append(attrid_list, &range);

	// Get a list of service records that have required UUID
	response_list = nullptr;

	int status = sdp_service_search_attr_req(session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
	if (status != 0) {

		std::cerr << "SDP service search:\t" << strerror(errno) << std::endl;

	}
	std::cout << "Service search:\t\tOK" << std::endl;

	sdp_list_t* proto_list	= nullptr;
	sdp_list_t* r		= response_list;

	// Go through each of the service records 
	for (; r; r = r->next) {

		sdp_record_t* rec = reinterpret_cast<sdp_record_t*>(r->data);

		// Get a list of the protocol sequences
		if (sdp_get_access_protos(rec, &proto_list) == 0) {

			std::cout << "Audio profile found:\t0x" << std::hex << rec->handle << std::dec << "\t";

			if ((l2capPort	= sdp_get_proto_port(proto_list, L2CAP_UUID)) != 0) {

				std::cout << "psm: " << static_cast<unsigned int>(l2capPort) << std::endl;

			}

			sdp_list_free(proto_list, nullptr);

		}

		sdp_record_free(rec);

	}

	sdp_list_free(response_list, nullptr);
	sdp_list_free(search_list, nullptr);
	sdp_list_free(attrid_list, nullptr);
	sdp_close(session);

	if (l2capPort != 0) {

		std::cout << "Some Audio profile:\tfound (";
		std::cout << "psm: " << static_cast<unsigned int>(l2capPort);
		std::cout << ")" << std::endl;
                return true;

	} else {

		std::cout << "Any Audio profile:\tnot found." << std::endl;
                return false;
	
	}

	std::cout << std::endl;

}

// Register service
bool btCore::serviceRegister(const unsigned int service) {}

// Unregister service
bool btCore::serviceUnregister(const unsigned int service) {}

