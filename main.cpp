#include <iostream>
#include <iomanip>
#include <errno.h>
#include <list>

#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>

#include "BT/btCore.hpp"
#include "BT/btDevice.hpp"


int main(int argc, char** argv) {

	btCore bt;
	
	const char	localName[20]	= "RDM DM3730 Test KIT";

	if (!bt.setup(localName, 20, 0x0024041C)) {

		std::cout << "Shit during setup..." << std::endl;

	}

	std::cout << std::endl;
	std::cout << "HCI Device name set to:\t\"" << localName << "\"" << std::endl;
	std::cout << std::endl;
	std::cout << "HCI Inquiry started..." << std::endl;

	std::list<btDevice>* devList;// = bt.inquiry(8);

	if (devList->empty()) {

		std::cout << "List empty..." << std::endl;

	}

	std::cout << "=================================================================================" << std::endl;
	std::cout << "| #\t" << "| BTA\t\t\t" << "| Name\t\t\t\t" << "| COD\t\t|" << std::endl;
	std::cout << "---------------------------------------------------------------------------------" << std::endl;

	char addr[19] = {0};

	int i = 0;

	for (auto const dev : *devList) {

		//dev.getAddress().get(addr, 18);

		std::cout << "| " << std::setw(2) << std::setfill(' ') << std::left << ++i << std::right << "\t";
		//std::cout << "| " << addr << "\t";
		//std::cout << "| " << std::setw(30) << std::left << dev.getName() << std::right;
		std::cout << "| " << "0x";
		//std::cout << std::setw(8) << std::setfill('0') << std::hex << dev.getCOD() << std::dec << std::endl;
		std::cout << "\t|" << std::endl;

	}

	std::cout << "=================================================================================" << std::endl;
	std::cout << std::endl;

	unsigned int userChoise = 0;

	std::list<btDevice>::iterator it;

	while (true) {

		std::cout << "Device to connect:\t";
		std::cin >> userChoise;
		std::cout << std::endl;

		if (userChoise > 0 && userChoise < devList->size()) {

			//btDevice device = devList[i];
			it = std::next(devList->begin(), userChoise);

			std::cout << "Connecting to device\t" << userChoise << " ..." << std::endl;
			std::cout << std::endl;

			if (bt.connect(it->getAddress())) {

				std::cout << "Connection succeded" << std::endl;
				std::cout << std::endl;

				break;

			}

		}

	}

	// Connection to L2CAP
        struct sockaddr_l2 lAddr = {0};
        int sk2 = socket(AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
	if (sk2 < 0) {

		std::cerr << "Socket open:\t\t" << strerror(errno) << std::endl;

	}
	std::cout << "Socket open:\t\tOK" << std::endl;

        memset(&lAddr, 0, sizeof(lAddr));

	bdaddr_t target;
	str2ba(it->getAddress().get(addr, 18), &target);

	lAddr.l2_family		= AF_BLUETOOTH;
        lAddr.l2_psm		= 0x0100;
        lAddr.l2_bdaddr		= target;

        if (connect(sk2, (struct sockaddr*)&lAddr, sizeof(lAddr)) < 0) {

        	std::cerr << "Socket connect:\t\t" << strerror(errno) << std::endl;

	}
	std::cout << "Socket connect:\t\tOK" << std::endl;
	std::cout << std::endl;


	std::cout << "Waiting for disconnection..." << std::endl;
	struct pollfd p;
	p.fd		= sk2;
	p.events	= POLLERR | POLLHUP;

	while (true) {

		p.revents = 0;

		if (poll(&p, 1, 500)) {

			break;

		}

	}

	std::cout << "Connection stopped." << std::endl;

	// close(sk);
	close(sk2);

	return 0;

}

