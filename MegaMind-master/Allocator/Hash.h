#define _CRT_SECURE_NO_WARNINGS
#include"Allocator.h"
#include <string.h>

#define CAPACITY 5 

unsigned long hash_function(const char* str) {

    
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

class Ht_item {
public:
    const char* key;
    const char* value;

    Allocator* ItemAllocator;

    Ht_item() {
        ItemAllocator = new Allocator(4096, 1);
     //   Ht_item* item = (Ht_item*)ItemAllocator->Allocate(sizeof(Ht_item));
    }
    void* SetKey() {
        return (char*)ItemAllocator->Allocate(sizeof(key) + 1);
    }

    void* SetValue() {
        return (char*)ItemAllocator->Allocate(sizeof(value) + 1);
    }

};


class LinkList {
public:
    Ht_item* item;
    LinkList* next;
    Allocator* ListAllocator;

    LinkList() {
        ListAllocator = new Allocator(4096, 1);
    }
    LinkList* newNode() {
        LinkList* list = (LinkList*)ListAllocator->Allocate(sizeof(LinkList));
        return list;
    }

    void free(void* ptr) {
        ListAllocator->Free(ptr);
    }
};

void* Lcalloc(Allocator* TypeAllocator, size_t a, size_t b) {
    return(TypeAllocator->CAllocatator(a, b));
}

class HashTable {
public:
    Ht_item** items;
    LinkList** overflow_buckets;
    int size;
    int count;
    Allocator* HashAllocator;

    HashTable() {
        HashAllocator = new Allocator(4096, 8);
    }
};

static LinkList* linkedlist_insert(LinkList* list, Ht_item* item) {
    if (!list) {
        LinkList* head = new LinkList();
        head = list->newNode();;
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }

    else if (list->next == NULL) {
        LinkList* node = list->newNode();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkList* temp = list;
    while (temp->next->next) {
        temp = temp->next;
    }
    LinkList* node = list->newNode();;
    node->item = item;
    node->next = NULL;
    temp->next = node;

    return list;
}

static Ht_item* linkedlist_remove(LinkList* list) {
 
    if (!list)
        return NULL;
    
  
    LinkList* temp = list;
    temp->next = NULL;
    list = list->next;
    Ht_item* it = NULL;
   // memcpy(temp->item, it, sizeof(Ht_item));

    list->free((void*)temp->item->key);
    list->free((void*)temp->item->value);
    list->free((void*)temp->item);
    list->free((void*)temp);
    return it;
}

static void free_linkedlist(LinkList* list) {
    LinkList* temp = list;
    while (list) {
        temp = list;
        list = list->next;
        list->free((void*)temp->item->key);
        list->free((void*)temp->item->value);
        list->free((void*)temp->item);
        list->free((void*)temp);
    }
}

static LinkList** create_overflow_buckets(HashTable* table) {
    LinkList** buckets = (LinkList**)new LinkList(); ;
    buckets = (LinkList**)Lcalloc((((LinkList*)buckets)->ListAllocator), table->size, sizeof(LinkList*));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

static void free_overflow_buckets(HashTable* table) {
    LinkList** buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);
    ((LinkList*)buckets)->free(buckets);
}

Ht_item* create_item(const char* key, const char* value) {
    Ht_item* item = new Ht_item();
    item->key = (char*)item->SetKey();
    item->value = (char*)item->SetValue();

    strcpy((char*)item->key, key);
    strcpy((char*)item->value, value);
    return item;
}

HashTable* create_table(int size) {
    HashTable* table = new HashTable();
    table = (HashTable*)table->HashAllocator->Allocate(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)Lcalloc(table->HashAllocator, table->size, sizeof(Ht_item*));
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    item->ItemAllocator->Free((void*)item->key);
    item->ItemAllocator->Free((void*)item->value);
    item->ItemAllocator->Free(item);
}

void free_table(HashTable* table) {

    for (int i = 0; i < table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    table->HashAllocator->Free(table->items);
    table->HashAllocator->Free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {
    LinkList* head = table->overflow_buckets[index];

    if (head == NULL) {
        head = new LinkList();
        head->item = item;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable* table, const char* key, const char* value) {

    Ht_item* item = create_item(key, value);
    unsigned long index = hash_function(key);
    Ht_item* current_item = table->items[index];

    if (current_item == NULL) {
        if (table->count == table->size) {
            printf("Ошибка! Таблица заполнена\n");
            free_item(item);
            return;
        }

        table->items[index] = item;
        table->count++;
    }

    else {
        if (strcmp(current_item->key, key) == 0) {
            strcpy((char*)table->items[index]->value, value);
            return;
        }

        else {
            handle_collision(table, index, item);
            return;
        }
    }
}

const char* ht_search(HashTable* table, const char* key) {

    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkList* head = table->overflow_buckets[index];

    while (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->value;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, const char* key) {

    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkList* head = table->overflow_buckets[index];

    if (item == NULL) {
        return;
    }
    else {
        if (head == NULL && strcmp(item->key, key) == 0) {
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL) {
            if (strcmp(item->key, key) == 0) {

                free_item(item);
                LinkList* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkList* curr = head;
            LinkList* prev = NULL;

            while (curr) {
                if (strcmp(curr->item->key, key) == 0) {
                    if (prev == NULL) {
                        linkedlist_remove(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else {
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }
                curr = curr->next;
                prev = curr;
            }

        }
    }
}

void print_search(HashTable* table, const char* key) {
    const char* val;
    if ((val = ht_search(table, key)) == NULL) {
        printf("% значение не найдено\n", key);
        return;
    }
    else {
        printf("Ключ:%s, Значение:%s\n", key, val);
    }
}

void print_table(HashTable* table) {
    printf("\n-------------------\n");
    for (int i = 0; i < table->size; i++) {
        if (table->items[i]) {
            printf("Индекс:%d, Ключ:%s, Значение:%s", i, table->items[i]->key, table->items[i]->value);
            if (table->overflow_buckets[i]) {
                printf(" => Доп. значения => ");
                LinkList* head = table->overflow_buckets[i];
                while (head) {
                    printf("Ключ:%s, Значение:%s ", head->item->key, head->item->value);
                    head = head->next;
                }
            }
            printf("\n");
        }
    }
    printf("-------------------\n");
}


