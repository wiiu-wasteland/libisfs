#include <ISFS/ISFSFileDevice.hpp>

ISFSFileDevice::ISFSFileDevice(std::string &path, bool writeEnabled)
	: writeEnabled(writeEnabled)
{
	auto mode = std::fstream::in | std::fstream::binary; 
	if (writeEnabled) {
		mode |= std::fstream::out;
	}

	fdev.open(path, mode);
}

ISFSFileDevice::~ISFSFileDevice()
{
	fdev.close();
}

int ISFSFileDevice::WritePage(NANDPage page, uint8_t *data, uint8_t *spare, bool calcEcc)
{
	std::vector<uint8_t> spareBuf(SpareSize, 0);

	/* check if writing is allowed */
	if (!writeEnabled) {
		return -1;
	}

	/* copy input spare data */
	if (spare) {
		memcpy(spareBuf.data(), spare, SpareSize);
	}

	/* compute ecc if requested */
	if (calcEcc) {
		ISFSEcc::ComputeBlock(&data[0x000], &spareBuf[0x30]);
		ISFSEcc::ComputeBlock(&data[0x200], &spareBuf[0x34]);
		ISFSEcc::ComputeBlock(&data[0x400], &spareBuf[0x38]);
		ISFSEcc::ComputeBlock(&data[0x600], &spareBuf[0x3C]);
	}

	/* good page */
	spareBuf[0] = 0xff;

	/* write page and spare to nand file */
	fdev.seekp(page * (PageSize + SpareSize));
	fdev.write((char *)data, PageSize);
	fdev.write((char *)spareBuf.data(), spareBuf.size());
	
	return 0;
}

ISFSFileDevice::ReadPage(NANDPage page, uint8_t *data, uint8_t *spare = nullptr, bool checkEcc = true)
{
	std::vector<uint8_t> spareBuf(SpareSize);
	
	/* read page and spare from nand file */
	fdev.seekg(page * (PageSize + SpareSize));
	fdev.read((char *)data, PageSize);
	fdev.read((char *)spareBuf.data(), spareBuf.size());
	
	/* copy spare data */
	if (spare) {
		memcpy(spare, spareBuf.data(), SpareSize);
	}
	
	/* not a good page */
	if (spareBuf[0] != 0xff) {
		return -1
	}
	
	/* verify ecc if requested */
	if (checkEcc) {
		std::vector<uint8_t> computedEcc(0x10);
		int eccStatus;
		
		/* compute ecc */
		ISFSEcc::ComputeBlock(&data[0x000], &computedEcc[0x0]);
		ISFSEcc::ComputeBlock(&data[0x200], &computedEcc[0x4]);
		ISFSEcc::ComputeBlock(&data[0x400], &computedEcc[0x8]);
		ISFSEcc::ComputeBlock(&data[0x600], &computedEcc[0xC]);
		
		/* correct ecc errors */
		eccStatus = ISFSEcc::Correct(data, &spareBuf[0x30], computedEcc.data());
		
		/* uncorrectable errors */
		if (eccStatus < 0) {
			return eccStatus;
		}
	}
	
	return 0;
}

