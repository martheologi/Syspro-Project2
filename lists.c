#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "structs.h"
#include "functions.h"

IDsListNode* initializeIDsList(int id){
    IDsListNode* head;

    head = malloc(sizeof(IDsListNode));
    head->next = NULL;
    head->id = id;

    return head;
}

int IDsListAppend(IDsListNode* head, int id){
    IDsListNode* newNode;
    IDsListNode* ptr = head;

    if(ptr->id == id){
        return 0;
    }

    while(ptr->next!=NULL){
        if(ptr->id == id){
            return 0;
        }
        ptr = ptr->next;
    }

    if(ptr->id == id){
        return 0;
    }

    newNode = malloc(sizeof(IDsListNode));
    newNode->next = NULL;
    newNode->id = id;

    ptr->next = newNode;
    return 1;
}

IDsListNode* IDsListRemove(IDsListNode* head, int id) {
    IDsListNode* next;
    IDsListNode* curr = head;
    if (curr == NULL) {
        printf("not found\n");
        return NULL;
    }
    else if (curr->id == id) {
        next = curr->next;
        free(curr);
        return next;
    }
    else {
        curr->next = IDsListRemove(curr->next, id);
        return curr;
    }
}

int numOfNodes(IDsListNode* head){
    IDsListNode* ptr = head;
    int c=0;

    while(ptr!=NULL){
        c++;
        ptr = ptr->next;
    }
    return c;
}

void FreeIDsList(IDsListNode* ptr){
    IDsListNode* current = ptr;
    IDsListNode* temp_next;

    while(current != NULL){
        temp_next = current;
        current = current->next;
        free(temp_next);
    }
}
