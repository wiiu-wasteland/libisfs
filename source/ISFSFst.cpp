#pragma once

void ISFSFst::Read(uint8_t *p)
{
	int index;
	for (index = 0; index < FstEntryCount; index++) {
		FST[index].Read(p + index * ISFSFstEntry::FstEntrySize);
	}
}

void ISFSFst::Write(uint8_t *p)
{
	int index;
	for (index = 0; index < FstEntryCount; index++) {
		FST[index].Write(p + index * ISFSFstEntry::FstEntrySize);
	}
}

uint16_t ISFSFst::AllocateEntry()
{
	uint16_t index;
	for (index = 0; index < FstEntryCount; index++) {
		if (FST[index].IsEmpty()) {
			return index;
		}
	}
	return -1;
}

uint16_t ISFSFst::GetDirEntry(uint16_t dirIndex, std::string &name)
{
	uint16_t index;

	if ((dirIndex == InvalidEntry) || !FST[dirIndex].IsDirectory()) {
		return InvalidEntry;
	}

	for (index = FST[dirIndex].sub; index != InvalidEntry; index = FST[dirIndex].sib) {
		if (FST[index].name == name) {
			break;
		}
	}

	return index;
}

uint16_t ISFSFst::GetPathEntry(std::string &path)
{
	std::stringstream spath(path);
	std::vector<int> subdirs;
	std::string subpath;
	uint16_t entry = 0;

	while (std::getline(spath, subpath, '/') && (entry != InvalidEntry)) {
		if ((subpath.length() == 0) || (subpath == ".")) {
			continue;
		}

		if (subpath == "..") {
			if (subdirs.size() == 0) {
				return InvalidEntry;
			}
			entry = subdirs.pop_back();
			continue;
		}

		subdirs.push_back(entry);
		entry = GetDirEntry(entry, subpath);
	}

	return entry;
}


