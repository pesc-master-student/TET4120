
/************************************************************************************
::File Name: sfun_load_emu.cpp


                    /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$
                   | $$__  $$| $$_____/ /$$__  $$ /$$__  $$
                   | $$  \ $$| $$      | $$  \__/| $$  \__/
                   | $$$$$$$/| $$$$$   |  $$$$$$ | $$
                   | $$____/ | $$__/    \____  $$| $$
                   | $$      | $$       /$$  \ $$| $$    $$
                   | $$      | $$$$$$$$|  $$$$$$/|  $$$$$$/
                   |__/      |________/ \______/  \______/
             Department of Electric Power Engineering, NTNU 2022

::GITHUB REPOSITORY::
  - https://github.com/ntnu-pesc/TET4120_LAB (restricted access)

::DESCRIPTION::
 - System functions (s-functions) provide a powerful mechanism for extending capabilities of 
   the MATLAB and Simulink environment. M-file, Fortran and C/C++ supported.
 - This file is responsible for communicating with the Simulink engine API during simulation.
 - Actual s-function block functionality is defined through external objects.
 - Class definition/declaration are provided as separate .cpp/.hpp files.

::DEPENDENCIES::
 - SwLoadEmulator.cpp
 - SwLoadEmulator.hpp

::WHAT YOU NEED::
 - Complete solution requires three components:
   1) C++ source files implementing actual object behaviour
   2) Interface file (this) connecting to S-Function API
   3) Simulink model with S-Function block.
 - MATLAB/Simulink R2022a. Plain installation, no Toolboxes/Add-Ons required.
 - MATLAB-supported compiler (MinGW64 Compiler reccomended).
 - Dependencies in same location as this file (project folder).
 - IDE to edit source code (Xilinx SDK or Visual Studio Code).


::SIMULINK API::
 - This file implements the minimum functionality required to correctly interract
   with the Simulink S-Function application program interface (API).
 - For correct and efficient execution pointers are used, hence care must be taken
   to avoid memory leak/dangling pointers etc. Failing to do so crashes MATLAB.
 - High-level explanation of how S-Functions work (MathWorks): 
   https://se.mathworks.com/help/simulink/sfg/what-is-an-s-function.html


::INTEGRATE INTO SIMULINK MODEL::
 1) From Simulink Library add S-Function block into model canvas.
 2) Double click S-Function block and add S-function name.
 3) The S-Function block gets populated with inputs/outputs according to definitions
    here in this file. Note that ports remain anonymous.
 4) Create mask if more convenient.
 5) As usual the module defined by its respective S-Function must
    be compatible with its surrounding Simulink environment. This means datatype,
    signal width, sampling time and variable or fixed-step solver. 
 6) Run "open_system('sfundemos');" in MATLAB for MathWorks S-Function examples.


::HOW TO BUILD::
 1) In MATLAB command window write:
    "mex sfun_load_emu.cpp SwLoadEmulator.cpp".
 2) Press Enter to let MATLAB/Mex utility compile the source files.
 3) ** Compilation successfull **
      Building with <compiler-name> MEX completed successfully.
    ** Compilation failed **
      Compiler throws error messages, correction of src files or build params needed.
 4) Output artifact is a MATLAB executable (mex) file "sfun_load_emu.mexw64".
 5) When adding additional source files:
    - Put CPP/HPP files in project folder
    - Add #include <declaration-file-name.hpp> in header section 
    - Add <definition-file-name.cpp> as additional input to mex command
    - Recompile and check for error messages from compiler or Simulink.

*************************************************************************************
MODIFICATION HISTORY:

Ver  Author  			  		 Date       Changes
---- --------------------------- ---------- -------------------------------------
1.00 thomas.haugan@ntnu.no       01.08.2022 Original code release.
1.01 ***                         08.08.2022 First working example (3rd party code).
1.02 ***                         12.08.2022 Included working implementation pump load. 
1.03 ***                         16.08.2022 Cleaned up code. Added documentation.
1.05 ***                         18.08.2022 Added: graphics + support 4 parameters
                                            supplied from mask + 2 input signals.   
1.06 thomas.haugan@ntnu.no       02.09.2022 Corrected ENUM translation error.                                           
*************************************************************************************
TODO:
************************************************************************************/


#include "SwLoadEmulator.hpp"

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  sfun_load_emu  // Instantiate function name in S-function block
#include "simstruc.h"

#define NUM_OF_INPUTS 3
#define NUM_OF_OUTPUTS 1
#define SAMPLE_TIME 0.001
#define OFFSET_TIME 0.0

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) &&\
!mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

#define RUN_TIME_DATA_TYPE SS_DOUBLE
#if RUN_TIME_DATA_TYPE == SS_DOUBLE
typedef real_T RunTimeDataType;
#endif

#define NPARAMS 4

#define VAR0_IDX 0
#define AUX_PARAM_0(S) ssGetSFcnParam(S,VAR0_IDX) 

#define VAR1_IDX 1
#define AUX_PARAM_1(S) ssGetSFcnParam(S,VAR1_IDX) 

#define VAR2_IDX 2
#define AUX_PARAM_2(S) ssGetSFcnParam(S,VAR2_IDX) 

#define LOAD_SELECT 3
#define LOAD_SELECTOR(S) ssGetSFcnParam(S,LOAD_SELECT) 



#define MDL_CHECK_PARAMETERS
#if defined(MDL_CHECK_PARAMETERS)  && defined(MATLAB_MEX_FILE)
  /*
   * Check to make sure that each parameter is 1-d and positive
   */
