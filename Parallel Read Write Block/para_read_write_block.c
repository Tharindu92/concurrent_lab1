#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#define maximun_num 65535

struct list_node **head_p;
struct argument_list arguments;
pthread_rwlock_t lock; 
struct list_node
{
  int data;
  struct list_node* next;
};

struct argument_list {
    int m; 
    float mOps[3]; 
    struct list_node** head_p;
    int threads;
};

int member(int value, struct list_node* head_p);
int insert(int value, struct list_node** head_pp);
int delete(int value, struct list_node** head_pp);
void *callOperations(void *arguments);
int insed = 0;
int main(){
  while(1){
    int n,m,samples,i,threads,thread_count;
    float mOperations[3];
    clock_t begin, end;
    pthread_t* thread_handles;
    printf("Enter the number of samples ");
    scanf("%d",&samples);
    printf("Enter the initial size of linked list (n) ");
    scanf("%d",&n);
    printf("Enter the number of operations (m) ");
    scanf("%d",&m);
    printf("Fraction of Member operations ");
    scanf("%f",&mOperations[0]);
    printf("Fraction of Insert operations ");
    scanf("%f",&mOperations[1]);
    printf("Fraction of Delete operations ");
    scanf("%f",&mOperations[2]);
    printf("Enter the number of threads ");
    scanf("%d",&threads);

    double time_list[samples];
    double time_spent, total_time = 0,mean;
    srand(time(NULL));
    
    //arguments = malloc(sizeof(struct argument_list));

    if (pthread_rwlock_init(&lock, NULL) != 0)
    {
      printf("\n rwlock init failed\n");
      return 1;
    }

    for(i=0; i< samples;i++){ 
      head_p = NULL;
      head_p = malloc( sizeof(struct list_node));
      populate_linked_list(n,head_p);
      setArguments(m,mOperations,head_p,threads);
      thread_handles = malloc(threads*sizeof(pthread_t));
      begin = clock();
      for(thread_count=0; thread_count < threads; thread_count++){  
        pthread_create(&thread_handles[thread_count], NULL, callOperations, (void*) &arguments);
      }

      for(thread_count=0; thread_count < threads; thread_count++){
        pthread_join(thread_handles[thread_count], NULL);
      }
      end = clock();
      time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
      time_list[i] = time_spent;
      total_time += time_spent; 
      free(thread_handles);
      free(head_p);
    } 
    //free(arguments);
    mean = total_time / samples;
    calculateSTD(time_list, samples, mean);
    pthread_rwlock_destroy(&lock);
  }
}

int setArguments(int m, float mOps[3], struct list_node** head_p, int threads){
  //*arguments = malloc(sizeof(struct argument_list));
  int count;
  (arguments).m = m;
  (arguments).head_p = head_p;
  (arguments).threads = threads;
  for(count=0; count < 3; count++){
    (arguments).mOps[count] = mOps[count];
  }
  
  return 0;
}

void *callOperations(void *arguments)
{
    struct argument_list *args = arguments;
    int m = args -> m,i; 
    float mOps[3]; 
    struct list_node** head_p = args -> head_p; 
    int threads = args -> threads;
    for(i=0; i<3;i++){
      mOps[i] = args -> mOps[i];
    }
    opearations(m,mOps,head_p,threads);
}

int calculateSTD(double time_list[], int samples, double mean){
  int i;
  float std=0;
  float temp=0.0;
  float min_samples;
  for(i=0; i<samples; i++){
    time_list[i] -= mean;
    temp = time_list[i]*time_list[i];
    std += temp;
  }
  std = std/samples;
  std = sqrt(std);
  min_samples = pow((100*1.96*std)/(5*mean),2);
  printf("Average time spent = %f\n",mean);
  printf("Standard Deviation = %f\n",(std));
  printf("Minimum samples need = %f\n", min_samples);

  return 0;
}


int opearations(int m, float mOps[3], struct list_node** head_p, int threads){
  int ops[3];
  int i,num,success;
  //printf("m = %d\n",m);
  //printf("mOps = %f %f %f\n",mOps[0],mOps[1],mOps[2]);
  for(i=0; i<3; i++){
    /*
      0 - Member
      1 - Insert
      2 - Delete
    */
    ops[i] = m*mOps[i]/threads;
  }
    //printf("Insert %d\n %f",ops[2],mOps[2]);
  while(ops[0]!= 0|| ops[1] != 0 || ops[2] != 0){
    if(ops[0] != 0){
      num = rand() % maximun_num;
      pthread_rwlock_rdlock(&lock);
      member(num, *head_p);
      pthread_rwlock_unlock(&lock);
      ops[0]--;
    }

    if(ops[1] != 0){
      num = rand() % maximun_num;
      pthread_rwlock_wrlock(&lock);
      insert(num, head_p);
      pthread_rwlock_unlock(&lock);
      ops[1]--;
    }

    if(ops[2] != 0){
      num = rand() % maximun_num;
      pthread_rwlock_wrlock(&lock);
      delete(num, head_p);
      pthread_rwlock_unlock(&lock);
      ops[2]--;
    }
  }
}

int populate_linked_list(int n, struct list_node** head_p)
{
  int num,i=0;
  int inserted;
  *head_p = NULL;
  *head_p = malloc( sizeof(struct list_node));
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
  int count =0,i=0;
  while(curr_p != NULL){
    curr_p = curr_p->next;
    count++;
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

/*
  25
  1000
  10000
  0.99
  0.005
  0.005
  4
*/