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

#include "PyTypeRegistration.h"

namespace ARTKBlender
{

// static list of types
std::vector<PyTypeRegistration*> PyTypeRegistration::types;


// constructor
PyTypeRegistration::PyTypeRegistration (const char * name, PyTypeObject & data)
  : typeName(name), typeData(data)
{
  types.push_back(this);
}

// prepare python type for registration
bool PyTypeRegistration::GetReady ()
{
  typeData.tp_new = PyType_GenericNew;
  return PyType_Ready(&typeData) >= 0;
}

// add python type to module
void PyTypeRegistration::AddType (PyObject * module)
{
  Py_INCREF(&typeData);
  PyModule_AddObject(module, typeName, (PyObject *)&typeData);
}

// prepare all types
bool PyTypeRegistration::GetAllReady ()
{
  for (auto pType : types)
    if (!pType->GetReady())
      return false;
  return true;
}

// add all types to module
void PyTypeRegistration::AddAllTypes (PyObject * module)
{
  for (auto pType : types)
    pType->AddType(module);
}

}