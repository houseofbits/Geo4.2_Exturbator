#pragma once

#include <string>

////////////////
//// ???? 

class HardwareDataPacket {
public:

	std::string			header;
	unsigned int	code;
	std::string			rawData;
};

class HardwareInterface
{
public:
	HardwareInterface();
	~HardwareInterface();




};

