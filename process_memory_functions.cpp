/************************************************************************
*Group Members: Jamie Simpson, Juan Apolinar, Sam Epstein
*Assignment: Project 2
*Course: CSCE 4600
*Professor: David Keathly

Description: Indludes all relevant functions for the memory allocation program,
including the custom allocation simulation functions that we designed.
*************************************************************************/

#include "process_memory.h"

/*****************************************************************
*function: add process()

*Paramater:    int process_ID: The ID of the process that is being
			   added to the queue. The number is incremented everytime
			   a process is added.
*Return Value: NULL
*Description:  Adds processes that will be used for the memory allocation 
               simulation.
*******************************************************************/
void Process_memory::add_process(int process_ID){
	int new_number_of_cycles=generate_cycles();
	int new_memory=generate_memory();
	int new_process_ID=process_ID;
	int new_cycles_spent=0;
	Process new_process(new_process_ID,new_memory,new_number_of_cycles,new_cycles_spent);
	Process_list.push_back(new_process);
}

/*****************************************************************
*function: Return Memory()

*Paramater:    Null
*Return Value: int: the size of all the memory in the process queue
*Description:  Returns the size of the total memory that is required
			   by all processes in the queue
*******************************************************************/
int Process_memory::return_memory(){
	return total_memory;//returns total memory of all processes in queue.
}

/*****************************************************************
*function: Generate Cycles()

*Paramater:    Null
*Return Value: int: The number of cycles that is required to execute
               the added process.
*Description:  Generates required cycles for a process that is being
               added.
*******************************************************************/
int Process_memory::generate_cycles(){
	return (rand()%(MAX_CYCLE-MIN_CYCLE+1)+MIN_CYCLE);//generates cycles with a mean of 6000 and range 1000-11000
}

/*****************************************************************
*function: Generate Memory()

*Paramater:    Null
*Return Value: int: The amount of memory that the added process
               will take.
*Description:  Generates memory for a process that is being
               added.
*******************************************************************/
int Process_memory::generate_memory(){
	int bound_selector=rand()%100;//this is used to select if bounds will be below or above the mean of 20.
	int memory;
	float random = (float)rand() / (float)RAND_MAX;//generates a inting mem number
	if(bound_selector<80)//Necessary to set mean of 20.
    	memory = (random * (MEAN_MEM-MIN_MEM))+MIN_MEM;
	else
		memory = (random * (MAX_MEM-MEAN_MEM))+MEAN_MEM;
	total_memory+=memory;//keeps sum of the total memory of all the processes combined.
	return (int)memory;
}

/*****************************************************************
*function: Generate Cycles()

*Paramater:    int: The size of the memory that is required to be used
               as specified by the assignemnt.
*Return Value: NULL
*Description:  A custom memory simulation that is required for parts
               2 and 3 of the assignemnt. It utilizes the my_alloc and
               my_free that is required to be used and simulates memory
               management in the generated memory block.
*******************************************************************/
void Process_memory::custom_memory_simulation(int size_of_memory){
	number_processes_completed=0;
	number_processes_arrived=0;
	total_cycle_time=0;
	memory_space=(int*)malloc(size_of_memory*sizeof(int));//the memory space to be used
	for(int i=0;i<size_of_memory;i++){
		memory_space[i]=0;//sets all memory to 0 to signify free space
	}
	while(number_processes_completed!=NUMBER_OF_PROCESSES){//goes through all processes until all are executed
		if(total_cycle_time%50==0&&number_processes_arrived!=NUMBER_OF_PROCESSES){//adds new process every 50 cycles until no new ones can be added
			if(my_alloc(Process_list[number_processes_arrived],size_of_memory,"new process")!=0)//alocates space for the new process.
				return;
			++number_processes_arrived;
		}
		++Ready_Queue.front().cycles_spent;//uses FIFO structure, so process first in will have its cycles incremented first
		if(Ready_Queue.front().cycles_spent==Ready_Queue.front().number_of_cycles){
			my_free(Ready_Queue.front());//when process has finished incrementing, frees it.
			++number_processes_completed;
			if(!Wait_Queue.empty()){//adds a new process from the wait queue of processes if possible
				my_alloc(Wait_Queue.front(),size_of_memory,"waiting process");
			}
		}
		++total_cycle_time;
	}
	std::cout<<"Done! Time took: "<<total_cycle_time<<" cycles"<<std::endl;
	free(memory_space);
}

