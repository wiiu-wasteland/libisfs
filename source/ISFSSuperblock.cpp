#include <ISFS/ISFSSuperblock.hpp>

void ISFSSuperblock::Read(uint8_t *p)
{
	fat.Read(&p[0x0000C]);
	fst.Read(&p[0x1000C]);
}

void ISFSSuperblock::Write(uint8_t *p)
{
	fat.Write(&p[0x0000C]);
	fst.Write(&p[0x1000C]);
}

static bool IsValidSuperblock(uint8_t *p)
{
	std::string magic = std::string(
		reinterpret_cast<char const*>(&p[0x00000]), 4
	);
	return (magic == "SFFS") || (magic == "SFS!");
}

static uint32_t GetSuperblockVersion(uint8_t *p)
{
	return rbe32(&p[0x00004]);
}
