#include "list.h"

static int Count = 0;

struct List {
	void item;
	struct List* next;
};

struct Lists {
	struct List* list;
} ListVector[MAX_LIST];

struct
List* NewList() {
	struct List* newList = (struct List*) malloc(sizeof(struct List)); 
	newList->item = -1;
	newList->next = NULL;
	return newList;
}

void InitVectorList() {
	for (int i=0; i < MAX_LIST; ++i)
		ListVector[i].list = NULL;
}

list CreateList(void) {
	if (Count > MAX_LIST) return ERROR;
	if (Count == 0) InitVectorList();
	ListVector[Count].list = NewList();
	list fd = Count++;
	return fd;
}

void DestroyList(list fd) {
	if (Count < 0 || Count >= MAX_LIST || 
			ListaVector[fd].list !=NULL) 
		return;
	struct List* toDestroy = ListaVector[fd].list;
	free(toDestroy);
	ListaVector[fd].list = NULL;
}
	

