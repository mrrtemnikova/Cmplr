#pragma once
#include "List.h"
#include <assert.h>


class Allocator {
public:
	size_t totalSize;
	size_t chunkSize;

	size_t chunksPerBlock;

	size_t used = 0;

	void* startPtr = nullptr;


	struct Chunk {

	};
	using Node = List<Chunk>::Node;
	List<Chunk> list;

	Allocator(size_t chunkSize, size_t chunksPerBlock) {
		assert(chunkSize >= 8 && "Chunk size must be greater or equal to 8");
		//assert(totalSize % chunkSize == 0 && "Total Size must be a multiple of Chunk Size");

		this->chunkSize = chunkSize;
		this->chunksPerBlock = chunksPerBlock;
		Init();
	}

	void* AllocateBlock() {

		size_t blockSize = chunkSize * chunksPerBlock;

		Chunk* blockBegin = reinterpret_cast<Chunk*>(malloc(blockSize));

		for (int i = 0; i < chunksPerBlock;i++) {
			size_t address = (size_t)blockBegin + i * chunkSize;
			cout << "Адрес [" << i << "] сегмента -- " << (Node*)address << endl;
			list.Push((Node*)address);
		}


		return blockBegin;

	}

	void* Allocate(size_t allocationSize) {

		assert(allocationSize <= this->chunkSize && "Allocation size must be less or equal to chunk size");

		Node* freePosition = list.Pop();

		if (freePosition == NULL) {
			AllocateBlock();
			freePosition = list.Pop();
		}

		used += chunkSize;
		cout << "Использованно памяти -- " << used << endl;

		return (void*)freePosition;
	}
	void *CAllocatator(size_t number, size_t size) {
		return calloc(number, size);
	}

	void Init() {
		startPtr = malloc(totalSize);
		used = 0;
		AllocateBlock();
	}


	void Free(void* ptr) {

		//size_t ptr = (size_t)startPtr + chunk * chunkSize;

		cout << "Освобожден сегмент по адресу -- " << (Node*)ptr << endl;

		used -= chunkSize;

		list.Push((Node*)ptr);

		
	}

};