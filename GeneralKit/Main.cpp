#include<stdio.h>
#include<stdlib.h>
#include"Container.h"

bool Run = true;
int Count = 0;
DWORD RandomSeekThreadProc(LPVOID L) {
	List<int>*A = (List<int>*)L;
	int id = Count;
	Count++;
	while (Run) {
		int a = rand();
		A->Add(0, a);
	}
	return 0;
}

int main() {
	
	return 0;
}