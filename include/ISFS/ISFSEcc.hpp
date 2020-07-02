#include <cstdint>

namespace ISFSEcc
{
	void Compute(uint8_t *data, uint8_t *outEcc);

	int Correct(uint8_t *data, uint8_t *goodEcc, uint8_t *computedEcc);
};
