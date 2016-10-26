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


namespace UnitTests
{

// implementation of PyTestHelper methods

PyTestHelper::PyTestHelper ();
{
  Py_Initialize();
  PySys_SetPath(L"../../UnitTests/Python");
}

PyTestHelper::~PyTestHelper ();
{
  Py_Finalize();
}

std::vector<std::wstring> PyTestHelper::RunTests (const std::string & fileName)
{
  const wchar_t testPrefix[] = L"test";
  const size_t testPrefixLen = wcslen(testPrefix);
  
  std::vector<std::wstring> result;

  PyObject * module = PyImport_ImportModule(fileName.cstr());
  if (module == nullptr)
    return result;
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
          bool failed = PyObject_IsTrue(rslt) == 0;
          if (failed)
            result.push_back(keyStr);
        }
      }
    }
  }

  return result;
}

}
