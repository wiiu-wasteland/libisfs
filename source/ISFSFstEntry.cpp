#include <ISFS/ISFSFst.hpp>

void IFSFFstEntry::Read(uint8_t *p)
{
	name = std::string(reinterpret_cast<char const*>(&p[0x00]), 12);
	mode	= p[0x0C];
	attr	= p[0x0D];
	sub		= rbe16(&p[0x0E]);
	sib		= rbe16(&p[0x10]);
	size	= rbe32(&p[0x12]);
	x1		= rbe16(&p[0x16]);
	uid		= rbe16(&p[0x18]);
	gid 	= rbe16(&p[0x1A]);
	x3		= rbe32(&p[0x1C]);
}

void IFSFFstEntry::Write(uint8_t *p)
{
	strncpy((char *)&p[0x00], name.c_str(), 11);
	p[0x0B] = 0;
	p[0x0C] = mode;
	p[0x0D] = attr;
	wbe16(&p[0x0E], sub);
	wbe16(&p[0x10], sib);
	wbe32(&p[0x12], size);
	wbe16(&p[0x16], x1);
	wbe16(&p[0x18], uid);
	wbe16(&p[0x1A], gid);
	wbe32(&p[0x1C], x3);
}

bool IFSFFstEntry::IsEmpty()
{
	return attr == 0;
}

bool IFSFFstEntry::IsFile()
{
	return (mode & 3) == 1;
}

bool IFSFFstEntry::IsDirectory()
{
	return (mode & 3) == 2;
}