/*****************************************************************
*function: my alloc()

*Paramater:    struct Process: The process to be added to memory.
			   int: The size of the total memory block. 
			   std::string: lets function know if process being added
			   comes from ready queue or wait queue.
*Return Value: int, returns 1 if a single process memory exceeds current
			   memory workspace
*Description:  Allocates each process onto the memory lock so that it can
               soon begin to be executed.
*******************************************************************/

int Process_memory::my_alloc(Process Process_to_queue,int size_of_memory,std::string queue_source){
	int free_memory_space=0;
	int free_memory_start=0;
	if(Process_to_queue.memory_footprint>size_of_memory){
		std::cout<<"Critical Error. Processes greater than total memory size. Shutting down."<<std::endl;
		return 1;
	}
	//loop wiil loop through array to find a space beg enough to host the current process memory
	for(int memory_location=0;memory_location<size_of_memory;memory_location++){
		if(memory_space[memory_location]==0){
			if(free_memory_space==0){
				free_memory_start=memory_location;
			}
			++free_memory_space;
		}
		else if(memory_space[memory_location]!=0){
			free_memory_space=0;
		}
		if(free_memory_space==Process_to_queue.memory_footprint){//memory big enough to hold process memory
			Memory_location new_memory_location(Process_to_queue.process_ID,free_memory_start,Process_to_queue.memory_footprint);
			process_memory_location.push_back(new_memory_location);//adds indicator of process location in memory
			for(int j=free_memory_start;j<free_memory_start+free_memory_space;j++){
				memory_space[j]=1;//sets memory space equal to 1 to indicate taken memory
			}
			Ready_Queue.push_back(Process_to_queue);
			if(queue_source=="waiting process"){
				Wait_Queue.erase(Wait_Queue.begin()+0);//erases process from wait queue if added from there.
			}
			break;
		}
		if(memory_location+1==size_of_memory){//no free space big enough to hold memory
			if(queue_source=="new process"){
				memory_miss++;
				Wait_Queue.push_back(Process_to_queue);//adds process to wait queue to be checked later.
			}
			break;
		}
	}
	return 0;
}

/*****************************************************************
*function: my alloc()

*Paramater:    struct Process: The process to be added to memory.
*Return Value: NULL
*Description:  Free memory process from the meory block of processes.
*******************************************************************/

void Process_memory::my_free(Process process_to_free){
	int process_ID_to_free=process_to_free.process_ID;
	for(int i=0;i<process_memory_location.size();i++){
		if(process_memory_location[i].process_ID==process_ID_to_free){
			int j;
			for(j=process_memory_location[i].process_location;j<process_memory_location[i].process_location+process_memory_location[i].process_size;j++){
				memory_space[j]=0;//frees memory space of process location
			}
			process_memory_location.erase(process_memory_location.begin()+i);
			break;
		}
	}
	Ready_Queue.erase(Ready_Queue.begin()+0);//frees Process from ready queue
}

Process_memory::Memory_location::Memory_location(int a_process_ID,int a_process_location,int a_process_size){
	process_ID=a_process_ID;
	process_location=a_process_location;
	process_size=a_process_size;
}

Process_memory::Process_memory(){
	total_memory=0;
}

Process_memory::Process::Process(int a_process_ID,float a_memory_footprint,int a_number_of_cycles,int a_cycles_spent){
	process_ID=a_process_ID;
	number_of_cycles=a_number_of_cycles;
	memory_footprint=a_memory_footprint;
	cycles_spent=a_cycles_spent;
}