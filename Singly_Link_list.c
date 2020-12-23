#include <stdio.h>
#include <stdlib.h>

// Linked list node
typedef struct s_node {
  int data;
  struct s_node *next;
}node;

node *start = NULL;  // starting pointer to linked list

/* Common methods for 
   operation on LL */
void insert_at_begin(int);
void insert_at_end(int);
void traverse();
void delete_from_begin();
void delete_from_end();
int count = 0;

/* driver main function
   insert elements , print, 
   remove few elements, print*/
int main () {
  int i, data;

      data=2;
      insert_at_end(data);

      data=4;
      insert_at_begin(data);
      
      data=12;
      insert_at_end(data);

      traverse();

      delete_from_begin();

      delete_from_end();
      
       traverse();

  return 0;
}


/* create new node with data
   allocate pointer at start */
void insert_at_begin(int x) {
  //struct node *t;
  node *t;
  
  t= (node*) malloc(sizeof(node));
  t->data = x;
  count++;

  if (start == NULL) {
    start = t;
    start->next = NULL;
    return;
  }

  t->next = start;
  start = t;
}

/* create new node with data
   traverse whole LL
   assign pointer of new node at last location */
void insert_at_end(int x) {
  node *t, *temp;

  t = (node*)malloc(sizeof( node));
  t->data = x;
  count++;

  if (start == NULL) {
    start = t;
    start->next = NULL;
    return;
  }

  temp = start;

  while (temp->next != NULL)
    temp = temp->next;

  temp->next = t;
  t->next   = NULL;
}


/* traverse while
   next pointed by node is not empty */
void traverse() {
  node *t;

  t = start;

  if (t == NULL) {
    printf("Linked list is empty.\n");
    return;
  }

  printf("There are %d elements in linked list.\n", count);

  while (t->next != NULL) {
    printf("%d, ", t->data);
    t = t->next;
  }
  printf("%d\n", t->data); // Print last node
}

/* point start of LL to second element
   free past start node */
void delete_from_begin() {
  node *t;
  int n;

  if (start == NULL) {
    printf("Linked list is empty.\n");
    return;
  }

  n = start->data;
  t = start->next;
  free(start);
  start = t;
  count--;

  printf("%d deleted from the beginning successfully.\n", n);
}

/* Traverse till end 
   free last element 
   make next pointer as NULL for second last element*/
void delete_from_end() {
   node *t, *u;
  int n;

  if (start == NULL) {
    printf("Linked list is empty.\n");
    return;
  }

  count--;

  if (start->next == NULL) {
    n = start->data;
    free(start);
    start = NULL;
    printf("%d deleted from end successfully.\n", n);
    return;
  }

  t = start;

  while (t->next != NULL) {
    u = t;
    t = t->next;
  }

  n = t->data;
  u->next = NULL;
  free(t);

  printf("%d deleted from end successfully.\n", n);
}
