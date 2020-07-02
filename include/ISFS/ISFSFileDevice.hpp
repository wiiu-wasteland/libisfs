#include <ISFS/ISFSDevice.hpp>

class ISFSFileDevice : ISFSDevice
{
public:
	ISFSFileDevice(std::string &path, bool writeEnabled = false);
	~ISFSFileDevice();

private:
	std::fstream fdev;
};
