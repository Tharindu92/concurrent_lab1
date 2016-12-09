#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define maximun_num 65535

struct list_node **head_p;
struct list_node
{
	int data;
	struct list_node* next;
};

int member(int value, struct list_node* head_p);
int insert(int value, struct list_node** head_pp);
int delete(int value, struct list_node** head_pp);

int main(){
	int n,m,samples,i;
  float mMember,mInsert,mDelete;
  printf("Enter the number of samples ");
  scanf("%d",&samples);
	printf("Enter the initial size of linked list (n) ");
	scanf("%d",&n);
  printf("Enter the number of operations (m) ");
  scanf("%d",&m);
  printf("Fraction of Member operations ");
  scanf("%f",&mMember);
  printf("Fraction of Insert operations ");
  scanf("%f",&mInsert);
  printf("Fraction of Delete operations ");
  scanf("%f",&mDelete);


	srand(time(NULL));
  head_p = malloc(sizeof(struct list_node));
  for(i=0; i< samples;i++){
  	//head_p = malloc(sizeof(struct list_node));
  	//*head_p = malloc(sizeof(struct list_node));
  	/*for(i=0; i < n; i++){
  		num = rand() % 65535;
  		if(i==0){
  			(*head_p)->data = num;
  		}else{
  			insert(num, head_p);
  		}
  	}*/
    populate_linked_list(n,head_p);
    int cnt = getLinkedList(*head_p);
    printf("\nCount = %d\n",cnt);
  }
}

int populate_linked_list(int n, struct list_node** head_p)
{
  int num,i;
  int inserted;
  *head_p = malloc(sizeof(struct list_node));
  for(i=0; i<n; ){
    num = rand() % maximun_num;
    if(i==0){
      (*head_p)->data = num;
      i++;
    }else
      inserted = insert(num, head_p);
      if(inserted == 1){
        i++;
      }
    }
}

int getLinkedList(struct list_node* head_p)
{
  struct list_node* curr_p = head_p;
  int count =0;
  while(curr_p != NULL){
    printf("%d ",curr_p->data);
    curr_p = curr_p->next;
    ++count;
  }
  return count;
}

int member(int value, struct list_node* head_p)
{
	struct list_node* curr_p = head_p;

	while(curr_p != NULL && curr_p->data < value )
		curr_p = curr_p->next;

	if (curr_p == NULL || curr_p->data > value)
		return 0;
	else
		return 1;
}

int insert(int value, struct list_node** head_pp)
{
	struct list_node* curr_p = *head_pp;
	struct list_node* pred_p = NULL;
	struct list_node* temp_p ;

	while(curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if(curr_p ==NULL || curr_p->data > value)
	{
		temp_p = malloc(sizeof(struct list_node));
		temp_p->data = value;
		temp_p->next = curr_p;
		if(pred_p == NULL)
			*head_pp = temp_p;
		else
			pred_p->next = temp_p;
		return 1;
	}else{
		return 0;
	}

}

int delete(int value, struct list_node** head_pp)
{
	struct list_node* curr_p = *head_pp;
	struct list_node* pred_p = NULL;

	while(curr_p != NULL && curr_p->data < value){
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if(curr_p != NULL && curr_p->data == value)
	{
		if(pred_p == NULL)
		{
			*head_pp = curr_p->next;
			free(curr_p);
		}else{
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	}else{
		return 0;
	}
}
// */
