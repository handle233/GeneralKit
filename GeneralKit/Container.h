#pragma once
/**
* project : 物理和渲染模型
* file    : Container.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"

class IndexOverflow :public Exception {
public:
	IndexOverflow() :Exception(EXPT_ERROR, "Index Overflow") { }
};
class IndexUnderflow :public Exception {
public:
	IndexUnderflow() :Exception(EXPT_ERROR, "Index Underflow") { }
};

template<typename Type>
class List {
	friend class Node;
private:
	SRWLOCK Lock;
	class Node{
	private:
		Type* Data;
		Node* NextNode;
		Node* LastNode;
	public:
		operator Type& () {
			return *Data;
		}
		Node(Type const* pData) {
			Data = new Type;
			memcpy(Data, pData, sizeof(Type));
			NextNode = nullptr;
			LastNode = nullptr;
		}
		Node* operator >>(Node* Nxt) {
			Node* Last = NextNode;
			NextNode = Nxt;
			return Last;
		}
		Node* operator <<(Node* Lst) {
			Node* Last = LastNode;
			LastNode = Lst;
			return Last;
		}
		Node* operator +(int num) {
			Node* TheNode = this;
			for (int a = 0; a < num; a++) {
				if (TheNode == nullptr) {
					throw IndexOverflow();
				}
				TheNode = TheNode->NextNode;
			}
			return TheNode;
		}
		Node& operator -(int num) {
			Node* TheNode = this;
			for (int a = 0; a < num; a++) {
				TheNode = TheNode->LastNode;
				if (TheNode == nullptr) {
					throw IndexUnderflow();
				}
			}
			return *TheNode;
		}
		~Node() {
			NextNode == nullptr ? 0 : *NextNode << LastNode;
			LastNode == nullptr ? 0 : *LastNode >> NextNode;
			delete Data;
		}
	};

	Node* Root;
	int Length;

	Node* CacheNode;
	int CacheIndex;

	Node* ToThatNode(int ToIndex) {
		//initalize
		IndexLegal(ToIndex);
		Node* CurNode = nullptr;
		int Index = 0;
		//Check cache
		if (CacheHit(ToIndex)) {
			CurNode = CacheNode;
			Index = CacheIndex;
			//cout << "Cache hitted." << CurNode->GetData()<< "\t" << Index << endl;
		}
		else {
			CurNode = Root;
		}
		//do
		CurNode = *CurNode + (ToIndex - Index);

		CacheIndex = ToIndex;
		CacheNode = CurNode;
		//cout << "Cache updated" << CacheNode->GetData() << "\t" << CacheIndex << endl;
		return CurNode;
	}
	void IndexLegal(int Index) {
		if (Index < 0) {
			throw IndexUnderflow();
		}
		if (Index >= Length && Length != 0 && Index != 0) {
			throw IndexOverflow();
		}
	}
	bool CacheHit(int Index) {
		if (CacheNode == nullptr) {
			return false;
		}
		if (Index >= CacheIndex) {
			return true;
		}
		return false;
	}
public:
	const int& length = Length;
	List() {
		InitializeSRWLock(&Lock);
		Root = nullptr;
		Length = 0;
		CacheIndex = 0;
		CacheNode = nullptr;
	}
	bool Add(int Index, const Type& Data);
	Type& operator[](int Index) {
		AcquireSRWLockShared(&Lock);
		IndexLegal(Index);
		Node* Cur = ToThatNode(Index);
		if (Cur == nullptr) {
			cout << "Bug 来啦！！" << endl;
		}
		ReleaseSRWLockShared(&Lock);
		return *Cur;
	}
	void Del(int Index) {
		AcquireSRWLockExclusive(&Lock);
		Node* Cur = ToThatNode(Index);
		if (Index == 0) {
			Root = *Cur + 1;
			delete Cur;
			if (CacheIndex == Index) {
				CacheIndex = 0;
				CacheNode = nullptr;
			}
			Length--;
			ReleaseSRWLockExclusive(&Lock);
			return;
		}
		if (CacheIndex == Index) {
			CacheIndex = 0;
			CacheNode = nullptr;
		}
		delete Cur;
		Length--;
		ReleaseSRWLockExclusive(&Lock);
	}
	void operator delete(void* p) {
		List* pL = (List*)p;
		while (pL->Length != 0) {
			pL->Del(0);
		}
	}

	/*
	* Untested
	*/
	int Seek(const Type& Data) {
		Node Cur = *Root;
		for (int a = 0; a < length; a++) {
			if (memcmp(Data, (Type)Cur, sizeof(Type)) == 0) {
				return a;
			}
			Cur = Cur + 1;
		}
		return -1;
	}
};

template<typename Type>
inline bool List<Type>::Add(int Index, const Type& Data)
{
	AcquireSRWLockExclusive(&Lock);
	if (Index != -1) {
		IndexLegal(Index);
	}
	if (Index == -1) {
		Node* New = new Node(&Data);
		*New >> Root;
		*New << nullptr;
		Root==nullptr?0:*Root << New;
		Root = New;
		CacheIndex++;
		Length++;
		ReleaseSRWLockExclusive(&Lock);
		return true;
	}
	Node* Ins = ToThatNode(Index);
	Node* New = new Node(&Data);
	*New >> (*Ins + 1);
	*New << Ins;
	*Ins >> New;

	if (Index < CacheIndex) {
		CacheIndex++;
	}

	Length++;
	ReleaseSRWLockExclusive(&Lock);
	return true;
}
