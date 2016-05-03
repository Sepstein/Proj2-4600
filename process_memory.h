/************************************************************************
*Group Members: Jamie Simpson, Juan Apolinar, Sam Epstein
*Assignment: Project 2
*Course: CSCE 4600
*Professor: David Keathly

Description: class that holds all the relevant allocations program as well
as the list of programs that will be allocated to memory
*************************************************************************/

#include "declarations.h"

class Process_memory{
	public:
		Process_memory();
		void add_process(int);//adds process to ueue of processes to be executed
		void default_memory_simulation(int);
		void custom_memory_simulation(int);//memory allocation using my_alloc and my_free
		int return_memory();//returns process memory
		int largest_memory_required();//returns largest process memory requirement
	private:
		//stats for each individual process
		struct Process{
			int process_ID;
			int number_of_cycles;
			float memory_footprint;
			int cycles_spent;
			Process(int,float,int,int);
			Process(void*){};
		};
		std::vector<Process> Process_list;
		std::vector<Process> Ready_Queue;
		std::vector<Process*> Ready_Queue_default;
		std::vector<Process> Wait_Queue; 
		struct Memory_location{
			int process_ID;
			int process_location;
			int process_size;
			Memory_location(int,int,int);
		};
		std::vector<Memory_location> process_memory_location;
		int* memory_space;
		int allocated_memory;
		float cache_miss_rate;
		float cache_hit_rate;
		int total_cycle_time;
		int total_memory;
		int number_processes_completed;
		int number_processes_arrived;
		int generate_cycles();
		int generate_memory();
		void my_free(Process);
		int my_alloc(Process,int,std::string);
};