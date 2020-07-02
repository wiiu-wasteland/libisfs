#pragma once

class ISFSFat
{
public:
	static constextr auto
		FatEntryCount = 0x8000,
		FatSize = FatEntryCount * sizeof(uint16_t);

	static constexpr uint16_t
		FinalCluster	= 0xFFFB,
		ReservedCluster	= 0xFFFC,
		BadCluster		= 0xFFFD,
		EmptyCluster	= 0xFFFE,
		InvalidCluster	= 0xFFFF;

	std::vector<uint16_t> FAT(FatEntryCount);

	uint16_t AllocateCluster();
	void FreeCluster(uint16_t cluster);

	void Read(uint8_t *p);
	void Write(uint8_t *p);
};
