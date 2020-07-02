#include <ISFS/ISFSDevice.hpp>

class ISFS
{
public:
	ISFS(ISFSDevice &device, ISFSKeys &keys);
	~ISFS();

	void Init();

	void ReadCluster(uint16_t cluster );
	void WriteCluster(uint16_t );
	
	ISFSSuperblock superblock;
};
