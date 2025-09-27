////////////////////////////////////////////////////////////////////////////
//  FileName:     TCB.ino
//  Author:       Ethan Terrill
//  Date:         27.07.2025
//
//  Description: Implementation of a simple TCB scheduler
//////////////////////////////////////////////////////////////////////



// ============================ function Implementation ============================ //
void scheduler_delay(int delay) {
  task_list[curr]->state = SLEEPING;
  task_list[curr]->start_time = *((volatile uint32_t *)TIMG_T0LO_REG(0));
  task_list[curr]->delay = delay;
}
void scheduler_kill() {
  task_list[curr]->state = DISABLED;
}
void executeTask(void (*func_ptr)()) {
  (*func_ptr)();
}
