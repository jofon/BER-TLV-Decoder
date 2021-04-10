#pragma once

#include <string>

namespace ber_tlv_decoder {
	class BerTlv {
	public:
		BerTlv(const std::string & tlv);

		std::string getTag() const;
		std::string getLength() const;
		std::string getValue() const;

	private:

		std::string tag;
		std::string length;
		std::string value;

		void decode(const std::string & tlv);

		void decodeTag(const std::string & tlv, int & currentReadIndex);

		void decodeLength(const std::string & tlv, int & currentReadIndex);

		void decodeValue(const std::string & tlv, int & currentReadIndex);

	};
}