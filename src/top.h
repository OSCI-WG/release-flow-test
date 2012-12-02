#ifndef TOP_H
#define TOP_H

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

#include <systemc>

class top_module
: public sc_core::sc_module
{
public:
  // Ports - NONE
  // Structure (e.g. submodules) - NONE
  // Constructor
  top_module(sc_core::sc_module_name instance_name)
  : sc_module(instance_name)
  {}
  // Destructor
  virtual ~top_module(void)
  {}
  // Callbacks - NONE
  // Processes - NONE
};

#endif
