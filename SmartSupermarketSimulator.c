//irfan kaan algan 2328599
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


/*The code was showing up to the report section. 
But although I changed nothing, the "initialiseSimulator" section stopped working the next time I ran it.
 I couldn't fix that part because I didn't have time.*/




int CustomerNumber,CashierNumber,ArrivalTime,ServiceTime;

struct CustomerList{
	char customerType;
	int customerAge;
	int arrivalTime;
	int serviceTime;
	int serviceStartTime;
	int cashierID;
	int paymentType;
	struct CustomerList * next;
};
typedef struct CustomerList *List;

struct CustomerQueue{
	struct CustomerList *front;
	struct CustomerList *rear;
	int size;
};
typedef struct CustomerQueue *Queue;

int parseInput(int argc,char * argv[]){
	if(argc >= 5){
		CustomerNumber = atoi(argv[1]);
		CashierNumber = atoi(argv[2]);
		ArrivalTime = atoi(argv[3]);
		ServiceTime = atoi(argv[4]);
		return 1;
	}
	else
		return 0;
}

Queue initialiseSimulator(int NumberofCustomer,int CashierNum,int *cashier){

	struct CustomerQueue *q;


	if(NumberofCustomer < 0)
	{
		printf("Error!");
		return ;
	}
	
	q = malloc(sizeof(struct CustomerQueue));
	q->front = malloc(sizeof(struct CustomerQueue));
	q->front->next = malloc(sizeof(struct CustomerQueue));;
	q->front->next->next = NULL;
	q->rear = q->front;

	if(q == NULL){
		printf("not enough memory space!");
		return ;
	}
	cashier = (int*)malloc(CashierNum * sizeof(int));
	if(cashier == NULL)
		printf("error!");
 
	return q;

}

Queue newCustomer(Queue q, List head){
	struct CustomerList *t;
	t = malloc(sizeof(struct CustomerList));
	t->next = NULL;
	t = head;
    
	if(q->size == 0){
    	q->front->next = t;
    	q->rear = q->front;
    	q->size ++;
    	t = t->next;
	}    

    
	if(q->size == 1 && t != NULL){
		if(t->customerType > (q->front->next->customerType)){
			t->next=q->front->next;
			q->front->next = t;
			q->size ++;
			t = t->next;
		}
		else if(t->customerType == (q->front->next->customerType)){
			if(t->arrivalTime < (q->front->next->arrivalTime)){
				t->next=q->front->next;
				q->rear = q->front->next;				
				q->front->next = t;
				q->size ++;
				t = t->next;
			}	
		}
	}

	if(q->size > 1 && t != NULL){
		
		struct CustomerList *temp1, *temp2;
		temp1 = malloc(sizeof(struct CustomerList));
		temp1->next = NULL;
		temp2 = malloc(sizeof(struct CustomerList));
		temp2->next = NULL;
		struct CustomerList *queueTemp, *listTemp;
		queueTemp = malloc(sizeof(struct CustomerList));
		queueTemp->next = NULL;
		
		listTemp=malloc(sizeof(struct CustomerList));
		listTemp->next = NULL;
		temp1 = q->front->next;
		queueTemp = temp1;
		listTemp = t;
		temp2 = temp1->next;
		
		while(t->next != NULL){
			
			temp1 = queueTemp;
			temp2 = temp1->next;
			t = listTemp;
			while(t->customerType< (temp2->customerType) && t->customerType < temp1->customerType){
				temp1=temp1->next;
				temp2 = temp2->next;
			}
			
			t->next = temp2;
			temp1->next = t;
			q->size ++;				
			listTemp = listTemp->next;			
		}
	
	}
	return q;
	}
	



