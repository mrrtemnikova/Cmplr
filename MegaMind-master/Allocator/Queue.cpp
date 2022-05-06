
#include "Allocator.h"

template <class T>
class Queue
{
public:
	struct Node
	{
		T data;
		Node* next;
	};
	Node* head;
	Node* tail;


	Allocator* allocator;

	//							<- head |->|->| | | |->| <- tail
	
	Queue(){
		allocator = new Allocator(4096, 8);
	}

	void Push(T data)
	{
		if (head == NULL)
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			head = nNode;
			tail = nNode;
		}
		else
		{
			Node* nNode = (Node*)allocator->Allocate(sizeof(data));
			tail->next = nNode;
			tail = nNode;
		}
		tail->data = data;
	}


	T Pop()
	{
		if (head == NULL)
			return NULL;
		Node* temp;
		temp = head;
		head = head->next;
		if (head == NULL)
			tail = NULL;
		allocator->Free((void*)temp);
		return temp->data;
	}
};
