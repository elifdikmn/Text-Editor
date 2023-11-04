#include <string>
 

using namespace std;

class Node {
public:
	Node() {
		next = 0;
	}
	Node(string el, Node* ptr = 0) {
		info = el; next = ptr;
	}
	string info = "";
	Node* next = NULL;
};

class LinkedList {
public:
	LinkedList() {
		head = tail = 0;
	}
	~LinkedList();
	bool isEmpty() {
		return head == 0;
	}
	void traverse(int, int);
	void insertStart(string);
	void insertEnd(string);
	void removeAll();
	string getTextAt(int);
	string removeFromStart();
	string removeFromEnd();
	string removeAt(int);
	void moveNode(int, int);
	string switchNode(int, string);
	void insertt(int, string);
	
	void saveToFile(string) const;

private:
	Node* head, * tail;
};

