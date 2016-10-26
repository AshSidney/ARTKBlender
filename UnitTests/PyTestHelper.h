/*
-----------------------------------------------------------------------------
This source file is part of ARTKBlender library
Copyright (c) 2016 The Zdeno Ash Miklas
ARTKBlender is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with ARTKBlender.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/

#include <vector>
#include <string>


namespace UnitTests
{

/**
    Class for running tests from Python files.
    Object of this class loads python file as module and runs every function
    with prefix "test". These functions must have no parameters and must return
    boolean value. If return is true, test succeded. Otherwise error message
    is returned.
*/
class PyTestHelper
{
public:
  /**
      Constructor starts python interpret.
  */
  PyTestHelper ();

  /**
      Constructor ends python interpret.
  */
  ~PyTestHelper ();

  /**
      Method to run python scrpts from file.
      \param fileName name of test file (without .py extension)
      \return vector of error texts
  */
  std::vector<std::wstring> RunTests (const std::string & fileName);
};

}
