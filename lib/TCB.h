////////////////////////////////////////////////////////////////////////////
//  FileName:     TCB.h
//  Author:       Ethan Terrill
//  Date:         27.07.2025
//
//  Description: Header descring the necassary operations of a 
//                TCB scheduler
//////////////////////////////////////////////////////////////////////





// ============================ Macros ============================ //


/////////////////////////////////////
//  task states
//////////////////////////////////
#define ACTIVE_STATE  0
#define SLEEPING      1
#define DISABLED      2


#define SECOND 1000000


#define MAX_TASK_NUM 10



// ============================ Structs ============================ //
typedef struct task{
  int pid;
  void (*func)();
  int delay;
  uint32_t start_time;
  int state;
  int priority;
}task;




// ============================ ProtoTypes ============================ //


//////////////////////////////////////////////
/// @brief
/// Scheduler_delay is repsonsible for adding
///  a delay to the function currently being
///  processed by the scheduler
/// @param delay
///   Specifies the length of the delay in 
///   microseconds
/////////////////////////////////////////////
void scheduler_delay(int delay);



//////////////////////////////////////////////
/// @brief
/// Scheduler_delay marks a given process  
/// as being "dead" or finished
/////////////////////////////////////////////
void scheduler_kill();



//////////////////////////////////////////////
/// @brief
/// execute Task takes a function pointer to 
///  a task and executes that task
///
///  @param void(*func_ptr)()
///    pointer to function of type void
/////////////////////////////////////////////
void executeTask(void (*func_ptr)());





// ============================ Global variables ============================ //
unsigned short int curr = 0;
task* task_list[MAX_TASK_NUM]; 
