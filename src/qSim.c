#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

float mean;
float SD;
int totalCustomerServed = 0;
float maxTime = 0.0;
float totalServingTime = 0.0;
float maxWaitTime = 0.0;
float totalIdleTime= 0.0;


struct customer 
{
	int id;
	float arrivalTime;
	float leavingTime;
	float serviceTime;
	float reachedHead;
	struct customer * next; 
}typedef customer;
struct teller
{
	int size;
	int id;
	bool isShortest;
	float idleTime;
}typedef teller;
struct event
{
	float  time;
	int flag;
	int name;
	struct event* next;
}typedef event;
event *createEvent(float clock, float t , int k,int i)
{
	event *tempeve = (event*)malloc(sizeof (event));
	if (k == 1) // arrival of customer
	{
		tempeve -> time = t;
		tempeve -> flag = 1;
		tempeve -> name = i;	
	}
	else if (k == 2) // teller searching for next customer
	{
		tempeve -> flag = 2;
		tempeve -> time = clock + t;
		tempeve -> name = i;

	}

	else if(k == 3)
	{
		tempeve -> flag = 3;
		tempeve -> time = clock + t;
		tempeve -> name = i;       // t1 is service time + time 

	}
	/*
	else if(k == 4)
	{
		tempeve -> flag = 4;
		tempeve -> time = clock;   // t1 is service time + time
		tempeve -> t.name = i;
		tempeve -> t. active = 1; 
		tempeve  -> t. IdealTime = t; //idle time 
	}*/
	return tempeve;
}
event *insertEve(event **head_ref, event* new_node){ 
    event* current; 
    /* Special case for the head end */
    if (*head_ref == NULL || (*head_ref)->time  >= new_node->time) { 
        new_node->next = *head_ref; 
        *head_ref = new_node; 
    } 
    else { 
        current = *head_ref; 
        while (current->next != NULL && current->next->time < new_node->time) { 
            current = current->next; 
        } 
        new_node->next = current->next; 
        current->next = new_node; 
    } 
}


void printList(event* eventQueue){
	event* p = eventQueue;
	if(p == NULL){
		printf("no events in eventQueue\n");
		return;
	}
	while(p != NULL){
		printf("event type %d of name %d with time %f\n",p-> flag,p -> name, p->time );
		p = p-> next;
	}
	return;
}

int findShortest(teller* tellerArray , int numOfTellers){
	int shortest = tellerArray[0].size;
	int i;
	for ( i = 1; i < numOfTellers; ++i)
	{
		if (tellerArray[i].size < shortest){
			shortest = tellerArray[i].size;
		}
	}
	return shortest;

}
customer* insertInTellerQueue(customer* head, customer* newNode ){
    int i = newNode -> id;
    newNode -> next = NULL;
    customer* p =  head;
   if(head==NULL)
   return newNode;
    while(p -> next!= NULL){
        p = p -> next;
    }
    p -> next = newNode;
    newNode->next=NULL;
    return head;

}
void printTellerQueues(customer *head){
    customer* p =head;
    while(p!=NULL)
    {
        printf("%d\t",p->id);
        p=p->next;
    }
    printf("\n");
}
float meanOfArray(float* a, int n){
    float k = 0.0;
    int i;
    for(i = 0; i < n ; i ++){
        k = k + a[i];
    }
    return k/n;
}
float standardDeviation(float* a, int n){
    float sd = 0.0;
    int i;
    float mean = meanOfArray(a,n);
    for(i = 0; i < n ; i ++){
        sd+= ( a[i] - mean) * (a[i] - mean);
    }
    return sqrt(sd/n);
}

