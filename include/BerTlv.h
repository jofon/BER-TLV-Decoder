#pragma once

#include <string>
#include <vector>

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

		bool isValid() const;

		std::vector<BerTlv> getChildren() const;

	private:

		int currentReadIndex = 0;

		std::string tag;
		std::string length;
		std::string value;

		TagClass tagClass = TagClass::UNIVERSAL;
		TagType tagType = TagType::PRIMITIVE;

		bool isValidTlv = true;

		std::vector<BerTlv> childrenTlvs;

		BerTlv() = default;

		void decode(const std::string & tlv, const bool allowConstructedIfMultipleTlvs);

		void decodeTag(const std::string & tlv, int & currentReadIndex);

		void decodeLength(const std::string & tlv, int & currentReadIndex);

		void decodeValue(const std::string & tlv, int & currentReadIndex);

	};
}