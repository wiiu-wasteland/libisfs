#include <ISFS/ISFSEcc.h>

namespace ISFSEcc
{
	/* ECC code, originally taken from segher's unecc.c */

	static uint8_t Parity(uint8_t x)
	{
		uint8_t y = 0;
		
		while (x) {
			y ^= (x & 1);
			x >>= 1;
		}
		
		return y;
	}

	void ComputeBlock(uint8_t *data, uint8_t *outEcc)
	{
		uint8_t a[12][2];
		int i, j;
		u32 a0, a1;
		uint8_t x;
		
		memset(a, 0, sizeof(a));
		for (i = 0; i < 512; i++) {
			x = data[i];
			for (j = 0; j < 9; j++) {
				a[3 + j][(i >> j) & 1] ^= x;
			}
		}
		
		x = a[3][0] ^ a[3][1];
		a[0][0] = x & 0x55;
		a[0][1] = x & 0xaa;
		a[1][0] = x & 0x33;
		a[1][1] = x & 0xcc;
		a[2][0] = x & 0x0f;
		a[2][1] = x & 0xf0;
		
		for (j = 0; j < 12; j++) {
			a[j][0] = Parity(a[j][0]);
			a[j][1] = Parity(a[j][1]);
		}
		
		a0 = a1 = 0;
		for (j = 0; j < 12; j++) {
			a0 |= a[j][0] << j;
			a1 |= a[j][1] << j;
		}
		
		outEcc[0] = a0;
		outEcc[1] = a0 >> 8;
		outEcc[2] = a1;
		outEcc[3] = a1 >> 8;
	}

	int Correct(uint8_t *data, uint8_t *goodEcc, uint8_t *computedEcc)
	{
		int i, uncorrectable = 0, corrected = 0;
		uint32_t *l_goodEcc = (uint32_t*)goodEcc;
		uint32_t *l_computedEcc = (uint32_t*)computedEcc;
		
		for (i = 0; i < 4; i++, data += 0x200, l_goodEcc++, l_computedEcc++) {
			uint32_t syndrome = *l_goodEcc ^ *l_computedEcc;
			
			/* don't try to correct unformatted pages (all FF) */
			if (!syndrome || (*l_goodEcc == ~0)) {
				continue;
			}
			
			/* single-bit error in ECC */
			if ((syndrome & (syndrome - 1)) == 0) {
				corrected++;
				continue;
			}
			
			/* byteswap and extract odd and even halves */
			uint16_t even = (syndrome >> 24) | ((syndrome >> 8) & 0xf00);
			uint16_t odd = ((syndrome << 8) & 0xf00) | ((syndrome >> 8) & 0x0ff);
			
			if((even ^ odd) != 0xfff) {
				/* oops, can't fix this one */
				uncorrectable++;
			} else {
				/* fix the bad bit */
				data[odd >> 3] ^= 1 << (odd & 7);
				corrected++;
			}
		}
		
		if (uncorrectable) {
			return -1;
		}
		
		if (corrected) {
			return 1;
		}
		
		return 0;
	}
};

