#ifndef PAUSE_ON_EXIT_H
#define PAUSE_ON_EXIT_H

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
class pause_on_exit //< instantiate at top of sc_main
{
  public:
    pause_on_exit(bool wait_for_keypress=false); // Constructor
    ~pause_on_exit(void); // Destructor
  private:
    static void sighandler(int sig); // Signal handler
    static void notify(void); // Notification of exit
  private:
    static bool s_wait_for_keypress;
};

#endif /*PAUSE_ON_EXIT_H*/
