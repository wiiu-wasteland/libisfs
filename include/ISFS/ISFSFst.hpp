#pragma once

class ISFSFst
{
	static constextr auto
		FstEntryCount = 0x17FF,
		FstSize = FstEntryCount * ISFSFstEntry::FstEntrySize;

	static constexpr uint16_t
		InvalidEntry = 0xFFFF;
	
	std::vector<ISFSFstEntry> FST(FstEntryCount);

	uint16_t GetDirEntry(uint16_t dir, std::string &name);
	uint16_t GetPathEntry(std::string &path);

	uint16_t AllocateEntry();

	void Read(uint8_t *p);
	void Write(uint8_t *p);
};

