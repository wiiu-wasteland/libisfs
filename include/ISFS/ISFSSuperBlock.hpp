#pragma once
#include <ISFS/ISFSFat.hpp>
#include <ISFS/ISFSFst.hpp>

class ISFSSuperblock
{
public:
	ISFSFat fat;
	ISFSFst fst;

	void Read(uint8_t *p);
	void Write(uint8_t *p);

	static bool IsValidSuperblock(uint8_t *p);
	static uint32_t GetSuperblockVersion(uint8_t *p);
};
