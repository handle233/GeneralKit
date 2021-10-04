#pragma once
/**
* project : 物理和渲染模型
* file    : Container.h
* Copyright 2021 handle All rights reserved.
* 缅怀袁隆平院士
**/
#include "CommonHead.h"

template<typename Type>
class List {
private:
	class Node {
	private:
		Type Data;
		Node* lst, * nxt;
		CRITICAL_SECTION Lock;
		List& parent;
	public:
		Node(List& par) :parent(par) {
			lst = nxt = nullptr;
			memset(&Data, 0, sizeof(Type));
			InitializeCriticalSection(&Lock);
			parent.length++;
		}
		~Node() {
			Node* a = this;
			if (nxt != nullptr && lst != nullptr) {
				*lst >> *nxt;
				*nxt << *lst;
			}
			DeleteCriticalSection(&Lock);
			parent.length--;
		}
		operator Type() {
			return Data;
		}
		Type Element() {
			return Data;
		}
		Node& operator =(const Type NData) {
			Data = NData;
			return *this;
		}
		Node& operator <<(Node& nlst) {
			lst = &nlst;
			return *lst;
		}
		Node& operator >>(Node& nnxt) {
			nxt = &nnxt;
			return *nxt;
		}
		inline Node& operator +(int Num) {
			int a = 0;
			Node* T = this;
			while (a < Num) {
				if (T == nullptr) {
					throw Indexflow();
				}
				T = T->nxt;
				a++;
			}
			return *T;
		}
		inline Node& operator --() {
			return *lst;
		}
	};
	int length;

	Node* CacheNode;
	int CacheIndex;
	CRITICAL_SECTION CacheLock;

	void FlushCache() {
		EnterCriticalSection(&CacheLock);
		CacheNode = nullptr;
		CacheIndex = 0;
		LeaveCriticalSection(&CacheLock);
	}
	void UpdateCache(Node& N, int CurInx) {
		EnterCriticalSection(&CacheLock);
		CacheNode = &N;
		CacheIndex = CurInx;
		LeaveCriticalSection(&CacheLock);
	}
	int CacheHit(Node** Node, int ToInx) {
		EnterCriticalSection(&CacheLock);
		if (ToInx > CacheIndex && CacheNode!=nullptr) {//Hit
			*Node = CacheNode;
			LeaveCriticalSection(&CacheLock);
			return CacheIndex;
		}
		*Node = &Root;
		LeaveCriticalSection(&CacheLock);
		return -1;
	}
public:
	Node Root;//这是列表的-1项，也是Before Begin 指针
	const int& Length = length;
	List() :Root(*this) {
		CacheNode = nullptr;
		CacheIndex = 0;
		length = 0;
		InitializeCriticalSection(&CacheLock);
	}
	List(List& Copies) :Root(*this) {
		length = 0;
		Node* C = &Copies.Root;

		C = &((*C) + Copies.length);
		Add(0, (*C));
	}
	~List() {
		Node* L = &(Root + 1), * N = &(Root + 2);
		while (length > 0) {
			delete L;
			L = N;
			if (length > 0) {
				N = &((*N) + 1);
			}
		}
	}
	Node& operator [](int Inx) {
		Node *T = nullptr;
		int i = CacheHit(&T,Inx);
		while (i < Inx) {
			T = &((*T) + 1);
			i++;
		}
		UpdateCache(*T, Inx);
		return *T;
	}

	bool Add(int Inx,Type Data) {
		Node* T = &Root;
		T = &((*T) + Inx);

		Node* N = new Node(*this);
		*N = Data;
		*N >> (*T) + 1;
		if (&((*T) + 1) != nullptr) {
			((*T) + 1) << *N;
		}
		*N << *T;
		*T >> *N;
		FlushCache();
		
		return true;
	}
	bool Insert(List& Source, Range& Copys, int ToInx = 0) {
		Node* T = &Source[Copys.low];
		for (int a = Copys.low; a < Copys.high; a++) {
			Add(ToInx, (*T));
			T = &((*T) + 1);
		}
		FlushCache();
		return true;
	}

	class Indexflow :public Exception{
	public:
		Indexflow() :Exception(EXPT_ERROR,"Index over//under flow") {

		}
	};
};