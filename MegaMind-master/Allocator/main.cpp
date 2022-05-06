#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
//#include "Allocator.cpp"
#include "Stack.cpp"
#include "Queue.cpp"
#include "Deq.cpp"
#include "Hash.h"



using namespace std;



//struct Object {
//
//	// Object data, 16 bytes:
//
//	uint64_t data[2];
//
//	// Declare out custom allocator for
//	// the `Object` structure:
//
//	static Allocator allocator;
//
//	static void* operator new(size_t size) {
//		return allocator.Allocate(size);
//	}
//
//	static void operator delete(void* ptr) {
//		return allocator.Free(ptr);
//	}
//};



int main(int argc, char const* argv[]) {
	setlocale(LC_ALL, "Russian");
	
    HashTable* ht = create_table(CAPACITY);
    ht_insert(ht, "петрушка", "петрушка");
    ht_insert(ht, "ананас", "ананас");
    ht_insert(ht, "персик", "персик");
    ht_insert(ht, "кот", "кот");
    ht_insert(ht, "собака", "собака");
    ht_insert(ht, "белка", "белка");


    ht_insert(ht, "банан", "банан");
    print_search(ht, "петрушка");

    print_table(ht);
    ht_delete(ht, "собака");
   
    print_table(ht);
   // free_table(ht);
	return 0;
};