#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

template<class T>
class List
{
public:
    struct Node
    {
        T data;
        Node* next;


        bool isAvailable = true;
    };
    Node* head = NULL;


    void Push(Node* newNode)
    {
        newNode->next = head;
        newNode->isAvailable = true;
        head = newNode;
    }


    Node* Pop()
    {
        Node* top = head;
        if (top == NULL) {
            return NULL;
        }
        head = head->next;
        return top;
    }

    void ShowSegments(Node* startNode) {
        Node* curr = startNode;

        int i = 0;
        while (curr->next != NULL) {
            cout << "Сегмент [" << i << "] -> " << curr << endl;
            i++;
            curr = curr->next;
        }
    }

    //void Show() 
    //{
    //    Node<T>* current = this->head;
    //    if (count == 0) {
    //        cout << "Список пуст" << endl;
    //    }
    //    else {
    //        for (int i = 0;i < count;i++) {
    //            if (i == 0) {
    //                cout << "\t________________________________________________________" << endl;
    //                cout << "\t|\tИндекс\t|\tКлюч\t|\tЗначение\t|" << endl;
    //                cout << "\t--------------------------------------------------------" << endl;
    //            }
    //            cout << "\t|\t   " << i << "\t|\t" << current->isAvailable << "  \t|\t" << "\t\t|" << endl;
    //            cout << "\t--------------------------------------------------------" << endl;
    //            current = current->pNext;
    //        }
    //    }
    //}
};
