/******************************************************************************
 *
 * General PID implementation for HCS12X.
 *
 * Discrete PID controller implementation. Set up by giving P/I/D terms
 * to Init_PID(), and uses a struct PID_DATA to store internal values.
 *
 * - File:               pid.c
 * - Compiler:           CodeWarrior 4.7 for HCS12X
 * - Supported devices:  MC9S12XDT256CAL can be used.
 *****************************************************************************/

#include "includes.h"

/*! Initialisation of PID controller parameters.
 *
 *  Initialise the variables used by the PID algorithm.
 *
 *   p_factor  Proportional term.
 *   i_factor  Integral term.
 *   d_factor  Derivate term.
 *   pid  Struct with PID status.
 */
void pid_Init(int p_factor, int i_factor, int d_factor, struct PID_DATA *pid)
// Set up PID controller parameters
{
  // Start values for PID controller
  pid->sumError = 0;
  pid->lastProcessValue = 0;
  // Tuning constants for PID loop
  pid->P_Factor = p_factor;
  pid->I_Factor = i_factor;
  pid->D_Factor = d_factor;
}


/*! PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *   setPoint  Desired value.
 *   processValue  Measured value.
 *   pid_st  PID status struct.
 */
double pid_Controller(int setPoint, int processValue, struct PID_DATA *pid_st)
{
  int error;
  long temp;
  double pid_value, p_term, i_term, d_term;
  
  error = setPoint - processValue;

  p_term = (double)(pid_st->P_Factor * error)/SCALING_FACTOR;

  // Calculate Iterm and limit integral runaway
  temp = pid_st->sumError + error;
  pid_st->sumError = temp;
  i_term = (double)(pid_st->I_Factor * pid_st->sumError)/SCALING_FACTOR;

  // Calculate Dterm
  d_term = (double)(pid_st->D_Factor * (pid_st->lastProcessValue - processValue))/SCALING_FACTOR;
  pid_st->lastProcessValue = processValue;

  pid_value = (p_term + i_term + d_term);

  return pid_value;
}

/*! Resets the integrator.
 *
 *  Calling this function will reset the integrator in the PID regulator.
 */
void pid_Reset_Integrator(pidData_t *pid_st)
{
  pid_st->sumError = 0;
}
