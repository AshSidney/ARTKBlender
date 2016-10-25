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

#include "CppUnitTest.h"

#include <Python.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

// test class for PyTypeRegistration using Python
TEST_CLASS(PyARParamPythonTests)
{
public:
  TEST_METHOD_INITIALIZE(InitTest)
  {
    Py_Initialize();
  }

  TEST_METHOD_CLEANUP(ClearTest)
  {
    Py_Finalize();
  }

  TEST_METHOD(PythonTest)
  {
    PySys_SetPath(L"../../UnitTests/Python");
    const wchar_t testPrefix[] = L"test";
    const size_t testPrefixLen = wcslen(testPrefix);

    PyObject * module = PyImport_ImportModule("miniTest");
    PyObject * modDict = PyModule_GetDict(module);
    PyObject * keys = PyDict_Keys(modDict);
    for (int i = 0; i < PyList_Size(keys); ++i)
    {
      PyObject * key = PyList_GetItem(keys, i);
      if (PyUnicode_Check(key))
      {
        std::wstring keyStr (PyUnicode_AsWideCharString(key, NULL));
        if (keyStr.compare(0, testPrefixLen, testPrefix) == 0)
        {
          PyObject * func = PyDict_GetItem(modDict, key);
          if (PyCallable_Check(func))
          {
            PyObject * rslt = PyObject_CallObject(func, NULL);
            bool success = PyObject_IsTrue(rslt) != 0;
            printf("%d", success);
          }
        }
      }
    }

    /*ARTKBlender::PyTypeRegistration pyType("TestPyType", TestPythonType);

    Assert::IsTrue(ARTKBlender::PyTypeRegistration::GetAllReady());

    PyObject * testMod = PyModule_Create(&TestPyModule);

    ARTKBlender::PyTypeRegistration::AddAllTypes(testMod);

    PyObject * modDict = PyModule_GetDict(testMod);
    PyObject * modType = PyDict_GetItemString(modDict, "TestPyType");

    Assert::IsTrue(PyType_CheckExact(modType));*/
  }
};

}
