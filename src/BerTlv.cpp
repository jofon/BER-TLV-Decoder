#include "BerTlv.h"

#include <iostream>

BerTlv::BerTlv(const std::string & tlv)
{
	decode(tlv);
}

std::string BerTlv::getTag() const
{
	return tag;
}

std::string BerTlv::getLength() const
{
	return length;
}
std::string BerTlv::getValue() const
{
	return value;
}

void BerTlv::decode(const std::string & tlv)
{
	int currentReadIndex = 0;
	tag = tlv.substr(currentReadIndex, 2);
	currentReadIndex += 2;

	const int firstByte = std::stoi(tag, nullptr, 16);

	int dataClass; // TODO make enum. need tests
	int dataType; // TODO make enum. Need tests

	const int tagNumber = firstByte & 0x1f; // Check last 5 bits
	
	// Check if the tag is using more than one byte
	if (tagNumber >= 31)
	{
		for (; currentReadIndex < tlv.length();)
		{
			const std::string byteString = tlv.substr(currentReadIndex, 2);
			currentReadIndex += 2;
			tag += byteString;

			const int byte = std::stoi(byteString, nullptr, 16);
			const bool hasNextByte = byte & 0x80; // Check first bit
			if (!hasNextByte)
			{
				break;
			}
		}
	}



	std::cout << tag << "\n";
	
	length = tlv.substr(currentReadIndex, 2);
	currentReadIndex += 2;
	value = tlv.substr(currentReadIndex);
}