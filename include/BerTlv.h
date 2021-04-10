#pragma once

#include <string>

namespace ber_tlv_decoder {
	enum class TagClass
	{
		UNIVERSAL,
		APPLICATION,
		CONTEXT_SPECIFIC,
		PRIVATE
	};

	enum class TagType
	{
		PRIMITIVE,
		CONSTRUCTED
	};

	class BerTlv {
	public:
		BerTlv(const std::string & tlv);

		std::string getTag() const;
		std::string getLength() const;
		std::string getValue() const;

		TagClass getTagClass() const;
		TagType getTagType() const;

	private:

		std::string tag;
		std::string length;
		std::string value;

		TagClass tagClass;
		TagType tagType;

		void decode(const std::string & tlv);

		void decodeTag(const std::string & tlv, int & currentReadIndex);

		void decodeLength(const std::string & tlv, int & currentReadIndex);

		void decodeValue(const std::string & tlv, int & currentReadIndex);

	};
}