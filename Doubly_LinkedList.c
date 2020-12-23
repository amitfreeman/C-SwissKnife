#include <stdio.h>
#include<stdlib.h>

/* Create doubly linked list, 
   and perform CRUD operations on it 
   DLL: 5 6 70 9 11 -1 */
   
typedef struct d_node{
    int data;
    struct d_node* next;
    struct d_node* prev;
}node;

node* HEAD=NULL;

/* append the new element at the end of DLL */
void append(node** head, int num){
    node* newNode=(node*)malloc(sizeof(node));
    newNode->data=num;
    newNode->next=NULL;
    
    /* if its head, and NULL
    then make new node as head */
    if((*head) == NULL){
      (*head)=newNode;
      newNode->prev=NULL;
      return;
    }
    
    /* else traverse till end
    and append newNode after end node */
    node* temp=(*head);
    
    while(temp->next != NULL){
        temp=temp->next;
    }
    temp->next=newNode;
    newNode->prev=temp;
    
    return;
}

/* push element at beginning of DLL */
void push(node** head, int num){
    node* newNode=(node*)malloc(sizeof(node));
    newNode->data=num;
    newNode->prev=NULL;
    
    /* DLL is empty create first */
    if( (*head)==NULL ){
        (*head)=newNode;
        newNode->next=NULL;
        return;
    }
    
    /* link at head */
    newNode->next=(*head);
    (*head)->prev=newNode;
    (*head)=newNode;
    
    return;
}

/* give a pointer after
   insert new node after this pointer */
void insertAfter(node* after, int num){
    node* newNode=(node*)malloc(sizeof(node));
    newNode->data=num;
    
    newNode->prev=after;
    newNode->next=after->next;
    
    if(after->next!=NULL)
       after->next->prev=newNode;
    
    after->next=newNode;
    return;
}

/* it prints DLL starting from HEAD node */
void printAll(){
    node* temp=HEAD;
    printf("\nLIST IS: ");
    while(temp!=NULL){
        printf("%d -> ", temp->data);
        temp=temp->next;
    }
    
    return;
}

void deleteDLL(){
    if(HEAD!=NULL){
     printf("\nDeleting nodes\n");
     node* temp=HEAD;
     node* prev=temp;
    
     while(temp->next!=NULL){
         prev=temp;
         temp=temp->next;
         free(prev);
     }
    }
    
    return;
}

int main()
{
    printf("Creating nodes and setting Doubly Linked List\n");
    append(&HEAD, 9);
    append(&HEAD, 11);
    push(&HEAD, 6);
    push(&HEAD, 5);
    printAll();
    //printf("\nHEAD->next->next->prev: %d\n", HEAD->next->next->prev->data);
    insertAfter(HEAD->next, 70);
    append(&HEAD, -1);
    
    printAll();
    
    deleteDLL();

    return 0;
}
