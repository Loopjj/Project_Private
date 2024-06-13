/******************************************************************************
 *
 * General PID implementation for HCS12X.
 *
 * Discrete PID controller implementation. Set up by giving P/I/D terms
 * to Init_PID(), and uses a struct PID_DATA to store internal values.
 *
 * - File:               pid.h
 * - Compiler:           CodeWarrior 4.7 for HCS12X
 * - Supported devices:  MC9S12XDT256CAL can be used.
 *****************************************************************************/

#ifndef PID_H
#define PID_H

#define SCALING_FACTOR  128.0
/*! PID Status
 *
 * Setpoints and data used by the PID control algorithm
 */
typedef struct PID_DATA{
  //! Last process value, used to find derivative of process value.
  int lastProcessValue;
  //! Summation of errors, used for integrate calculations
  long sumError;
  //! The Proportional tuning constant
  int P_Factor;
  //! The Integral tuning constant
  int I_Factor;
  //! The Derivative tuning constant
  int D_Factor;
} pidData_t;

void Init_PID(void);
void pid_Init(int p_factor, int i_factor, int d_factor, struct PID_DATA *pid);
double pid_Controller(int setPoint, int processValue, struct PID_DATA *pid_st);
void pid_Reset_Integrator(pidData_t *pid_st);

#endif