void displayData(customer* customerArray , int numOfCustomers, float totalIdleTime, float simulationTime, int numOfTellers, int type){
	totalCustomerServed = 0;
	maxTime = 0.0;
	totalServingTime = 0.0;
	maxWaitTime = 0.0;
	int i;
	
	for (i = 0; i < numOfCustomers; ++i)
	{
	    //printf("reachedHead for %d: %f,  %f, %f \n",i,customerArray[i].reachedHead,customerArray[i].arrivalTime,customerArray[i].leavingTime);

		if(customerArray[i].leavingTime > maxTime)
			maxTime = customerArray[i].leavingTime;
		if(customerArray[i].leavingTime < simulationTime){
			totalCustomerServed++;
			totalServingTime = totalServingTime + customerArray[i].serviceTime;
			if(maxWaitTime < customerArray[i].reachedHead - customerArray[i].arrivalTime)
				maxWaitTime = customerArray[i].reachedHead - customerArray[i].arrivalTime;
		}
	}
	float timeSpent[totalCustomerServed];
	int j=0;
	for(i = 0; i< numOfCustomers; ++i){
	    if(customerArray[i].leavingTime < simulationTime){
	        timeSpent[j] = customerArray[i].leavingTime - customerArray[i].arrivalTime;
	        j++;
	    }
   
	}
	mean = meanOfArray(timeSpent,totalCustomerServed);
	SD = standardDeviation(timeSpent,totalCustomerServed);

	
}
void multipleTellerSimulation(customer *customerArray , teller *tellerArray, int numOfCustomers, int numOfTellers, int simulationTime ){
	customer* tellerQueue[numOfTellers];
	totalIdleTime= 0.0;
	float clock = 0.0;
	int i;
	for ( i = 0; i < numOfTellers; ++i)
	    tellerQueue[i] = NULL;

	    
	    
	event* eventQueue = NULL;
	event* tempeve;
	for (i = 0; i < numOfCustomers; ++i)
	{
		tempeve = createEvent(clock, customerArray[i].arrivalTime,1,i );
		insertEve(&eventQueue,tempeve);
	}
//	eventQueue = eventQueue->next;
	for (i = 0; i < numOfTellers; ++i)
	{
		totalIdleTime = totalIdleTime + tellerArray[i].idleTime;
		tempeve = createEvent(clock, tellerArray[i].idleTime,2, i);
		insertEve(&eventQueue,tempeve);
	}
//	printf("clock: %f\n", clock );
//	printList(eventQueue);
	int shortest,count,randomSelect,selected;
	count = 0;
	int name,noOf3;
	noOf3 = 0;
	
	while(true){
	    if(eventQueue == NULL){
	        break;
	    }
	    //		    printf(" \n\n\n\n\n clock: %f\n", clock );
		  //  printList(eventQueue);

		randomSelect = 0;


		switch (eventQueue->flag){

			case 1:
				clock = eventQueue -> time;
				count = 0;
				//printf("clock: %f\n", clock);
				shortest = findShortest(tellerArray, numOfTellers);
				for ( i = 0; i < numOfTellers; ++i)
					if (tellerArray[i].size == shortest)
						count++;
				//printf("count is %d\n",count);
				randomSelect = (rand() %  count) + 1;
				count = 0;
				for ( i = 0; i < numOfTellers; ++i)
				{
					if (tellerArray[i].size == shortest)
					{
						count++;
						if(count == randomSelect)
						{
							selected = i;
							break;
						}
					}

				}

				

				customer* queue = tellerQueue[selected];
				
				//printf("%d is the shortest\n",selected);
				name = eventQueue -> name;
				customer* temp = &customerArray[name];
				int name2 = temp -> id;
				if(tellerQueue[selected] == NULL)
					i =0;
				else
					i = 1;
				tellerQueue[selected ] = insertInTellerQueue(tellerQueue[selected], temp);
				if(i == 0){
				    //printf("\n\n\n");
				   // c(tellerQueue[selected ]);
					customerArray[tellerQueue[selected] -> id].reachedHead = clock;
				}
				eventQueue = eventQueue -> next;
				tellerArray[selected].size++;
				for(i = 0; i < numOfTellers; i++){
				  //  printTellerQueues(tellerQueue[i]);
				}
				break;
				
			case 2:
			    clock = eventQueue -> time;
			    count = 0;
			     name = eventQueue -> name;
			    if(tellerArray[name].size == 0){
			      //  printf("teller queue size is zero\n");
			       for (i = 0; i<numOfTellers ; i++) {
			           if(tellerArray[i].size != 0)
			            count++;
			       }
			       if(count != 0){
			        //   printf("there are non zero queues\n");
			           randomSelect = (rand() %  count) + 1;
			           count = 0;
			           for (i = 0; i<numOfTellers ; i++){
			               if(tellerArray[i].size != 0){
			                   count++;
			                   if(count == randomSelect){
			                       selected = i;
			                       break;
			                   }
			               }
			           }

			           tellerQueue[name] = tellerQueue[selected];
			           tellerQueue[selected] = tellerQueue[selected] -> next;
			           customerArray[tellerQueue[name] -> id].reachedHead = clock;
			           tellerQueue[name] -> next = NULL;
			           //tellerQueue[selected] = tellerQueue[selected] -> next;
			           tellerArray[name].size++;
			           tellerArray[selected].size--;
			       }
			       
			       else{
			         //  printf("all queus of length zero\n");
			           float idle =  2.5 * rand()/(float)RAND_MAX;
			           totalIdleTime = totalIdleTime + idle;
			           tempeve = createEvent(clock, idle,2, name);
			           insertEve(&eventQueue,tempeve);
			           eventQueue = eventQueue -> next;
			           count = -1;
			           break;
			           
			       }
			        
			    }
//			    printf(" %d \n",count);

			    customer* tempCustomer = tellerQueue[name];
			    tellerArray[name].size--;
			    tellerQueue[name] = tellerQueue[name] -> next;
			    if(tellerQueue[name] != NULL){
			    	customerArray[tellerQueue[name] -> id].reachedHead = clock;
			    }
			    tempeve = createEvent(clock, tempCustomer->serviceTime,2, name);
			    insertEve(&eventQueue,tempeve);
			    tempeve = createEvent(clock, tempCustomer ->serviceTime, 3 ,tempCustomer->id);
			    insertEve(&eventQueue,tempeve);
			    
			    eventQueue = eventQueue -> next;
			    break;
			case 3:
				noOf3++;
			    clock = eventQueue -> time;
			    float finalTime = clock;
			    name = eventQueue -> name;
			   // printf("%d\n",name );
			    customerArray[name].leavingTime = finalTime;
			    eventQueue = eventQueue -> next;
			    break;
			default:
			eventQueue = eventQueue -> next;
				break;
		}
		count = 0;
		for(i = 0 ; i < numOfCustomers; i ++){
		    if(customerArray[i].leavingTime < 0)
		        count++;
		}
		if(count == 0)
		    break;
		
	}
	for(i = 0 ; i < numOfCustomers; i ++){
//	    printf("name: %d ,leavingTime: %f \n", i, customerArray[i].leavingTime);
	}
//	printf("number of three: %d  \n",noOf3);
	displayData(customerArray,numOfCustomers,totalIdleTime,simulationTime,numOfTellers,1);



}