static void mdlCheckParameters(SimStruct* S){

    if (!mxIsDouble(AUX_PARAM_0(S)) ||
        (mxGetNumberOfElements(AUX_PARAM_0(S)) != 1)) {
        ssSetErrorStatus(S, "Aux Param 0 must be entered as a scalar double.");
        return;
    }

    if (!mxIsDouble(AUX_PARAM_1(S)) ||
        (mxGetNumberOfElements(AUX_PARAM_1(S)) != 1)) {
        ssSetErrorStatus(S, "Aux Param 1 must be entered as a scalar double.");
        return;
    }

    if (!mxIsDouble(AUX_PARAM_2(S)) ||
        (mxGetNumberOfElements(AUX_PARAM_2(S)) != 1)) {
        ssSetErrorStatus(S, "Aux Param 2 must be entered as a scalar double.");
        return;
    }
}
#endif


/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct* S){
    ssSetNumSFcnParams(S, NPARAMS);  /* Number of expected parameters */
#if defined(MATLAB_MEX_FILE)
    if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
        mdlCheckParameters(S);
        if (ssGetErrorStatus(S) != NULL) {
            return;
        }
    }
    else {
        return; /* Parameter mismatch will be reported by Simulink */
    }
#endif

    ssSetSFcnParamTunable(S, VAR0_IDX, true);
    ssSetSFcnParamTunable(S, VAR1_IDX, true);
    ssSetSFcnParamTunable(S, VAR2_IDX, true);
    ssSetSFcnParamTunable(S, LOAD_SELECT, true);


    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

    if (!ssSetNumInputPorts(S, NUM_OF_INPUTS)) return;
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortOverWritable(S, 0, 1);
    
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortOverWritable(S, 1, 1);
    
    ssSetInputPortWidth(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortOverWritable(S, 2, 1);


    if (!ssSetNumOutputPorts(S, NUM_OF_OUTPUTS)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetNumSampleTimes(S, 1);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); // reserve element in the pointers vector
    ssSetNumModes(S, 0); // to store a C++ object
    ssSetNumNonsampledZCs(S, 0);

    ssSetOperatingPointCompliance(S, USE_CUSTOM_OPERATING_POINT);

    /* Set this S-function as runtime thread-safe for multicore execution */
    ssSetRuntimeThreadSafetyCompliance(S, RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE);
    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct* S){
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
    ssSetSampleTime(S, 0, SAMPLE_TIME);
    ssSetOffsetTime(S, 0, OFFSET_TIME);
}


/* Function: mdlProcessParameters ===========================================
 * Abstract:
 *      Update run-time parameters.
 */
static void mdlProcessParameters(SimStruct* S){

}

#define MDL_START  
#if defined(MDL_START) 
/* Function: mdlStart =======================================================
 * Abstract:
 *    This function is called once at start of model execution. If you
 *    have states that should be initialized once, this is the place
 *    to do it.
 */
static void mdlStart(SimStruct* S){

    ssGetPWork(S)[0] = (void*) new LoadEmulator;
    LoadEmulator* loadEMU = (LoadEmulator*)ssGetPWork(S)[0];  
    loadEMU->Initialize();
}                                            
#endif /*  MDL_START */

/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block.Prefix i/o means input/output port.
 */
static void mdlOutputs(SimStruct* S, int_T tid){
  
    LoadEmulator *loadEMU = (LoadEmulator*) ssGetPWork(S)[0];
 
    // READ INPUTS
    InputRealPtrsType i_Ptrs = ssGetInputPortRealSignalPtrs(S, 0);
    loadEMU->setSig0(*i_Ptrs[1]);
    loadEMU->setSig1(*i_Ptrs[2]);
   
    // PROCESS DATA
    loadEMU->setParamFromMask(0, (real_T) *mxGetPr(AUX_PARAM_0(S)));
    loadEMU->setParamFromMask(1, (real_T) *mxGetPr(AUX_PARAM_1(S)));
    loadEMU->setParamFromMask(2, (real_T) *mxGetPr(AUX_PARAM_2(S)));
    int tempLoadType = *mxGetPr(LOAD_SELECTOR(S)) - 1;
    loadEMU->setLoadType((LoadType)tempLoadType);
   
    // WRITE OUTPUTS
    real_T* o_pump_load = ssGetOutputPortRealSignal(S, 0);
    o_pump_load[0] = loadEMU->Run(*i_Ptrs[0]);

    //real_T* o_aux_var = ssGetOutputPortRealSignal(S, 1);
    //o_aux_var[0] = (double)loadEMU->getLoadType();
    //o_aux_var[0] = loadEMU->getParam(1);
    //o_aux_var[0] = loadEMU->getSinus();

    UNUSED_ARG(tid);                             
}

#ifdef MATLAB_MEX_FILE
/* For now mdlG[S]etSimState are only supported in normal simulation */

/* Define to indicate that this S-Function has the mdlG[S]etOperatingPoint mothods */
#define MDL_OPERATING_POINT

/* Function: mdlGetOperatingPoint =====================================================
 * Abstract:
 *
 */
static mxArray* mdlGetOperatingPoint(SimStruct* S){
    return NULL;
}
/* Function: mdlGetOperatingPoint =====================================================
 * Abstract:
 *
 */
static void mdlSetOperatingPoint(SimStruct* S, const mxArray* ma){

}

#endif


/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it (memory leak).
 */
static void mdlTerminate(SimStruct* S){
    LoadEmulator* L = (LoadEmulator*)ssGetPWork(S)[0];
    delete L;
}                                              


 /*=============================*
  * Required S-function trailer *
  *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

