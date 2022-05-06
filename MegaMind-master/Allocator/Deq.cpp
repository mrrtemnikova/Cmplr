
#include "Allocator.h"

template <class T>
class Deq
{
public:
	struct Node
	{
		T data;
		Node* next;
		Node* prev;
	};
	Node* head;
	Node* tail;
	Allocator* allocator;

	//							<-> head |<->|<->| | | |<->| <-> tail
	
	Deq(){
		allocator = new Allocator(4096, 8);
	}

	void CF(T data)
	{
		Node* nNode = (Node*)allocator->Allocate(sizeof(data));
		head = nNode;
		tail = nNode;
		tail->data = data;
	}
	//Â íà÷àëî ñïèñêà
	void PushH(T data)
	{
		if (head == NULL)
			CF(data);
		else
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			nNode->next = head;
			head->prev = nNode;
			head = nNode;
			nNode->data = data;
		}
	}
	//Â êîíåö ñïèñêà
	void PushT(T data)
	{
		if (tail == NULL)
			CF(data);
		else
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			nNode->prev = tail;
			tail->next = nNode;
			tail = nNode;
			nNode->data = data;
		}
	}
	//Óäàëåíèå èç íà÷àëà
	T PopH()
	{
		if (head == NULL)
			return NULL;
		head = head->next;
		if (head == NULL)
			tail = NULL;
		allocator->Free((void*)head->prev);
		return head->prev->data;
	}
	//Óäàëåíèå èç êîíöà
	T PopT()
	{
		Node* temp = tail;
		tail = tail->prev;


		if (tail == NULL)
			head = NULL;
		else
			tail->next = NULL;

		allocator->Free((void*)tail);
		return temp->data;
	}
};
