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

#include "PyObjectHelper.h"

namespace ARTKBlender
{

// static list of types
PyTypeRegistration * PyTypeRegistration::firstType = nullptr;


// constructor
PyTypeRegistration::PyTypeRegistration (const char * name, PyTypeObject & data)
  : typeName(name), typeData(data), nextType(firstType), prevType(nullptr)
{
  // store type as first
  firstType = this;
  // add itself as previous type to the next one
  if (nextType != nullptr)
    nextType->prevType = this;
}

// destructor
PyTypeRegistration::~PyTypeRegistration(void)
{
  // interconnect neighbor types in linked list
  if (prevType != nullptr)
    prevType->nextType = nextType;
  if (nextType != nullptr)
    nextType->prevType = prevType;
  // if this is first type, reset pointer to it
  if (firstType == this)
    firstType = nextType;
}

// prepare python type for registration
bool PyTypeRegistration::getReady (void)
{
  return PyType_Ready(&typeData) == 0;
}

// add python type to module
void PyTypeRegistration::addType (PyObject * module)
{
  Py_INCREF(&typeData);
  PyModule_AddObject(module, typeName, (PyObject *)&typeData);
}

// prepare all types
bool PyTypeRegistration::getAllReady (void)
{
  for (auto currType = firstType; currType != nullptr; currType = currType->nextType)
    if (!currType->getReady())
      return false;
  return true;
}

// add all types to module
void PyTypeRegistration::addAllTypes (PyObject * module)
{
  for (auto currType = firstType; currType != nullptr; currType = currType->nextType)
    currType->addType(module);
}


// implementation of enumeration type registration

// constructor
PyTypeRegistrationEnum::PyTypeRegistrationEnum (const char * name, PyTypeObject & data, EnumMap & enumVals)
  : PyTypeRegistration(name, data), enumValues(enumVals)
{}

// preparing python type
bool PyTypeRegistrationEnum::getReady(void)
{
  // prepare type
  if (!PyTypeRegistration::getReady())
    return false;
  // add enumeration to dictionary
  for (auto & item : enumValues)
  {
    // check if key is already in dictionary
    PyObjectOwner key(PyUnicode_FromString(item.first.c_str()));
    if (PyDict_Contains(typeData.tp_dict, key.get()) != 0)
      return false;
    // add item to dictionary
    PyObjectOwner value(PyLong_FromLong(item.second));
    if (PyDict_SetItem(typeData.tp_dict, key.get(), value.get()) != 0)
      return false;
  }
  return true;
}

}