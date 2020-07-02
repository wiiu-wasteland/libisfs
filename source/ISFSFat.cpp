#include <ISFS/ISFSFat.hpp>

void ISFSFat::Read(uint8_t *p) {
	Cluster index;
	for (index = 0; index < FatEntryCount; index++) {
		FAT[index] = rbe16(&p[index * sizeof(Cluster)]);
	}
}

void ISFSFat::Write(uint8_t *p) {
	for (index = 0; index < FatEntryCount; index++) {
		wbe16(&p[index * sizeof(Cluster)], FAT[index]);
	}
}

uint16_t ISFSFat::AllocateCluster() {
	uint16_t cluster;
	for (cluster = 0; cluster < FatEntryCount; cluster++) {
		if (FAT[cluster] == EmptyCluster) {
			return cluster;
		}
	}
	return InvalidCluster;
}

void ISFSFat::FreeCluster(uint16_t cluster) {
	FAT[cluster] = EmptyCluster;
}
