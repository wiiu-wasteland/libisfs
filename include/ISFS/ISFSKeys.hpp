#pragma once
#include <cstdint>
#include <vector>

class ISFSKeys
{
public:
	ISFSKeys(std::vector<uint8_t> &nandKey, std::vector<uint8_t> &hmacKey)
		: nandKey(nandKey)
		, hmacKey(hmacKey);

	std::vector<uint8_t> GetNandKey() {
		return nandKey;
	}

	std::vector<uint8_t> GetHmacKey() {
		return hmacKey;
	}

protected:
	std::vector<uint8_t> nandKey, hmacKey;
};