List createCustomerList(int CustomerNumber, int ArrivalTime, int ServiceTime){

	srand(time(NULL)+100);
	
	int cash=0,credit=0;
	int i;
	struct CustomerList *temp;
	temp= malloc(sizeof(struct CustomerList));
	temp->next = NULL;

	
	struct CustomerList *head;
	head = malloc(sizeof(struct CustomerList));
	head->next = NULL;
	head->serviceStartTime = 0;

	
	temp = head;
	for(i = 0 ; i <= CustomerNumber ; i++){
		temp->cashierID = 0;
		temp->arrivalTime= rand()%ArrivalTime+1;
		temp->serviceTime= rand()%ServiceTime+1;
		temp->serviceStartTime = 0;
		temp->customerAge = rand()%90+1;
		temp->paymentType = rand()%2+1;
		temp->next = malloc(sizeof(struct CustomerList));
		temp = temp->next;
		temp->next = NULL;
	}
	temp = head;
	printf("Type\t\tCustomer Age\tService Time\tService start time\tCashier ID\tArrival Time\tPayment Type\n");
	for(; temp->next != NULL ; )
	{
		if(temp->customerAge <=40)
			printf("Young Age\t%d\t\t",temp->customerAge);
		else if(temp->customerAge  <= 60)
			printf("Adult \t\t%d\t\t",temp->customerAge);
		else
			printf("Elderly\t\t%d\t\t",temp->customerAge);

		printf(" %d\t\t %d\t\t\t %d\t\t %d\t\t",temp->arrivalTime,temp->serviceTime,temp->serviceStartTime,temp->cashierID);
			
			
  		if(temp->paymentType == 1){
		  	cash++;
			printf("Cash\n");}
		else{
			credit++;
			printf("CreditCard \n");
		}
			
		temp = temp->next;
		}
		printf("****************Report***************** \n");
		if(cash>credit)
			printf("\nMost popular payment type: Cash\n");
		else
			printf("\nMost popular payment type: Credit Card\n");
	return head;

}


void ServeCustomer(Queue q, int cashier[],int CashierNumber){

	int static clock=0;	
	struct CustomerQueue *tq;
	tq = malloc(sizeof(struct CustomerQueue));
	tq->front = NULL;
	tq->rear = NULL;
	tq = q;
	int i = 0 ,cashierControl = 0, j = 0;

	for(i = 0 ; i<tq->size ; i++){
		if(cashierControl < CashierNumber){
			clock = tq->front->next->arrivalTime;
			clock = tq->front->next->serviceTime;
			printf("Maximum waiting time: %d\nServe time is %d",clock,(tq->front->next->arrivalTime-tq->front->next->serviceTime));
			cashierControl ++;
			tq->front->next = tq->front->next->next;
			i ++;
		}
		else{
			cashierControl = 0;
			i --;
			j = 0;
			while(j < CashierNumber)
				cashier[j] = 0;
		}
	}
}

	
	
int reportStatistics(int CashierNumber, int CustomerNumber, List l, Queue q){
	int i = 0 ;
	int completion =0;
	struct CustomerList *t;
	t = malloc(sizeof(struct CustomerList));
	t->next=NULL;
	
	t = l;
	
	printf("\n\n\n");
	printf("The number of cashiers: %d\nThe number of customers: %d\n",CashierNumber,CustomerNumber);
	completion = (q->front->next->arrivalTime)-(q->rear->serviceTime + q->rear->arrivalTime);
	printf("Completion time: %d\n",completion);
	int age_Y=0,age_A=0,age_E=0;
	for(; t->next != NULL;){
		if(t->customerAge <=40){
			age_Y++;
		}
		else if(t->customerAge <=60)
			age_A++;
		else
			age_E++;
	}
	printf("Number of customers for each age group: \n");
	printf("Elderly: %d\n Adult: %d\n Young Age: %d\n",age_E,age_A,age_Y);
	
	printf("Number of customers for each cashier: \n");
}



int main(int argc, char *argv[]) {
	int control=0;
	
	
	struct CustomerList *head;
	head = malloc(sizeof(struct CustomerList));
	head->next = NULL;
	if(head == NULL)
		printf("Out of memory space!!");
		
	struct CustomerQueue *q;
	q = malloc(sizeof(struct CustomerQueue));
	if(q == NULL)
		printf("Out of memory!!");
	else{
		q->front = NULL;
		q->rear = NULL;
	}
	control=parseInput(argc,argv);
	
	if(control==0){
		printf("Enter Number Of Customers:");scanf("%d",&CustomerNumber);
		printf("Enter the number of Cashiers:");scanf("%d",&CashierNumber);
		printf("Enter the max Arrival Time:");scanf("%d",&ArrivalTime);
		printf("Enter the number max Service Time:");scanf("%d",&ServiceTime);
	}
	int *cashier;
	head = createCustomerList(CustomerNumber,ArrivalTime,ServiceTime);

	q = initialiseSimulator(CustomerNumber,CashierNumber,cashier);
	q = newCustomer(q,head);
	ServeCustomer(q,cashier,CashierNumber);
	reportStatistics(CashierNumber,CustomerNumber,head,q);



	return 0;
}



