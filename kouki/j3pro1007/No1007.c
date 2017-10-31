/* チェインハッシュ法 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TABLE_SIZE 100
#define DEBUG 0

typedef struct cell{
  int *data;
  struct cell *next;
}CELL;

/* プロトタイプ宣言 */
void hash_init(CELL **table);
CELL* makecell();
void cell_free(CELL *cell);
void hash_free(CELL **table);
int get_hash(int data);
void insert(CELL **table,int data);
CELL* search(CELL **table,int data);
void delete(CELL **table,int data);

int main(void)
{
  CELL *table[TABLE_SIZE];
  int selector,input_num;
  hash_init(table);

  while(1){
    printf("Select Action\n1.insert 2.delete 3.search 4.end\n");
    scanf("%d",&selector);

    switch(selector){
    case 1:
      printf("Please enter a numeric value.\n");
      scanf("%d",&input_num);
      insert(table,input_num);
      break;
    case 2:
      printf("Please enter a numeric value.\n");
      scanf("%d",&input_num);
      delete(table,input_num);
      break;
    case 3:
      printf("Please enter a numeric value.\n");
      scanf("%d",&input_num);
      search(table,input_num);
      break;
    case 4:
        hash_free(table);
        exit(EXIT_SUCCESS);
    default:
      break;
    }
  }

  hash_free(table);
  exit(EXIT_FAILURE);
}

void hash_init(CELL **table)
{
  int i;
  for(i = 0;i < TABLE_SIZE;i++){
    table[i] = NULL;
  }
  return;
}

/* CELL型ポインタ(next=NULL,data=malloc済み)を返す */
CELL* makecell()
{
  CELL* cell = NULL;
  cell = (CELL*)malloc(sizeof(CELL));
  cell->next = NULL;
  cell->data = (int *)malloc(sizeof(char));
  return cell;
}

void cell_free(CELL *cell)
{
  if(cell->data != NULL)free(cell->data);
  free(cell);
  return;
}

void hash_free(CELL **table)
{
  int i;
  CELL *tmp=NULL,*swap=NULL;

  for(i = 0;i < TABLE_SIZE;i++){
    tmp = table[i];
    while(tmp != NULL){
      swap = tmp->next;
      cell_free(tmp);
      tmp = swap;
    }
  }
  hash_init(table);

  printf("[SUCCESS]: hash_free/1 completed successflly.\n");
  return;
}

int get_hash(int data)
{
  srand((unsigned)data);
  return (rand()%TABLE_SIZE);
}

void insert(CELL **table,int data)
{
  CELL *cell = makecell();
  if(cell == NULL){
    printf("%x1b[31m");
    printf("[:error, chainhash.c:69 unexpected:malloc didn't scure memory.]\n");
  }
  CELL *target = NULL;
  *(cell->data) = data;
  int hash = get_hash(data);
  target = table[hash];
  if(target == NULL){
      table[hash] = cell;
  }else{
    while(target->next != NULL)target = target->next;
    target->next = cell;
  }
  #ifdef DEBUG
    printf(":ok\n");
  #endif
  printf("> [:ok, %x]\n",(unsigned int)cell);
}

CELL* search(CELL **table,int data)
{
  int hash = get_hash(data);
  CELL *tmp = NULL;
  tmp = table[hash];

  while(tmp != NULL){
    if(*(tmp->data) == data)break;
    tmp = tmp->next;
  }

  if(tmp == NULL){
    printf("[:ok, this value is not registered.]\n");
  }else{
    printf("> [:ok, %x]\n",(unsigned int)tmp);
  }

  return tmp;
}

void delete(CELL **table,int data)
{
  int hash = get_hash(data);
  CELL *target = table[hash];
  CELL *tmp;

  if(target == NULL)return;
  if(*(target->data) == data){
    table[hash] = table[hash]->next;
    cell_free(target);
    printf(":ok\n");
    return;
  }else{
    while((target->next != NULL) && (*(target->next->data) != data)){
      target = target->next;
    }
    tmp = target->next;
    target->next = tmp->next;
    cell_free(tmp);
    printf(":ok\n");
  }
}
