// file: randv_02.cc
// version: $Id: randv_02.cc 2013-07-20 Amir Harati $
//

// isip include files
//
#include "Dirichlet.h"
#include <Console.h>
#include <VectorFloat.h>

// method: diagnose
//
// arguments:
//  Integral::DEBUG level: (input) debug level for diagnostics
//
// return: a bool8 value indicating status
//
bool8 Dirichlet::diagnose(Integral::DEBUG level_a) {

  // local  variables
  //
  int32 N = 1000;
  float64 avg;
  
  //----------------------------------------------------------------------
  //
  // 0. preliminaries
  //
  //----------------------------------------------------------------------

  // output the class name
  //
  if (level_a > Integral::NONE) {
    SysString output(L"diagnosing class ");
    output.concat(CLASS_NAME);
    output.concat(L": ");
    Console::put(output);
    Console::increaseIndention();
  }

  //---------------------------------------------------------------------
  //
  // 1. required public methods
  //
  //---------------------------------------------------------------------

  // set indentation
  //
  if (level_a > Integral::NONE) {
    Console::put(L"testing required public methods...\n");
    Console::increaseIndention();
  }

  // test the debug methods
  //
  setDebug(debug_level_d);
  
  if (level_a > Integral::BRIEF) {
    Dirichlet tmp;
    tmp.debug(L"debug");
  }
  
  // testing class constructors
  //
  Dirichlet rand_1;
  Dirichlet rand_2(rand_1);
  Dirichlet rand_3;

  // testing the operator= operator and assign method
  //
  rand_3 = rand_1;

  // testing i/o methods 
  //

  // testing the equality method
  //
  if (!rand_3.eq(rand_1)) {
    return Error::handle(name(), L"assign method/eq", Error::TEST,
			 __FILE__, __LINE__);
  }  
  if (!rand_2.eq(rand_1)) {
    return Error::handle(name(), L"copy constructor/eq", Error::TEST,
			 __FILE__, __LINE__);
  }
  
  // testing memory management methods
  //
  Dirichlet::setGrowSize((int32)500);

  for (int32 j = 1; j <= 10; j++) {
    Random** dyn_cal = new Random*[j * 100];
    
    // create the objects
    //
    for (int32 i = 0; i < j * 100; i++) {
      dyn_cal[i] = new Random();
    }
    
    // delete objects
    //
    for (int32 i = (j * 100) - 1; i >= 0; i--) {
      delete dyn_cal[i];
    }
      
    delete [] dyn_cal;
  }

  // reset indentation
  //
  if (level_a > Integral::NONE) {
    Console::decreaseIndention();
  }

 //---------------------------------------------------------------------
 //
 // 2. class-specific public methods
 //     initialization and computational methods for Dirichlet
 //
 //---------------------------------------------------------------------
 
 // set indentation
 //
 if (level_a > Integral::NONE) {
    Console::put(L"testing class-specific public methods: Dirichlet compute methods...\n");
    Console::increaseIndention(); 
 }

 VectorFloat params; 
 params.assign(L"1,1,1");
 Dirichlet rand_4(params);
 VectorFloat r;
 
 // set the seed
 //
 if (!rand_4.seed(1)) {
   return Error::handle(name(), L"seed", Error::TEST, __FILE__, __LINE__);
 }

 // check the exact numbers being produced:
 //  for the default algorithm, and a seed = 1, the results should
 //  be as shown below.
 //
    
 rand_4.get(r);
  
 // check the size of result
  //
 if (r.length() != 3) {
    return Error::handle(name(), L"result", Error::TEST, __FILE__, __LINE__);
 }  
  
 if ((!Integral::almostEqual((float64)0.584492, (float64)r(0))) ||
     (!Integral::almostEqual((float64)0.0416952, (float64)r(1))) ||
     (!Integral::almostEqual((float64)0.373813, (float64)r(2)))) {
   return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
 }
 

 // compute some random numbers
 //
 VectorFloat tmp2(N);
 String tmp;
 tmp2.setRange(0,N,(float64)1);
 tmp2.get(tmp);  
 params.assign(tmp);

 if (!rand_4.setParameters(params)) {
   return Error::handle(name(), L"set parameters", Error::TEST,
			__FILE__, __LINE__);
 }
 
 rand_4.get(r);
 avg=r.mean();

 if (!Integral::almostEqual((float64)avg, (float64)0.0, 1.0,1.0)) {
   return Error::handle(name(), L"compute", Error::TEST, __FILE__, __LINE__);
 }
 
 // reset indentation
 //
 if (level_a > Integral::NONE) {
   Console::decreaseIndention();
 }  

 //------------------------------1---------------------------------------
 //
 // 3. print completion message
 //
 //---------------------------------------------------------------------

 // reset indentation
 //
 if (level_a > Integral::NONE) {
   Console::decreaseIndention();
 }
 
 if (level_a > Integral::NONE) {
   SysString output(L"diagnostics passed for class ");
   output.concat(name());
   output.concat(L"\n");
   Console::put(output);
 }
 
 // exit gracefully
 //
 return true;
}

