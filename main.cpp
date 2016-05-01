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
	srand(time(NULL));
	Process_memory process_memory;//declares an instance of process memory which holds all relevant functions and processes for the simulation
	for(int Process=0;Process<NUMBER_OF_PROCESSES;Process++){
		process_memory.add_process(Process);//Adds the specified number of processes that is required by this program
	}
	process_memory.custom_memory_simulation(TWENTY_MB);//The second part of the project requires us to use a memory block of size 20 MB
	process_memory.custom_memory_simulation(process_memory.return_memory()*.5);//Third part of assignment, half the size of needed memory
	process_memory.custom_memory_simulation(process_memory.return_memory()*.1);//Third part of assignemnt, one tenth size of needed memory
	return 0;
}