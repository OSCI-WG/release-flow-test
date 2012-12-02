//BEGIN pause_on_exit.cpp (systemc)
// -*- C++ -*- vim600:sw=2:tw=80:fdm=marker:fmr=<<<,>>>

///////////////////////////////////////////////////////////////////////////////
// $Info: Pauses console if running under windows. $
//
// Instatiating this class in your main function will guarantee that Windows
// will pause on exit.

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
#include "pause_on_exit.h"
#include <cstdlib>
#include <cstdio>
#include <signal.h>
using namespace std;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
pause_on_exit::pause_on_exit(bool wait_for_keypress) //< Constructor
{
  s_wait_for_keypress |= wait_for_keypress;
  signal(SIGINT, &sighandler); //< register with UNIX
  signal(SIGABRT, &sighandler); //< register with UNIX
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
pause_on_exit::~pause_on_exit(void) //< Destructor
{
  notify();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void pause_on_exit::sighandler(int sig) //< Signal handler
{
  printf("POSIX Signal %d caught...\n", sig);
  pause_on_exit::notify();
  exit(1);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void pause_on_exit::notify(void) //< Notification of exit
{
  static bool notified(false);
  if (notified) return;
  notified = true;
  if (s_wait_for_keypress) {
    printf("Press ENTER to exit...");
    char c;
    do {
      c=getc(stdin);
    } while (c != EOF && c != '\n');
  }//endif
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
# ifdef _WIN32
  bool pause_on_exit::s_wait_for_keypress(true);
# else
  bool pause_on_exit::s_wait_for_keypress(false);
# endif
