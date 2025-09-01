// Copyright (c) 2025 Abdulrahman Mohammad Salem. All rights reserved.
// Licensed under SALEM PERSONAL-USE LICENSE (SPUL) v1.0.
// Use: private/personal projects only. No fork/modify/redistribute.
// Fingerprint: SLM-2025-FP-18A3QE
// Contact: abdulrahman.mohammad.salem@gmail.com

#pragma once

#include <stdexcept> //To throw runtime errors when needed
#include <sstream>
#include <vector>

using namespace std;

template <class T>
class DoublyLinkedList {
public:
	struct Node {
		T value;
		Node * next;
		Node * prev;
	};

private:
	Node * _Head; //This pointer must always be kept pointing at the first node of the list.
	Node * _Tail; //This pointer must always be kept pointing at the last node of the list.
	size_t _Size; //This value must be updated dynamically within each method manipulating the list.

public:
	//----Constructors:

	//Time Complexity: O(1)
	DoublyLinkedList() {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;
	}

	//Time Complexity: O(n)
	DoublyLinkedList(const vector<T> & vct) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (const T & E : vct)
			pushBack(E);
	}

	//Time Complexity: O(n)
	DoublyLinkedList(const initializer_list<T> & initializer) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (const T & E : initializer)
			pushBack(E);
	}

	//Time Complexity: O(n)
	DoublyLinkedList(size_t requiredSize, const T & value) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (requiredSize; requiredSize > 0; requiredSize--)
			pushBack(value);
	}

	//Time Complexity: O(n)
	DoublyLinkedList(const DoublyLinkedList<T> & obj) {
		_Head = nullptr;
		_Tail = nullptr;
		_Size = 0;

		for (Node * _curr = obj._Head; _curr; _curr = _curr->next)
			pushBack(_curr->value);
	}

	//----Destructor:

	//Time Complexity: O(n)
	~DoublyLinkedList() {
		clear(); //To free the memory after a SinglyLinkedList object goes out-of-scope.
	}

	//Time Complexity: O(n)
	Node * getFirstNodeByValue(const T & value) const {
		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			if (_curr->value == value)
				return _curr;

		return nullptr;
	}

	//Time Complexity: O(n)
	Node * getNodeByIndex(const size_t & index) const {
		if (index >= _Size) //If the index exceeds the list size
			throw out_of_range("OutOfRangeException: Invalid index value [getNodeByIndex(const size_t&)]");

		Node * _curr = nullptr;

		if (index < _Size / 2) {
			//Traversing from the head:
			_curr = _Head;

			for (size_t i = 0; i < index; i++)
				_curr = _curr->next;
		}
		else {
			//Traversing from the tail:
			_curr = _Tail;

			for (size_t i = _Size - 1; i > index; i--)
				_curr = _curr->prev;
		}
	
		return _curr;
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
		return getNodeByIndex(index)->value;
	}

	//Time Complexity: O(n)
	void setValue(const size_t & index, const T & value) {
		getNodeByIndex(index)->value = value;
	}

	//Time Complexity: O(1)
	void pushFront(const T & value) {
		Node * new_node = new Node();

		new_node->value = value;
		new_node->prev = nullptr;
		new_node->next = _Head;

		if (!_Head) //If the list was empty
			_Tail = new_node;
		else
			_Head->prev = new_node;

		_Head = new_node;
		_Size++;
	}

	//Time Complexity: O(1)
	void pushBack(const T & value) {
		Node * new_node = new Node();

		new_node->value = value;
		new_node->next = nullptr;
		new_node->prev = _Tail;

		if (!_Head) { //If the list was empty
			_Head = new_node;
			_Tail = new_node;
		}
		else {
			_Tail->next = new_node;
			_Tail = new_node;
		}

		_Size++;
	}


	//Time Complexity: O(1)
	void pushAfter(Node *& target_node, const T & value) {
		Node * new_node = new Node();
		new_node->value = value;

		if (_Tail == target_node) { //If we insert at the end
			_Tail = new_node;
			new_node->next = nullptr;
		}
		else {
			new_node->next = target_node->next;
			target_node->next->prev = new_node;
		}

		new_node->prev = target_node;
		target_node->next = new_node;

		_Size++;
	}

	//Time Complexity: O(n)
	void pushAfter(const size_t & target_index, const T & value) {
		pushAfter(getNodeByIndex(target_index), value);
	}


	//Time Complexity: O(1)
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
		if (!_Head) //If the node is empty
			return;

		Node * target = _Head;
		_Head = _Head->next;

		if (target == _Tail) //If the list contains only one node
			_Tail = nullptr;
		else
			target->next->prev = nullptr;

		delete target;
		_Size--;
	}

	//Time Complexity: O(1)
	void eraseBack() {
		if (!_Head) //If the list is empty
			return;

		Node * target = _Tail;
		_Tail = _Tail->prev;

		if (target == _Head) //If the list contains only one node
			_Head = nullptr;
		else
			target->prev->next = nullptr;

		delete target;
		_Size--;
	}

	//Time Complexity: O(1)
	//Time complexity with validation: O(n)
	void erase(Node *& target, const bool & validateNode) {
		if (validateNode && (!(target && doesContain(target)))) //If the target is nullptr or doesn't exist in the list
			throw invalid_argument("InvalidArgumentException: [deleteNode(Node*&)]");

		if (target == _Head && target == _Tail) { //If the target is the only node
			_Head = nullptr;
			_Tail = nullptr;
			delete target;
			_Size--;
			return;
		}

		if (target == _Head) { //If the target is the first node
			target->next->prev = nullptr;
			_Head = target->next;
		}
		else
			target->prev->next = target->next;

		if (target == _Tail) { //If the target is the last node
			target->prev->next = nullptr;
			_Tail = target->prev;
		}
		else
			target->next->prev = target->prev;

		delete target;
		_Size--;
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
		for (Node * _curr = _Head->next; _curr; _curr = _curr->next) {
			if (_curr->value == value) {
				if (_curr == _Tail) //If the last element is to be deleted
					_Tail = _curr->prev;

				_curr->prev->next = _curr->next;
				_curr->next->prev = _curr->prev;
				delete _curr;
				_Size--;

				return;
			}
		}
	}

	//Time Complexity: O(n)
	void eraseAllOccurrences(const T & target_value) {
		Node * _curr = _Head;
		Node * _next = nullptr;

		while (_curr) {
			_next = _curr->next; //We must "rescue" the next node!

			if (_curr->value == target_value)
				erase(_curr, false);

			_curr = _next;
		}
	}

	//Time Complexity: O(n)
	void eraseAdjacentDuplicates() {
		Node * _curr = _Head;
		Node * _next = nullptr;

		while (_curr) {
			_next = _curr->next; //We must "rescue" the next node!

			if (_curr->next && _curr->value == _curr->next->value)
				erase(_curr, false);
				
			_curr = _next;
			
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

		swap(_Head, _Tail);

		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			swap(_curr->next, _curr->prev);
	}

	//Time Complexity: O(n)
	void extendWith(const DoublyLinkedList<T> & otherList) {
		for (Node * _curr = otherList._Head; _curr; _curr = _curr->next)
			pushBack(_curr->value);
	}

	//Time Complexity: O(n)
	void extendWith(const vector<T> & vct) {
		for (const T & E : vct)
			pushBack(E);
	}

	//Time Complexity: O(n)
	void overrideWith(const DoublyLinkedList<T> & otherList) {
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
	string toStringForward(const string & separator = ", ") const {
		if (!_Head) //If the list is empty
			return "[]";

		ostringstream _result;

		_result << '[' << _Head->value;

		for (Node * _curr = _Head->next; _curr; _curr = _curr->next)
			_result << separator << _curr->value;

		_result << ']';

		return _result.str();
	}

	//Time Complexity: O(n)
	vector<T> toVectorForward() const {
		vector<T> vList;

		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			vList.push_back(_curr->value);

		return vList;
	}

	//Time Complexity: O(n)
	string toStringBackward(const string & separator = ", ") const {
		if (!_Head) //If the list is empty
			return "[]";

		ostringstream _result;

		_result << '[' << _Tail->value;

		for (Node * _curr = _Tail->prev; _curr; _curr = _curr->prev)
			_result << separator << _curr->value;

		_result << ']';

		return _result.str();
	}

	//Time Complexity: O(n)
	vector<T> toVectorBackward() const {
		vector<T> vList;

		for (Node * _curr = _Tail; _curr; _curr = _curr->prev)
			vList.push_back(_curr->value);

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

		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			if (_curr->value == value)
				counter++;

		return counter;
	}

	//Time Complexity: O(1)
	bool isEmpty() const {
		return !_Size;
	}

	//Time Complexity: O(n)
	bool equals(const DoublyLinkedList & otherList) const {
		if (_Size != otherList._Size) //Early exit
			return false;

		Node * _otherHead = otherList._Head;

		for (Node * _curr = _Head; _curr; _curr = _curr->next) {
			if (_curr->value != _otherHead->value)
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

		for (Node * _curr = _Head; _curr; _curr = _curr->next)
			if (_curr == node)
				return true;

		return false;
	}

	//Operator overloading for accessing elements with [] like vectors & arrays
	T operator[] (const size_t & index) {
		return getValue(index);
	}

	//Operator overloading to use the equals() method with == & !=
	bool operator== (const DoublyLinkedList & otherList) {
		return equals(otherList);
	}

	bool operator!= (const DoublyLinkedList & otherList) {
		return !equals(otherList);
	}

	//Operator overloading to quickly override the list
	void operator= (const vector<T> & vct) {
		overrideWith(vct);
	}

	void operator= (const DoublyLinkedList<T> & otherList) {
		overrideWith(otherList);
	}

	//Operator overloading to quickly append on the list
	void operator+= (const vector<T> & vct) {
		extendWith(vct);
	}

	void operator+= (const DoublyLinkedList<T> & otherList) {
		extendWith(otherList);
	}

};