void singleQueueTellerSimulation (customer *customerArray , teller *tellerArray, int numOfCustomers, int numOfTellers, int simulationTime ){
	customer* tellerQueue = NULL;
	totalIdleTime= 0.0;
	float clock = 0.0;
	int i;
	    
	    
	event* eventQueue = NULL;
	event* tempeve;
	for (i = 0; i < numOfCustomers; ++i)
	{
		tempeve = createEvent(clock, customerArray[i].arrivalTime,1,i );
		insertEve(&eventQueue,tempeve);
	}
//	eventQueue = eventQueue->next;
	for (i = 0; i < numOfTellers; ++i)
	{
		totalIdleTime = totalIdleTime + tellerArray[i].idleTime;
		tempeve = createEvent(clock, tellerArray[i].idleTime,2, i);
		insertEve(&eventQueue,tempeve);
	}
//	printf("clock: %f\n", clock );
//	printList(eventQueue);
	int shortest,count,randomSelect,selected;
	count = 0;
	int name,noOf3;
	noOf3 = 0;

	while(true){

	    //		    printf(" \n\n\n\n\n clock: %f\n", clock );
		  //  printList(eventQueue);

		randomSelect = 0;
		//printf("tellerQueue - ");
		//printTellerQueues(tellerQueue);

		switch (eventQueue->flag){

			case 1:
				clock = eventQueue -> time;
				tellerQueue = insertInTellerQueue(tellerQueue,& customerArray[ eventQueue->name ]);
				eventQueue = eventQueue -> next;
				break;
				
			case 2:
			    clock = eventQueue -> time;
			    count = 0;
			    name = eventQueue -> name;
			    
			    if(tellerQueue == NULL){
			      //  printf("all queus of length zero\n");
			        float idle =  2.5 * rand()/(float)RAND_MAX;
			        totalIdleTime = totalIdleTime + idle;
			        tempeve = createEvent(clock, idle,2, name);
			        insertEve(&eventQueue,tempeve);
			        eventQueue = eventQueue -> next;
			        count = -1;
			        break;
			        
			    }
			    
			    
			    customer* tempCustomer = tellerQueue;
			    tellerQueue = tellerQueue -> next;
			    if(tellerQueue != NULL){
			    	customerArray[tellerQueue -> id].reachedHead = clock;
			    }
			    tempeve = createEvent(clock, tempCustomer->serviceTime,2, name);
			    insertEve(&eventQueue,tempeve);
			    tempeve = createEvent(clock, tempCustomer ->serviceTime, 3 ,tempCustomer->id);
			    insertEve(&eventQueue,tempeve);
			    
			    eventQueue = eventQueue -> next;
			    break;
			case 3:
				noOf3++;
			    clock = eventQueue -> time;
			    float finalTime = clock;
			    name = eventQueue -> name;
			   // printf("%d\n",name );
			    customerArray[name].leavingTime = finalTime;
			    eventQueue = eventQueue -> next;
			    break;
			default:
			eventQueue = eventQueue -> next;
				break;
		}
		count = 0;
		for(i = 0 ; i < numOfCustomers; i ++){
		    if(customerArray[i].leavingTime < 0)
		        count++;
		}
		if(count == 0)
		    break;
		
	}
	for(i = 0 ; i < numOfCustomers; i ++){
//	    printf("name: %d ,leavingTime: %f \n", i, customerArray[i].leavingTime);
	}
//	printf("number of three: %d  \n",noOf3);
	displayData(customerArray,numOfCustomers,totalIdleTime,simulationTime,numOfTellers,2);



}


