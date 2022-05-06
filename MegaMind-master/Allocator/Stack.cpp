#include "Allocator.h"

template <class T>
class Stack
{
public:
	struct Node
	{
		T data;
		Node* next;
	};
	Node* head;

	Allocator* allocator;


	Stack() {
		allocator = new Allocator(4096, 8);
	}

	//							 head ->|->|->| | | |->| 

	void Push(T data)
	{
		if (head == NULL)
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			head = nNode;
		}
		else
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			nNode->next = head;
			head = nNode;
		}
		head->data = data;
	}


	T Pop()
	{
		if (head == NULL)
			return NULL;
		Node* temp;
		temp = head;
		head = head->next;
		allocator->Free((void*) temp);
		return temp->data;
	}
};