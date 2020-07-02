#pragma once

class ISFSDevice
{
	static constexpr auto
		PageSize = 0x800,
		SpareSize = 0x40;

	virtual int WritePage(NANDPage page, uint8_t *data, uint8_t *spare = nullptr, bool calcEcc = true);

	virtual int ReadPage(NANDPage page, uint8_t *data, uint8_t *spare = nullptr, bool checkEcc = true);

	bool WriteEnabled() {
		return writeEnabled;
	}

protected:
	bool writeEnabled = false;
};
