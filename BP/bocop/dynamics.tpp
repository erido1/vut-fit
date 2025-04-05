// Function for the dynamics of the problem
// dy/dt = dynamics(y,u,z,p)

// The following are the input and output available variables 
// for the dynamics of your optimal control problem.

// Input :
// time : current time (t)
// normalized_time: t renormalized in [0,1]
// initial_time : time value on the first discretization point
// final_time : time value on the last discretization point
// dim_* is the dimension of next vector in the declaration
// state : vector of state variables
// control : vector of control variables
// algebraicvars : vector of algebraic variables
// optimvars : vector of optimization parameters
// constants : vector of constants

// Output :
// state_dynamics : vector giving the expression of the dynamic of each state variable.

// The functions of your problem have to be written in C++ code
// Remember that the vectors numbering in C++ starts from 0
// (ex: the first component of the vector state is state[0])

// Tdouble variables correspond to values that can change during optimization:
// states, controls, algebraic variables and optimization parameters.
// Values that remain constant during optimization use standard types (double, int, ...).

#include "header_dynamics"
{
	// Constants
	double T_max = constants[0];
	double V_ex = constants[1];
	double R_eq = constants[2];
	double mu = constants[3];

	// State vectors
	Tdouble radius = state[0];
	Tdouble theta = state[1];
	Tdouble v_r = state[2];
	Tdouble v_theta = state[3];
	Tdouble psi = state[4];
	Tdouble omega = state[5];
	Tdouble mass = state[6];
	Tdouble int_alpha = state[7];

	// Control variables
	Tdouble throttle = control[0];
	Tdouble alpha = control[1];

	// Dynamics for each state
	state_dynamics[0] = v_r;
	state_dynamics[1] = v_theta/radius;
	state_dynamics[2] = ((pow(v_theta, 2))/radius) - (mu/(pow(radius, 2))) + (((T_max*throttle)/mass)*cos(psi));
	state_dynamics[3] = -((v_r*v_theta)/radius) + (((T_max*throttle)/mass)*sin(psi));
	state_dynamics[4] = omega - (v_theta/radius);
	state_dynamics[5] = alpha; 
	state_dynamics[6] = -((T_max*throttle)/V_ex);
	state_dynamics[7] = 1e-4*alpha*alpha;
}


