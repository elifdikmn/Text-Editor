#include <fstream>
#include <iostream>
#include "LinkedList.h"

using namespace std;

LinkedList::~LinkedList() {
	for (Node* p; !isEmpty(); ) {
		p = head->next;
		delete head;
		head = p;
	}
}

void LinkedList::saveToFile(string userFileName) const {
	ofstream MyFile(userFileName);
	if (MyFile.is_open())
	{
		for (Node* tmp = head; tmp != 0; tmp = tmp->next)
			MyFile << tmp->info << " " << endl;
		MyFile.close();
	}
}

void LinkedList::insertStart(string el) {
	if (head == NULL || tail == NULL)
		head = tail = 0;

	head = new Node(el, head);
	if (tail == 0)
		tail = head;
}

void LinkedList::insertEnd(string el) {
	if (head == NULL || tail == NULL)
		head = tail = 0;

	if (tail != 0) {      
		tail->next = new Node(el);
		tail = tail->next;
	}
	else head = tail = new Node(el);
}

string LinkedList::removeFromStart() {
	string deletedText = head->info;
	head = head->next;
	return deletedText;
}

string LinkedList::removeFromEnd() {
	if (head == NULL)
		return " ";

	if (head->next == NULL)
		return removeFromStart();

	Node* second_last = head;
	while (second_last->next->next != NULL)
		second_last = second_last->next;	

	delete (second_last->next);	


	second_last->next = NULL;	

	return " ";
}

void LinkedList::removeAll() {
	if (head != NULL)
	{
		Node* current = head;
		Node* next;

		while (current != NULL)
		{
			next = current->next;
			free(current);
			current = next;
		}

		head = NULL;
	}
}

void LinkedList::moveNode(int currentPos, int targetPos)
{
	//cout << "curr: " << currentPos << " tar: " << targetPos << endl;
	if (currentPos != targetPos)
	{
		Node* tmp = head;

		string nodeTxt = getTextAt(currentPos);
		removeAt(currentPos);
		insertt(targetPos, nodeTxt);
	}
}

string LinkedList::getTextAt(int pos)
{
	Node* tmp = head;
	string nodeTxt = "";

	for (int i = 0; tmp != NULL && i < pos; i++)
	{
		if ((pos - 1) == i)
			nodeTxt = tmp->info;

		tmp = tmp->next;
	}
	return nodeTxt;
}

string LinkedList::removeAt(int pos)
{
	string deletedText = " ";

	if (head != NULL)
	{
		Node* prev = head;

		if (pos == 1)
			return removeFromStart();

		for (int i = 1; head != NULL && i < pos - 1; i++)	 
			prev = prev->next;

		deletedText = prev->next->info; 
		Node* next = prev->next->next;	
		delete(prev->next);	
		prev->next = next;	
	}

	return deletedText;
}

string LinkedList::switchNode(int pos, string newText)
{
	string replacedText = "";
	if (head == NULL)
		return replacedText;

	Node* tmp = head;

	for (int i = 0; tmp != NULL && i < pos; i++)
	{
		replacedText = tmp->info;
		if ((pos - 1) == i)
			tmp->info = newText;

		tmp = tmp->next;
	}
	return replacedText;
}

void LinkedList::insertt(int pos, string newText)
{

	if (pos == 1)
	{
		insertStart(newText);
	}
	else
	{
		if (head != NULL)
		{
			Node* tmp = head;
			Node* newNode = new Node(newText);

			for (int i = 1; head != NULL && i < pos; i++)
			{
				if ((pos - 1) == i)
					newNode->next = tmp->next;
				else
					tmp = tmp->next;
			}

			tmp->next = newNode;
		}
	}

}

void LinkedList::traverse(int page, int size) {
	int i = 0;
	int lineCounter = 0;

	cout << endl;

	for (Node* tmp = head; tmp != 0; tmp = tmp->next)
	{
		i++;
		if (((page - 1) * size) < i)
		{
			if (lineCounter < size)
			{
				lineCounter++;
				cout << lineCounter + ((page - 1) * size) << " - " << tmp->info << " " << endl;
			}

			if (lineCounter == size)
				break;
		}
	}
	cout << endl << "--- Page " << page << " ---"<< endl;
}



