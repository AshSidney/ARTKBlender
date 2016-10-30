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

#include <Python.h>
#include <vector>
#include <string>


namespace UnitTests
{

/**
    Class for running tests from Python files.
    Object of this class loads python file as module and runs every function
    with prefix "test". These functions must return string value.
    If empty string is returned, test succeded. Otherwise error message is returned.
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
};


/// global helper object
extern PyTestHelper pyTestHelper;

/**
    Function to run one python function from file.
    \param fileName name of test file (without .py extension)
    \param testName name of test function
    \param args     arguments for test function
    \return error text, if empty, test succeded
*/
std::wstring runTest (const std::string & fileName, const std::string & testName, PyObject * args = nullptr);

/**
    Function to run python scripts from file. Runned functions shouldn't have parameters.
    \param fileName   name of test file (without .py extension)
    \param testPrefix prefix of functions to run
    \return vector of error texts
*/
std::vector<std::wstring> runTests (const std::string & fileName, const char testPrefix[] = "test_");

/**
    Function to run one python function from file and check its result.
    \param fileName name of test file (without .py extension)
    \param testName name of test function
    \param args     arguments for test function
*/
void AssertPythonFunction (const std::string & fileName, const std::string & testName, PyObject * args = nullptr);

/**
    Function to run python scripts from file and check their results. Runned functions shouldn't have parameters.
    \param fileName   name of test file (without .py extension)
    \param testPrefix prefix of functions to run
*/
void AssertPythonModule (const std::string & fileName, const char testPrefix[] = "test_");

}
