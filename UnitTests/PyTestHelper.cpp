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
#include "CppUnitTest.h"

#include "PyObjectHelper.h"


namespace UnitTests
{

using namespace ARTKBlender;

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

// import module and return its dictionary
static PyObject * importModuleDict(const std::string & fileName, std::wstring & error)
{
  // try to import module
  PyObjectOwner module (PyImport_ImportModule(fileName.c_str()));
  if (module.isNull())
  {
    // if failed, report error
    std::wstringstream errStr;
    errStr << fileName.c_str() << ": module not found";
    error = errStr.str();
    return nullptr;
  }
  // get module's dictionary
  return PyModule_GetDict(module.get());
}

// call python function
static std::wstring callPythonFunction(PyObject * modDict, const std::string & testName, PyObject * args = nullptr)
{
  std::wostringstream error;
  // get function object
  PyObject * func = PyDict_GetItemString(modDict, testName.c_str());
  if (func != nullptr && PyCallable_Check(func))
  {
    // call function
    PyObjectOwner rslt(PyObject_CallObject(func, args));
    // check result of function
    if (!rslt.isNull() && PyObject_IsTrue(rslt.get()) != 0)
      // return if success
      return error.str();
    // otherwise prepare error message
    error << testName.c_str();
    PyObjectOwner excDesc[3];
    PyErr_Fetch(&excDesc[0].get(), &excDesc[1].get(), &excDesc[2].get());
    for (auto & exc : excDesc)
      if (!exc.isNull())
      {
        PyObjectOwner excStr(PyObject_Str(exc.get()));
        error << " : " << PyUnicode_AsUnicode(excStr.get());
      }
  }
  else
    // report function not found
    error << "function: " << testName.c_str() << " not found";
  // return error message
  return error.str();
}

// run one test from python file
std::wstring PyTestHelper::RunTest (const std::string & fileName, const std::string & testName, PyObject * args)
{
  std::wstring error;
  // get module dictionary
  PyObjectOwner modDict (importModuleDict(fileName, error));
  if (modDict.isNull())
    return error;
  // get function result
  return callPythonFunction(modDict.get(), testName, args);
}


// run tests from python file
std::vector<std::wstring> PyTestHelper::RunTests (const std::string & fileName, const char testPrefix[])
{
  // test functions' prefix size
  const size_t testPrefixLen = strlen(testPrefix);
  
  // resulting list of error messages
  std::vector<std::wstring> result;

  // get module dictionary
  std::wstring error;
  PyObjectOwner modDict(importModuleDict(fileName, error));
  if (modDict.isNull())
  {
    result.push_back(error);
    return result;
  }
  // get module object names
  PyObject * keys = PyDict_Keys(modDict.get());
  for (int i = 0; i < PyList_Size(keys); ++i)
  {
    PyObject * key = PyList_GetItem(keys, i);
    if (PyUnicode_Check(key))
    {
      // check name of object for test prefix
      std::string keyStr (PyUnicode_AsUTF8(key));
      // if prefix matches
      if (keyStr.compare(0, testPrefixLen, testPrefix) == 0)
      {
        // get function result
        std::wstring error = callPythonFunction(modDict.get(), keyStr, nullptr);
        if (!error.empty())
          result.push_back(error);
      }
    }
  }
  // return error messages
  return result;
}


// run python function, test assert when failed
void AssertPythonFunction(const std::string & fileName, const std::string & testName, PyObject * args)
{
  PyTestHelper pyTest;
  auto rslt = pyTest.RunTest(fileName, testName, args);
  Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(rslt.empty(), rslt.c_str());
}

// run all test functions from module, test assert when any of them failed
void AssertPythonModule(const std::string & fileName, const char testPrefix[])
{
  PyTestHelper pyTest;
  auto rslt = pyTest.RunTests(fileName, testPrefix);
  std::wostringstream errors;
  for (auto errStr : rslt)
    errors << errStr.c_str() << std::endl;
  Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsTrue(rslt.empty(), errors.str().c_str());
}

}
