#include "BerTlv.h"

#include <iostream>

namespace ber_tlv_decoder {

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

	TagClass BerTlv::getTagClass() const
	{
		return tagClass;
	}

	TagType BerTlv::getTagType() const
	{
		return tagType;
	}

	void BerTlv::decode(const std::string & tlv)
	{
		int currentReadIndex = 0;
		decodeTag(tlv, currentReadIndex);
		decodeLength(tlv, currentReadIndex);
		decodeValue(tlv, currentReadIndex);



		std::cout << tag << "\n";
		std::cout << length << "\n";
		std::cout << value << "\n";

	}

	void BerTlv::decodeTag(const std::string & tlv, int & currentReadIndex)
	{
		tag = tlv.substr(currentReadIndex, 2);
		currentReadIndex += 2;

		const int firstByte = std::stoi(tag, nullptr, 16);

		const int tagClassBits = firstByte & 0xC0; // Check first 2 bits
		if (tagClassBits == 0)
		{
			tagClass = TagClass::UNIVERSAL;
		}
		else if ((tagClassBits & 0xC0) == 0xC0)
		{
			tagClass = TagClass::PRIVATE;
		}
		else if ((tagClassBits & 0x80) == 0x80)
		{
			tagClass = TagClass::CONTEXT_SPECIFIC;
		}
		else if ((tagClassBits & 0x40) == 0x40)
		{
			tagClass = TagClass::APPLICATION;
		}

		const int tagTypeBits = firstByte & 0x20; // Check bit 6
		if (tagTypeBits == 0)
		{
			tagType = TagType::PRIMITIVE;
		}
		else
		{
			tagType = TagType::CONSTRUCTED;
		}



		const int tagNumber = firstByte & 0x1F; // Check last 5 bits

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
	}

	void BerTlv::decodeLength(const std::string & tlv, int & currentReadIndex)
	{
		length = tlv.substr(currentReadIndex, 2);
		currentReadIndex += 2;

		const int firstByte = std::stoi(length, nullptr, 16);

		const int lengthForm = firstByte & 0x80; // Check 8th bit

		// Check if the tag is using more than one byte
		// 8th bit at 0 means the length only uses one byte
		// 8th bit at 1 means the length uses more than one byte. The number of bytes is encoded in the other 7 bits
		if (lengthForm != 0)
		{
			const int numberLengthBytes = firstByte & 0x7F;
			for (int currentByte = 0; currentReadIndex < tlv.length() && currentByte < numberLengthBytes; ++currentByte)
			{
				const std::string byteString = tlv.substr(currentReadIndex, 2);
				currentReadIndex += 2;
				length += byteString;

				// to convert to an actual value, will stoi of the entire string work?
			}
		}
	}

	void BerTlv::decodeValue(const std::string & tlv, int & currentReadIndex)
	{
		value = tlv.substr(currentReadIndex);
	}
}