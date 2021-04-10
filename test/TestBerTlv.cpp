#include "catch.hpp"

#include "BerTlv.h"

TEST_CASE("Test BerTlv decoding") {
	using namespace ber_tlv_decoder;

	// TLV of a 1-byte tag and 1-byte length
	BerTlv tlvT1L1("95051234567890");
	CHECK(tlvT1L1.getTag() == "95");
	CHECK(tlvT1L1.getLength() == "05");
	CHECK(tlvT1L1.getValue() == "1234567890");

	// TLV of a 2-byte tag and 1-byte length
	BerTlv tlvT2L1("9F3303123456");
	CHECK(tlvT2L1.getTag() == "9F33");
	CHECK(tlvT2L1.getLength() == "03");
	CHECK(tlvT2L1.getValue() == "123456");

	// TLV of a 2-byte tag and 1-byte length of 127
	BerTlv tlvLength127("9F337F12345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength127.getTag() == "9F33");
	CHECK(tlvLength127.getLength() == "7F");
	CHECK(tlvLength127.getValue() == "12345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
		
	// TLV of a 2-byte tag and 2-byte length of 128 
	BerTlv tlvLength128("9F3381801112345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength128.getTag() == "9F33");
	CHECK(tlvLength128.getLength() == "8180");
	CHECK(tlvLength128.getValue() == "1112345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	
	// TLV of a 2-byte tag and 3-byte length of 435 
	BerTlv tlvLength435("9F338201B3111234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561122334455667711123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
	CHECK(tlvLength435.getTag() == "9F33");
	CHECK(tlvLength435.getLength() == "8201B3");
	CHECK(tlvLength435.getValue() == "111234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561122334455667711123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
	
	
	// Do without length and value
	// Do without value

	// Do with invalid value length?

	

	// Universal class
	BerTlv tlvUniversal("0533");
	CHECK(tlvUniversal.getTagClass() == TagClass::UNIVERSAL);
	
	// Application class
	BerTlv tlvApplication("4033");
	CHECK(tlvApplication.getTagClass() == TagClass::APPLICATION);
	
	// Context-specific class
	BerTlv tlvContextSpecific("8033");
	CHECK(tlvContextSpecific.getTagClass() == TagClass::CONTEXT_SPECIFIC);

	// Private class
	BerTlv tlvPrivate("D005");
	CHECK(tlvPrivate.getTagClass() == TagClass::PRIVATE);


	// Primitive type
	BerTlv tlvPrimitive("9505");
	CHECK(tlvPrimitive.getTagType() == TagType::PRIMITIVE);

	// Constructed type
	BerTlv tlvConstructed("A505");
	CHECK(tlvConstructed.getTagType() == TagType::CONSTRUCTED);


}