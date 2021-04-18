#include "BerTlv.h"

#include <iostream>

namespace ber_tlv_decoder {

	BerTlv::BerTlv(const std::string & tlv)
	{
		decode(tlv, true);
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

	bool BerTlv::isValid() const
	{
		return isValidTlv;
	}

	std::vector<BerTlv> BerTlv::getChildren() const
	{
		return childrenTlvs;
	}

	void BerTlv::decode(const std::string & tlv, const bool allowConstructedIfMultipleTlvs)
	{
		decodeTag(tlv);
		decodeLength(tlv);

		if (isValidTlv) {
			decodeValue(tlv);
		}

		if (tagType == TagType::CONSTRUCTED && !value.empty())
		{
			const int lengthInt = convertLengthToInt();
			int currentReadIndexChildren = 0;
			while (currentReadIndexChildren != lengthInt)
			{
				BerTlv nextTag;
				nextTag.decode(value.substr(currentReadIndexChildren), false);
				nextTag.isValidTlv = true;
				childrenTlvs.push_back(nextTag);

				currentReadIndexChildren += nextTag.currentReadIndex;
			}
		}
		else if (!isValidTlv) {
			if (allowConstructedIfMultipleTlvs)
			{
				if (currentReadIndex != tlv.size() && currentReadIndex != tlv.size() + 2)
				{
					const int lengthInt = tlv.size();
					if (lengthInt > 0)
					{
						isValidTlv = true;

						// this is a group of tags, so we add them as children and consider this a constructed tag
						childrenTlvs.push_back(*this);

						int currentReadIndexChildren = currentReadIndex;
						while (currentReadIndexChildren < lengthInt)
						{
							BerTlv nextTag;
							nextTag.decode(tlv.substr(currentReadIndexChildren), false);
							if (nextTag.isValidTlv)
							{
								childrenTlvs.push_back(nextTag);
							}
							currentReadIndexChildren += nextTag.currentReadIndex;
						}

						// If nothing else in the TLV was considered valid, we go back to having the first valid TLV as the only one
						if (childrenTlvs.size() > 1)
						{
							tagType = TagType::CONSTRUCTED;

							tag.clear();
							value = tlv;
							length.clear();
							tagClass = TagClass::UNIVERSAL;
						}
						else
						{
							childrenTlvs.clear();
							isValidTlv = false;
						}
					}
				}
			}
			else if (hasMoreAfterValue)
			{
				isValidTlv = true; // this is part of a group of tags not inside a constructed tag, so we consider this as valid and will continue to evalute the rest of the tlv
			}
		}
	}

	void BerTlv::decodeTag(const std::string & tlv)
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

	void BerTlv::decodeLength(const std::string & tlv)
	{
		length = tlv.substr(currentReadIndex, 2);
		currentReadIndex += 2;

		if (length.empty()) {
			isValidTlv = false;
			return;
		}

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
			}
		}
	}

	void BerTlv::decodeValue(const std::string & tlv)
	{
		value = tlv.substr(currentReadIndex);

		if (value.empty() || value.size() % 2 != 0) // a value with odd length is invalid
		{
			isValidTlv = false;
		}
		else
		{
			const int lengthInt = convertLengthToInt();
			if (value.size() != lengthInt)
			{
				value = tlv.substr(currentReadIndex, lengthInt);
				isValidTlv = false;
				hasMoreAfterValue = true;
			}
			currentReadIndex += lengthInt;
		}
	}

	int BerTlv::convertLengthToInt() const
	{
		return std::stoi(length.size() > 2 ? length.substr(2) : length, nullptr, 16) * 2; // times two because each element of the value is hexadecimal, so it has two characters
	}
}