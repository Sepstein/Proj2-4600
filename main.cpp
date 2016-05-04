/************************************************************************
*Group Members: Jamie Simpson, Juan Apolinar, Sam Epstein
*Assignment: Project 2
*Course: CSCE 4600
*Professor: David Keathly

Description: Main function of a memory allocation program that is the basis
of project 2. Adds all procesees that will be allocated in memory and will 
call the memory allocation program.
*************************************************************************/

#include "process_memory.h"

int main(void){
	clock_t start_t=0,end_t=0;
	srand(time(NULL));
	Process_memory process_memory;//declares an instance of process memory which holds all relevant functions and processes for the simulation
	for(int Process=0;Process<NUMBER_OF_PROCESSES;Process++){
		process_memory.add_process(Process);//Adds the specified number of processes that is required by this program
	}
	std::cout<<"20 MB results:"<<std::endl;
	std::cout<<"**************************************"<<std::endl;
	std::cout<<"Default:"<<std::endl;
	process_memory.default_memory_simulation(TWENTY_MB);
	std::cout<<"Custom:"<<std::endl;
	process_memory.custom_memory_simulation(TWENTY_MB);//The second part of the project requires us to use a memory block of size 20 MB
	int max_memory_required=process_memory.largest_memory_required();
	int memory_size=process_memory.return_memory()*.5;
	if(max_memory_required>memory_size)//used to guarantee that there will be enough memory space for each process.
		memory_size=max_memory_required;
	std::cout<<"**************************************"<<std::endl<<std::endl;
	std::cout<<memory_size<<" KB results:"<<std::endl;
	std::cout<<"**************************************"<<std::endl;
	std::cout<<"Default:"<<std::endl;
	process_memory.default_memory_simulation(memory_size);//Third part of assignment, half the size of needed memory
	std::cout<<std::endl;
	std::cout<<"Custom:"<<std::endl;
	process_memory.custom_memory_simulation(memory_size);
	std::cout<<"**************************************"<<std::endl<<std::endl;
	memory_size=process_memory.return_memory()*.1;
	if(max_memory_required>memory_size)
		memory_size=max_memory_required;
	std::cout<<memory_size<<" KB results:"<<std::endl;
	std::cout<<"**************************************"<<std::endl;
	std::cout<<"Default:"<<std::endl;
	process_memory.default_memory_simulation(memory_size);//Third part of assignemnt, one tenth size of needed memory
	std::cout<<std::endl<<"Custom:"<<std::endl;
	process_memory.custom_memory_simulation(memory_size);
	std::cout<<"**************************************"<<std::endl<<std::endl;
	return 0;
}
