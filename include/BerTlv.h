#pragma once

#include <string>

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


};