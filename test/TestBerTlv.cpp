#include "catch.hpp"

#include "BerTlv.h"

TEST_CASE("Test BerTlv decoding") {
	using namespace ber_tlv_decoder;

	// TLV of a 1-byte tag and 1-byte length
	BerTlv tlvT1L1("95051234567890");
	CHECK(tlvT1L1.getTag() == "95");
	CHECK(tlvT1L1.getLength() == "05");
	CHECK(tlvT1L1.getValue() == "1234567890");
	CHECK(tlvT1L1.isValid() == true);

	// TLV of a 2-byte tag and 1-byte length
	BerTlv tlvT2L1("9F3303123456");
	CHECK(tlvT2L1.getTag() == "9F33");
	CHECK(tlvT2L1.getLength() == "03");
	CHECK(tlvT2L1.getValue() == "123456");
	CHECK(tlvT2L1.isValid() == true);

	// TLV of a 2-byte tag and 1-byte length of 127
	BerTlv tlvLength127("9F337F12345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength127.getTag() == "9F33");
	CHECK(tlvLength127.getLength() == "7F");
	CHECK(tlvLength127.getValue() == "12345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength127.isValid() == true);

	// TLV of a 2-byte tag and 2-byte length of 128 
	BerTlv tlvLength128("9F3381801112345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength128.getTag() == "9F33");
	CHECK(tlvLength128.getLength() == "8180");
	CHECK(tlvLength128.getValue() == "1112345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345612345611223344556677");
	CHECK(tlvLength128.isValid() == true);

	// TLV of a 2-byte tag and 3-byte length of 435 
	BerTlv tlvLength435("9F338201B3111234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561122334455667711123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
	CHECK(tlvLength435.getTag() == "9F33");
	CHECK(tlvLength435.getLength() == "8201B3");
	CHECK(tlvLength435.getValue() == "111234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561234561122334455667711123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456123456111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
	CHECK(tlvLength435.isValid() == true);
	
	// Do without length and value
	BerTlv tlvNoLengthNoValue("9F33");
	CHECK(tlvNoLengthNoValue.getTag() == "9F33");
	CHECK(tlvNoLengthNoValue.getLength().empty());
	CHECK(tlvNoLengthNoValue.getValue().empty());
	CHECK(tlvNoLengthNoValue.isValid() == false);

	// Do without value
	BerTlv tlvhNoValue("9F3302");
	CHECK(tlvhNoValue.getTag() == "9F33");
	CHECK(tlvhNoValue.getLength() == "02");
	CHECK(tlvhNoValue.getValue().empty());
	CHECK(tlvhNoValue.isValid() == false);


	// TLV of a 2-byte tag and 1-byte length of 2 but the value has a wrong length 
	BerTlv tlvWrongLength("9F3302012345");
	CHECK(tlvWrongLength.getTag() == "9F33");
	CHECK(tlvWrongLength.getLength() == "02");
	CHECK(tlvWrongLength.getValue() == "0123");
	CHECK(tlvWrongLength.isValid() == false);


	

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
	


	// TLV of two tags. Should be considered as one constructed tag with both tags inside it
	BerTlv tlvTwoTags("95013396026699");
	CHECK(tlvTwoTags.getTag().empty());
	CHECK(tlvTwoTags.getLength().empty());
	CHECK(tlvTwoTags.getValue() == "95013396026699");
	CHECK(tlvTwoTags.getTagType() == TagType::CONSTRUCTED);
	CHECK(tlvTwoTags.isValid() == true);
	const auto tlvTwoTagsChildren = tlvTwoTags.getChildren();
	CHECK(tlvTwoTagsChildren.size() == 2);
	CHECK(tlvTwoTagsChildren[0].getTag() == "95");
	CHECK(tlvTwoTagsChildren[0].getLength() == "01");
	CHECK(tlvTwoTagsChildren[0].getValue() == "33");
	CHECK(tlvTwoTagsChildren[0].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvTwoTagsChildren[0].isValid() == true);	
	CHECK(tlvTwoTagsChildren[1].getTag() == "96");
	CHECK(tlvTwoTagsChildren[1].getLength() == "02");
	CHECK(tlvTwoTagsChildren[1].getValue() == "6699");
	CHECK(tlvTwoTagsChildren[1].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvTwoTagsChildren[1].isValid() == true);

	// constructed TLV with 1 tag inside
	BerTlv tlvConstructedOneTag("E003950133");
	CHECK(tlvConstructedOneTag.getTag() == "E0");
	CHECK(tlvConstructedOneTag.getLength() == "03");
	CHECK(tlvConstructedOneTag.getValue() == "950133");
	CHECK(tlvConstructedOneTag.getTagType() == TagType::CONSTRUCTED);
	CHECK(tlvConstructedOneTag.isValid() == true);
	const auto tlvConstructedOneTagChildren = tlvConstructedOneTag.getChildren();
	CHECK(tlvConstructedOneTagChildren.size() == 1);
	CHECK(tlvConstructedOneTagChildren[0].getTag() == "95");
	CHECK(tlvConstructedOneTagChildren[0].getLength() == "01");
	CHECK(tlvConstructedOneTagChildren[0].getValue() == "33");
	CHECK(tlvConstructedOneTagChildren[0].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedOneTagChildren[0].isValid() == true);

	// constructed TLV with 3 tags inside
	BerTlv tlvConstructedMultipleTags("E00F950133960266979F33051234567890");
	CHECK(tlvConstructedMultipleTags.getTag() == "E0");
	CHECK(tlvConstructedMultipleTags.getLength() == "0F");
	CHECK(tlvConstructedMultipleTags.getValue() == "950133960266979F33051234567890");
	CHECK(tlvConstructedMultipleTags.getTagType() == TagType::CONSTRUCTED);
	CHECK(tlvConstructedMultipleTags.isValid() == true);
	const auto tlvConstructedMultipleTagsChildren = tlvConstructedMultipleTags.getChildren();
	CHECK(tlvConstructedMultipleTagsChildren.size() == 3);
	CHECK(tlvConstructedMultipleTagsChildren[0].getTag() == "95");
	CHECK(tlvConstructedMultipleTagsChildren[0].getLength() == "01");
	CHECK(tlvConstructedMultipleTagsChildren[0].getValue() == "33");
	CHECK(tlvConstructedMultipleTagsChildren[0].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedMultipleTagsChildren[0].isValid() == true);
	CHECK(tlvConstructedMultipleTagsChildren[1].getTag() == "96");
	CHECK(tlvConstructedMultipleTagsChildren[1].getLength() == "02");
	CHECK(tlvConstructedMultipleTagsChildren[1].getValue() == "6697");
	CHECK(tlvConstructedMultipleTagsChildren[1].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedMultipleTagsChildren[1].isValid() == true);
	CHECK(tlvConstructedMultipleTagsChildren[2].getTag() == "9F33");
	CHECK(tlvConstructedMultipleTagsChildren[2].getLength() == "05");
	CHECK(tlvConstructedMultipleTagsChildren[2].getValue() == "1234567890");
	CHECK(tlvConstructedMultipleTagsChildren[2].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedMultipleTagsChildren[2].isValid() == true);

	// constructed TLV with a constructed TLV inside
	BerTlv tlvConstructedWithConstructedAndPrimitive("E010950133E0039501339F33051234567890");
	CHECK(tlvConstructedWithConstructedAndPrimitive.getTag() == "E0");
	CHECK(tlvConstructedWithConstructedAndPrimitive.getLength() == "10");
	CHECK(tlvConstructedWithConstructedAndPrimitive.getValue() == "950133E0039501339F33051234567890");
	CHECK(tlvConstructedWithConstructedAndPrimitive.getTagType() == TagType::CONSTRUCTED);
	CHECK(tlvConstructedWithConstructedAndPrimitive.isValid() == true);
	const auto tlvConstructedWithConstructedAndPrimitiveChildren = tlvConstructedWithConstructedAndPrimitive.getChildren();
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren.size() == 3);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[0].getTag() == "95");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[0].getLength() == "01");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[0].getValue() == "33");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[0].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[0].isValid() == true);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[1].getTag() == "E0");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[1].getLength() == "03");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[1].getValue() == "950133");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[1].getTagType() == TagType::CONSTRUCTED);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[1].isValid() == true);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[2].getTag() == "9F33");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[2].getLength() == "05");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[2].getValue() == "1234567890");
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[2].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedWithConstructedAndPrimitiveChildren[2].isValid() == true);
	const auto tlvConstructedinConstructedChildren = tlvConstructedWithConstructedAndPrimitiveChildren[1].getChildren();
	CHECK(tlvConstructedinConstructedChildren.size() == 1);
	CHECK(tlvConstructedinConstructedChildren[0].getTag() == "95");
	CHECK(tlvConstructedinConstructedChildren[0].getLength() == "01");
	CHECK(tlvConstructedinConstructedChildren[0].getValue() == "33");
	CHECK(tlvConstructedinConstructedChildren[0].getTagType() == TagType::PRIMITIVE);
	CHECK(tlvConstructedinConstructedChildren[0].isValid() == true);


}