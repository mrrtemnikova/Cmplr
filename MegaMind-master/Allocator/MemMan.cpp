#include <cstddef>
#include "Allocator.h"



class MemMan {
private:
	const size_t chunksPerBlock = 8;
	const size_t chunkSize = 4096;
	const size_t blockSize = 32768;

	MemMan() {
		Allocator* allocator = new Allocator(blockSize, chunkSize);
	}



};