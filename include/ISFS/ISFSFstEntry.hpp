#pragma once

class ISFSFstEntry
{
	static constextr auto
		FstEntrySize = 0x20;

	void Read(uint8_t *p);
	void Write(uint8_t *p);

	bool IsEmpty();

	bool IsFile();
	bool IsDirectory();

	/* FST entry data */
	std::string name;
	uint8_t mode;
	uint8_t attr;
	uint16_t sub;
	uint16_t sib;
	uint32_t size;
	uint16_t x1;
	uint16_t uid;
	uint16_t gid;
	uint32_t x3;
};
