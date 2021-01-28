#ifndef FXCG_STRING_H
#define FXCG_STRING_H

static inline void Memset4(void* dst, int val, size_t dwords) {
	int* point = (int*)dst;
	for (int i = 0; i < dwords; i++) {
		*(point++) = val;
	}
}

static unsigned int lastrandom = 0x12345678;

static inline void Srand(unsigned int seed) {
	lastrandom = seed;
}

static inline int Rand(void) {
	lastrandom = 0x41C64E6D * lastrandom + 0x3039;
	return lastrandom >> 16;
}


#endif