int main(int argc, char const *argv[])
{
	FILE *outputfp;
    outputfp = fopen ("output/datas.txt", "w");
	int numOfCustomers, numOfTellers,i ;
	float arrTime,serviceTime,clock,simulationTime ,averageServiceTime;
	numOfCustomers = atoi(argv[1]);
	numOfTellers = atoi(argv[2]);
	simulationTime = atof(argv[3]);
	averageServiceTime = atof(argv[4]);
	//scanf("%d%d%f%f",&numOfCustomers,&numOfTellers,&simulationTime,&averageServiceTime);
	customer customerArray[numOfCustomers];
	teller tellerArray[numOfTellers];
	for ( i = 0; i < numOfCustomers; ++i)// genrating initial event list
	{
		customerArray[i].id = i;
		customerArray[i].reachedHead = - 1.0;
		customerArray[i].arrivalTime =  simulationTime * rand()/(float)RAND_MAX;
        customerArray[i].leavingTime = -1.0;
		customerArray[i].serviceTime = 2*averageServiceTime * rand()/(float)RAND_MAX;
	}
//	printf("for loop runed for %d times \n",i );
	for ( i = 0; i < numOfTellers; ++i)// genrating initial event list
	{
		tellerArray[i].id = i;
		tellerArray[i].size = 0;
		tellerArray[i].idleTime = 10.0 * rand()/(float)RAND_MAX;
		//printf("%f\n",tellerArray[i].idleTime );
		tellerArray[i].isShortest = true;
	}


	multipleTellerSimulation(customerArray,tellerArray,numOfCustomers,numOfTellers,simulationTime);

	printf("total number of customer served by the end of SimTime  : %d\n", totalCustomerServed);
	printf("total number of customer served                        : %d\n", numOfCustomers);
	printf("total time required to serve all customers             : %f\n", maxTime);
	printf("total number of tellers                                : %d\n", numOfTellers);
	printf("type of queue                                          : each teller has an queue\n");
	printf("Average amount of time a customer spent in the bank    : %f\n", mean);
	printf("Standard Deviation of time a customer spent in the bank: %f\n", SD);
	printf("maximum wait time                                      : %f\n", maxWaitTime);
	printf("Total amount of teller service time                    : %f\n", totalServingTime);
	printf("total amount of teller idle time                       : %f\n", totalIdleTime);
	printf("\n\n\n");
	
		for ( i = 0; i < numOfCustomers; ++i)// genrating initial event list
	{

		customerArray[i].reachedHead = - 1.0;
        customerArray[i].leavingTime = -1.0;
	}
//	printf("for loop runed for %d times \n",i );
	for ( i = 0; i < numOfTellers; ++i)// genrating initial event list
	{
		tellerArray[i].size = 0;
		//printf("%f\n",tellerArray[i].idleTime );
		tellerArray[i].isShortest = true;
	}
	
	singleQueueTellerSimulation(customerArray,tellerArray,numOfCustomers,numOfTellers,simulationTime);
	printf("total number of customer served by the end of SimTime  : %d\n", totalCustomerServed);
	printf("total number of customer served                        : %d\n", numOfCustomers);
	printf("total time required to serve all customers 	       : %f\n", maxTime);
	printf("total number of tellers                                : %d\n", numOfTellers);
	printf("type of queue                                          : common queue for all teller\n");
	printf("Average amount of time a customer spent in the bank    : %f\n", mean);
	printf("Standard Deviation of time a customer spent in the bank: %f\n", SD);
	printf("maximum wait time                                      : %f\n", maxWaitTime);
	printf("Total amount of teller service time                    : %f\n", totalServingTime);
	printf("total amount of teller idle time                       : %f\n", totalIdleTime);
	//printf("\n");

		for ( i = 0; i < numOfCustomers; ++i)// genrating initial event list
	{

		customerArray[i].reachedHead = - 1.0;
        customerArray[i].leavingTime = -1.0;
	}

	int tempc=1000;
	int tempt=100;
	int st=60;
	float ast=2.3;
	teller ta[tempt];
	customer ca[tempc];
	for ( i = 0; i < numOfCustomers; ++i)// genrating initial event list
	{
		ca[i].id = i;
		ca[i].reachedHead = - 1.0;
		ca[i].arrivalTime =  st * rand()/(float)RAND_MAX;
        ca[i].leavingTime = -1.0;
		ca[i].serviceTime = 2*ast * rand()/(float)RAND_MAX;
	}
	for(int i=1;i<=tempt;i++){
		
		for(int j=0;j<i;j++){
			ta[i].id = j;
			ta[j].size = 0;
			ta[j].idleTime = 10.0 * rand()/(float)RAND_MAX;
			//printf("%f\n",tellerArray[i].idleTime );
			ta[j].isShortest = true;
		}
		singleQueueTellerSimulation(ca,ta,numOfCustomers,i,simulationTime);
		fprintf(outputfp, "%d ",i);
		fprintf(outputfp,"%f\n",mean);
		for (int k = 0; k < tempc; ++k)// genrating initial event list
		{

		ca[k].reachedHead = - 1.0;
        ca[k].leavingTime = -1.0;
		}


	}

	return 0;
}
