//BEGIN main.cpp (systemc)
// -*- C++ -*- vim600:sw=2:tw=80:fdm=marker:fmr=<<<,>>>

///////////////////////////////////////////////////////////////////////////////
// $Info: Prototypical sc_main $
//
// This is an example of a slightly fancier sc_main than normal that manages
// exceptions and errors more gracefully than the simplistic version used by
// most SystemC examples. Also handles Windows issue exiting without pausing.

///////////////////////////////////////////////////////////////////////////////
// $License: Apache 2.0 $
//
// This file is licensed under the Apache License, Version 2.0 (the "License").
// You may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

///////////////////////////////////////////////////////////////////////////////
#include "top.h"
#include "pause_on_exit.h" /* for windows */
using namespace sc_core;
using namespace std;

namespace {
  // Declare string used as message identifier in SC_REPORT_* calls
  static char const * const MSGID = "/Doulos/example/main";
  // Embed file version information into object to help forensics
  static char const * const RCSID = "(@)$Id: main.cpp  1.0 09/02/12 10:00 dcblack $";
  //                                         FILENAME  VER DATE     TIME  USERNAME
}

///////////////////////////////////////////////////////////////////////////////
int
sc_main
( int argc
, char *argv[]
)
{
  pause_on_exit if_windows;
  // Elaborate
  top_module* top_instance; //< sc_module that also inherits from special report class
  try {
    top_instance = new top_module("top_instance");
  } catch (std::exception& e) {
    SC_REPORT_ERROR(MSGID,e.what());
    SC_REPORT_INFO(MSGID,"Please fix elaboration errors and retry.");
    return 1;
  }//endtry

  // Simulate
  try {
    SC_REPORT_INFO(MSGID,"Starting kernal");
    sc_start();
    SC_REPORT_INFO(MSGID,"Exited kernal");
  } catch (std::exception& e) {
    std::ostringstream mout;
    mout << "Caught exception " << e.what();
    SC_REPORT_WARNING(MSGID,mout.str().c_str()); mout.str("");
  } catch (...) {
    SC_REPORT_ERROR(MSGID,"Caught exception during simulation");
  }//endtry
  if (not sc_end_of_simulation_invoked()) {
    SC_REPORT_INFO(MSGID,"ERROR: Simulation stopped without explicit sc_stop()");
    sc_stop();
  }//endif

  delete top_instance;
  unsigned int total_errors = sc_core::sc_report_handler::get_count(sc_core::SC_ERROR)
                            + sc_core::sc_report_handler::get_count(sc_core::SC_FATAL);
  return ((total_errors>0)?1:0);
}//end sc_main
///////////////////////////////////////////////////////////////////////////////
//END @() $Id: main.cpp,v 1.0 2012/09/01 05:20:48 dcblack Exp $
