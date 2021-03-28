#include "catch.hpp"

#include "BerTlv.h"

TEST_CASE("Test BerTlv decoding") {
	BerTlv tlvT1L1("95051234567890"); // TLV of a 1-byte tag and 1-byte length
	CHECK(tlvT1L1.getTag() == "95");
	CHECK(tlvT1L1.getLength() == "05");
	CHECK(tlvT1L1.getValue() == "1234567890");

	BerTlv tlv("9F3303123456"); // TLV of a 2-byte tag and 1-byte length
	CHECK(tlv.getTag() == "9F33");
	CHECK(tlv.getLength() == "03");
	CHECK(tlv.getValue() == "123456");

	// Do with more length

	// Do without length and value
	// Do without value


}