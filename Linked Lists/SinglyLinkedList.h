// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahmansalem.contact@gmail.com

#pragma once

#include <stdexcept> //To throw runtime errors when needed
#include <sstream>
#include <vector>

using namespace std;

template <class T>
class SinglyLinkedList {
public:
	struct Node {
	    T value;
		Node * next;
	};

private:
	Node * _Head; //This pointer must always be kept pointing at the first node of the list.
	Node * _Tail; //This pointer must always be kept pointing at the last node of the list.
	size_t _Size; //This value must be updated dynamically within each method manipulating the list.

public:
	//----Constructors:

	//Time Complexity: O(1)
	SinglyLinkedList() {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;
	}
	
	//Time Complexity: O(n)
	SinglyLinkedList(const vector<T> & vct) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;
		
		for (const T & E : vct)
			pushBack(E);
	}

	//Time Complexity: O(n)
	SinglyLinkedList(const initializer_list<T> & initializer) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (const T & E : initializer)
			pushBack(E);
	}

	//Time Complexity: O(n)
	SinglyLinkedList(size_t requiredSize, const T & value) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (requiredSize; requiredSize > 0; requiredSize--)
			pushBack(value);
	}

	//Time Complexity: O(n)
	SinglyLinkedList(const SinglyLinkedList<T> & obj) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (Node * _head = obj._Head; _head; _head = _head->next)
			pushBack(_head->value);
	}
	
	//----Destructor:
	
	//Time Complexity: O(n)
	~SinglyLinkedList() {
		clear(); //To free the memory after a SinglyLinkedList object goes out-of-scope.
	}

	//----Main Methods:

	//Time Complexity: O(n)
	Node * getFirstNodeByValue(const T & value) const {
		for (Node * _head = _Head; _head; _head = _head->next)
			if (_head->value == value)
				return _head;

		return nullptr;
	}

	//Time Complexity: O(n)
	Node * getNodeByIndex(const size_t & index) const {
		if (index >= _Size) //If the index exceeds the list size
			throw out_of_range("OutOfRangeException: Invalid index value [getNodeByIndex(const size_t&)]");

		Node * _head = _Head;

		//Traverse until the desired node:
		for (size_t i = 0; i < index; i++)
			_head = _head->next;

		return _head;
	}

	//Time Complexity: O(1)
	Node * getHead() const {
		return _Head;
	}

	//Time Complexity: O(1)
	Node * getTail() const {
		return _Tail;
	}

	//Time Complexity: O(1)
	T getFrontValue() const {
		return _Head->value;
	}

	//Time Complexity: O(1)
	T getBackValue() const {
		return _Tail->value;
	}

	//Time Complexity: O(n)
	T getValue(const size_t & index) const {
		if (index >= _Size) //If the index exceeds the size of the list
			throw out_of_range("OutOfRangeException [getValue(const size_t)]");

		Node * _head = _Head;

		//Traversing the list until the desired node:
		for (size_t i = 0; i < index; i++)
			_head = _head->next;

		//Now the _curr is "standing" on the desired node

		return _head->value;
	}

	//Time Complexity: O(n)
	void setValue(const size_t & index, const T & value) {
		getNodeByIndex(index)->value = value;
	}

	//Time Complexity: O(1)
	void pushFront(const T & value) {
		Node * new_node = new Node();

		if (!_Head) //If the list is empty
			_Tail = new_node;

		new_node->value = value;
		new_node->next = _Head;

		_Head = new_node;
		_Size++;
	}

	//Time Complexity: O(1)
	void pushBack(const T & value) {
		Node * new_node = new Node();

		new_node->value = value;
		new_node->next = nullptr;

		if (!_Head) { //If the list was empty
			_Head = new_node;
			_Tail = new_node;
			_Size++;

			return;
		}
		
		_Tail->next = new_node;
		_Tail = new_node;

		_Size++;
	}

	//Time Complexity: O(1)
	void pushAfter(Node * target_node, const T & value) {
		if (target_node == nullptr)
			throw invalid_argument("InvalidArgumentException: target is null [pushAfter(Node*, const T&)]");

		Node * new_node = new Node();

		new_node->value = value;
		new_node->next = target_node->next;

		if (target_node == _Tail)
			_Tail = new_node;
		else
			target_node->next = new_node;

		_Size++;
	}

	//Time Complexity: O(n)
	void pushAfter(const size_t & target_index, const T & value) {
		pushAfter(getNodeByIndex(target_index), value);
	}

	//Time Complexity: O(n)
	void push(const size_t & index, const T & value) {
		if (index > _Size) //If the index exceeds the size
			throw out_of_range("OutOfRangeException: invalid index value [push(const size_t&, const T&)]");

		if (index == 0)
			pushFront(value);
		else if (index == _Size)
			pushBack(value);
		else
			pushAfter(getNodeByIndex(index), value);
	}

	//Time Complexity: O(1)
	void eraseFront() {
		if (!_Head) //If the list is empty
			throw out_of_range("OutOfRangeException [eraseFront()]");

		if (_Head == _Tail) //If the list consists of only one element
			_Tail = nullptr;

		Node * first_node = _Head;
		_Head = _Head->next;
		delete first_node;
		_Size--;
	}

	//Time Complexity: O(n)
	void eraseBack() {
		if (!_Head) //If the list is empty
			throw out_of_range("OutOfRangeException [eraseBack()]");

		Node * _head = _Head;

		while (_head->next->next)
			_head = _head->next;

		//The _curr is now "standing" on the node before the end.

		_head->next = nullptr;
		delete _Tail;
		_Tail = _head;
		_Size--;
	}

	//Time Complexity: O(n)
	bool erase(Node * target, const bool & validateNode) {
		if (validateNode && (!(target && doesContain(target)))) //If the target is null
			throw invalid_argument("InvalidArgumentException: target is null [erase(Node*&)]");

		if (target == _Head) { //If the first node is the target
			if (target == _Tail) //If the list consisted only of the target
				_Tail = nullptr;

			_Head = _Head->next;
			delete target;
			_Size--;
			return true;
		}

		Node * _prev = _Head; //This will point at the node before the target

		//Traversing until the node before the target
		while (_prev && _prev->next != target)
			_prev = _prev->next;
		
		if (!_prev) //If the target does not exist in the list
			return false;

		if (target == _Tail) //If the target is the last element
			_Tail = _prev;

		_prev->next = target->next;
		delete target;
		_Size--;
		return true;
	}

	//Time Complexity: O(n)
	void erase(const size_t & index) {
		if (index >= _Size) //If the index exceeds the size
			throw out_of_range("OutOfRangeException: invalid index value [push(const size_t&, const T&)]");

		if (index == 0)
			eraseFront();
		else if (index == _Size - 1)
			eraseBack();
		else
			erase(getNodeByIndex(index), false);
	}

	//Time Complexity: O(n)
	void eraseFirstOccurrence(const T & value) {
		if (!_Head) //If the list is empty
			throw out_of_range("OutOfRangeException: Said list is empty [eraseFirstMatch(const T&)]");

		if (_Head->value == value) { //If the first element is to be deleted
			eraseFront();
			return;
		}

		//Searching for the element to be deleted:
		Node * _prev = _Head;

		for (Node * _curr = _Head->next; _curr; _curr = _curr->next) {
			if (_curr->value == value) {
				if (_curr == _Tail) //If the last element is to be deleted
					_Tail = _prev;

				_prev->next = _curr->next;
				delete _curr;
				_Size--;

				return;
			}

			_prev = _curr;
		}
	}
	
	//Time Complexity: O(n)
	void eraseAllOccurrences(const T & target_value) {
		Node * _curr = _Head;
		Node * _prev = nullptr;
		Node * _node_to_delete = nullptr;

		while (_curr) {
			if (_curr->value == target_value) {
				if (_curr == _Head)
					_Head = _Head->next; //We must preserve the _Head if its node is to be deleted

				if (_curr == _Tail)
					_Tail = _prev; //We must preserve the _Tail if its node is to be deleted

				if (_prev) //If we are not deleting the first element
					_prev->next = _curr->next; //Re-linking before deleting

				//We will not update the _prev here, because we will delete the current node -- The _prev must stay in-place
				_node_to_delete = _curr;
				_curr = _curr->next;
				delete _node_to_delete;
				_Size--;
			}
			else {
				_prev = _curr;
				_curr = _curr->next;
			}
		}
	}

	//Time Complexity: O(n)
	void eraseAdjacentDuplicates() {
		Node * _curr = _Head;
		Node * _prev = nullptr;
		Node * _node_to_delete = nullptr;

		while (_curr) {
			if (_curr->next && _curr->value == _curr->next->value) {
				if (_curr == _Head)
					_Head = _Head->next; //We must preserve the head if its node is to be deleted

				if (_prev) //If we are not deleting the first element
					_prev->next = _curr->next; //Re-linking before deleting

				_node_to_delete = _curr;

				//We will not update the _prev here, because we will delete current the node -- The _prev must stay in-place
				_curr = _curr->next;
				delete _node_to_delete;
				_Size--;
			}
			else {
				_prev = _curr;
				_curr = _curr->next;
			}
		}
	}

	//Time Complexity: O(n)
	void clear() {
		if (_Head) { //If the list is not empty
			Node * _temp = nullptr;

			while (_Head) {
				_temp = _Head;
				_Head = _Head->next;

				delete _temp;
			}

			_Tail = nullptr;
			_Size = 0;
			//The _Head is already null at this point.
		}
	}

	//Time Complexity: O(n)
	void reverse() {
		if (!_Head) //If the list is empty
			return;

		_Tail = _Head;

		Node * _curr = _Head;
		Node * _prev = nullptr;

		while (_Head) {
			_Head = _Head->next;

			_curr->next = _prev;

			_prev = _curr;
			_curr = _Head;
		}

		//At this point, the _prev is now the new head.
		_Head = _prev;
	}

	//Time Complexity: O(n)
	void extendWith(const SinglyLinkedList<T> & otherList) {
		for (Node * _curr = otherList._Head; _curr; _curr = _curr->next)
			pushBack(_curr->value);
	}

	//Time Complexity: O(n)
	void extendWith(const vector<T> & vct) {
		for (const T & E : vct)
			pushBack(E);
	}

	//Time Complexity: O(n)
	void overrideWith(const SinglyLinkedList<T> & otherList) {
		clear(); //To free the list

		extendWith(otherList);
	}
	
	//Time Complexity: O(n)
	void overrideWith(const vector<T> & vct) {
		clear(); //To free the list

		extendWith(vct);
	}

	//Time Complexity: O(n)
	void fill(size_t requiredSize, const T & value) {
		clear(); //To override the list with the new nodes

		for (requiredSize; requiredSize > 0; requiredSize--)
			pushBack(value);
	}

	//Time Complexity: O(n)
	string toString(const string & separator = ", ") const {
		if (!_Head) //If the list is empty
			return "[]";

		ostringstream _result;

		_result << '[' << _Head->value;

		for (Node * _head = _Head->next; _head; _head = _head->next)
			_result << separator << _head->value;

		_result << ']';
		
		return _result.str();
	}

	//Time Complexity: O(n)
	vector<T> toVector() const {
		vector<T> vList;

		for (Node * _head = _Head; _head; _head = _head->next)
			vList.push_back(_head->value);

		return vList;
	}

	//Time Complexity: O(1)
	size_t getSize() const {
		return _Size;
	}

	//Time Complexity: O(n)
	size_t getValueCount(const T & value) const {
		if (!_Head) //If the list is empty
			return 0;

		size_t counter = 0;

		for (Node * _head = _Head; _head; _head = _head->next)
			if (_head->value == value)
				counter++;

		return counter;
	}

	//Time Complexity: O(1)
	bool isEmpty() const {
		return !_Size;
	}

	//Time Complexity: O(n)
	bool equals(const SinglyLinkedList & otherList) const {
		if (_Size != otherList._Size) //Early exit
			return false;

		Node * _otherHead = otherList._Head;

		for (Node * _head = _Head; _head; _head = _head->next) {
			if (_head->value != _otherHead->value)
				return false;

			_otherHead = _otherHead->next;
		}

		return true;
	}

	//Time Complexity: O(n)
	bool doesContain(const T & value) const {
		if (!_Head) //If the list is empty
			return false;

		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			if (_curr->value == value)
				return true;

		return false;
	}

	//Time Complexity: O(n)
	bool doesContain(Node *& node) const {
		if (!_Head) //If the list is empty
			return false;

		for (Node * _head = _Head; _head; _head = _head->next)
			if (_head == node)
				return true;

		return false;
	}

	//Operator overloading for accessing elements with [] like vectors & arrays
	T operator[] (const size_t & index) {
		return getValue(index);
	}

	//Operator overloading to use the equals() method with == & !=
	bool operator== (const SinglyLinkedList & otherList) {
		return equals(otherList);
	}

	bool operator!= (const SinglyLinkedList & otherList) {
		return !equals(otherList);
	}

	//Operator overloading to quickly override the list
	void operator= (const vector<T> & vct) {
		overrideWith(vct);
	}

	void operator= (const SinglyLinkedList<T> & otherList) {
		overrideWith(otherList);
	}

	//Operator overloading to quickly append on the list
	void operator+= (const vector<T> & vct) {
		extendWith(vct);
	}

	void operator+= (const SinglyLinkedList<T> & otherList) {
		extendWith(otherList);
	}

};

