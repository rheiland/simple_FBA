#ifdef SIZE_DEFINITIONS
#define N_METABS 4
#define N_ODE_METABS 0
#define N_INDEP_METABS 3
#define N_COMPARTMENTS 1
#define N_GLOBAL_PARAMS 7
#define N_KIN_PARAMS 0
#define N_REACTIONS 3

#define N_ARRAY_SIZE_P  8	// number of parameters
#define N_ARRAY_SIZE_X  3	// number of initials
#define N_ARRAY_SIZE_Y  1	// number of assigned elements
#define N_ARRAY_SIZE_XC 3	// number of x concentration
#define N_ARRAY_SIZE_PC 0	// number of p concentration
#define N_ARRAY_SIZE_YC 1	// number of y concentration
#define N_ARRAY_SIZE_DX 3	// number of ODEs 
#define N_ARRAY_SIZE_CT 1	// number of conserved totals

#endif // SIZE_DEFINITIONS

#ifdef TIME
#define T  <set here a user name for the time variable> 
#endif // TIME

#ifdef NAME_ARRAYS
const char* p_names[] = {"Cell", "Alpha", "Beta", "Gamma", "phenotype.cycle.transition_rate_0_1", "phenotype.death.rate_0", "E_cyc_threshold", "E_death_threshold",  "" };
const char* x_names[] = {"Energy", "Glucose", "Hydrogen",  "" };
const char* y_names[] = {"Oxygen",  "" };
const char* xc_names[] = {"Energy", "Glucose", "Hydrogen",  "" };
const char* pc_names[] = { "" };
const char* yc_names[] = {"Oxygen",  "" };
const char* dx_names[] = {"ODE Energy", "ODE Glucose", "ODE Hydrogen",  "" };
const char* ct_names[] = {"CT Oxygen",  "" };
#endif // NAME_ARRAYS

#ifdef INITIAL
x[0] = 5;	//metabolite 'Energy': reactions
x[1] = 10;	//metabolite 'Glucose': reactions
x[2] = 0;	//metabolite 'Hydrogen': reactions
#endif /* INITIAL */

#ifdef FIXED
ct[0] = 0;	//ct[0] conserved total for 'Oxygen'
p[0] = 1;	//compartment 'Cell':fixed
p[1] = 2;	//global quantity 'Alpha':fixed
p[2] = 1;	//global quantity 'Beta':fixed
p[3] = 0.3;	//global quantity 'Gamma':fixed
p[4] = 0;	//global quantity 'phenotype.cycle.transition_rate_0_1':fixed
p[5] = 0;	//global quantity 'phenotype.death.rate_0':fixed
p[6] = 10;	//global quantity 'E_cyc_threshold':fixed
p[7] = 3;	//global quantity 'E_death_threshold':fixed
#endif /* FIXED */

#ifdef ASSIGNMENT
y[0] = ct[0]+x[1]+1*x[2];	//metabolite 'Oxygen': reactions
x_c[0] = x[0]/p[0];	//concentration of metabolite 'Energy': reactions
x_c[1] = x[1]/p[0];	//concentration of metabolite 'Glucose': reactions
x_c[2] = x[2]/p[0];	//concentration of metabolite 'Hydrogen': reactions
y_c[0] = y[0]/p[0];	//concentration of metabolite 'Oxygen': reactions
#endif /* ASSIGNMENT */

#ifdef FUNCTIONS_HEADERS
#endif /* FUNCTIONS_HEADERS */

#ifdef FUNCTIONS
#endif /* FUNCTIONS */

#ifdef ODEs
dx[0] = (p[1] * y_c[0] * x_c[1]) *p[0]+(p[2] * x_c[1]) *p[0]-(p[3] * x_c[0]) *p[0];
dx[1] = -(p[1] * y_c[0] * x_c[1]) *p[0]-(p[2] * x_c[1]) *p[0];
dx[2] = (p[2] * x_c[1]) *p[0];
#endif /* ODEs */
