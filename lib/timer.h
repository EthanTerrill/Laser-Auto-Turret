////////////////////////////////////////////////////////////////////////////
//  FileName:     timer.h
//  Author:       Ethan Terrill
//  Date:         27.07.2025
//
//  Description: Implementation of a handful of useful timing
//                functions
//////////////////////////////////////////////////////////////////////


// ============================ Macros ============================ //
#define TIMER_INCREMENT_MODE (1<<30)
#define TIMER_ENABLE (1<<31)
#define TIMER_DIVIDER_VALUE 80



// ============================ function Implementation  ============================ //
inline void timer_init() {
// Configure timer
  uint32_t timer_config = (TIMER_DIVIDER_VALUE << 13);

  // Optionally apply a clock divider
  timer_config |= TIMER_INCREMENT_MODE;
  timer_config |= TIMER_ENABLE;

  // Write config to timer register
  *((volatile uint32_t *)TIMG_T0CONFIG_REG(0)) |= timer_config;

  // Trigger a timer update to load settings
  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) |= 1;

}

inline void timer_refresh() {

  *((volatile uint32_t *)TIMG_T0UPDATE_REG(0)) |= 1;
}

inline uint32_t get_time() {
  return *((volatile uint32_t *)TIMG_T0LO_REG(0));
}
