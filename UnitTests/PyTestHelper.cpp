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

#include "PyTestHelper.h"

#include <Python.h>

#include <sstream>


namespace UnitTests
{

// implementation of PyTestHelper methods

// constructor
PyTestHelper::PyTestHelper ()
{
  // initialize python interpret
  Py_Initialize();
  // setup python path
#ifdef _WIN32
  const wchar_t testPythonPath[] = L".;../../UnitTests/Python";
#else
  const wchar_t testPythonPath[] = L".:../../UnitTests/Python";
#endif
  PySys_SetPath(testPythonPath);
}

// destructor
PyTestHelper::~PyTestHelper ()
{
  // shut down python interpret
  Py_Finalize();
}

// run tests from python file
std::vector<std::wstring> PyTestHelper::RunTests (const std::string & fileName)
{
  // test functions' prefix
  const wchar_t testPrefix[] = L"test_";
  const size_t testPrefixLen = wcslen(testPrefix);
  
  // resulting list of error messages
  std::vector<std::wstring> result;

  // try to import module
  PyObject * module = PyImport_ImportModule(fileName.c_str());
  if (module == nullptr)
  {
    // if failed, report error
    std::wstringstream errStr;
    errStr << fileName.c_str() << ": module not found";
    result.push_back(errStr.str());
    return result;
  }
  // get module's dictionary
  PyObject * modDict = PyModule_GetDict(module);
  PyObject * keys = PyDict_Keys(modDict);
  for (int i = 0; i < PyList_Size(keys); ++i)
  {
    PyObject * key = PyList_GetItem(keys, i);
    if (PyUnicode_Check(key))
    {
      // check name of module object
      std::wstring keyStr (PyUnicode_AsWideCharString(key, NULL));
      // if prefix matches
      if (keyStr.compare(0, testPrefixLen, testPrefix) == 0)
      {
        // get function object
        PyObject * func = PyDict_GetItem(modDict, key);
        if (PyCallable_Check(func))
        {
          // call function
          PyObject * rslt = PyObject_CallObject(func, NULL);
          // get result of function
          bool failed = PyObject_IsTrue(rslt) == 0;
          // if failed, add function name to results
          if (failed)
            result.push_back(keyStr);
        }
      }
    }
  }
  // return error messages
  return result;
}

}
