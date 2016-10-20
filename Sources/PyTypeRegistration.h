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

#pragma once

#include <Python.h>
#include <vector>

namespace ARTKBlender
{

/**
*   Class for Python types registration in module.

    Static objects of this class store information of Python types to be able to
    initialize them during module initialization.
*/
class PyTypeRegistration
{
public:
  /**
      Constructor
      \param typeName name of python class
      \param typeData description of python class
  */
  PyTypeRegistration(const char * name, PyTypeObject & data);

  /**
      Function to prepare all registered types.
      \return true, if all type were successfully prepared
  */
  static bool GetAllReady();

  /**
      Function to add all registered types to module.
      \module module object
  */
  static void AddAllTypes(PyObject * module);

protected:
  /// name for python type
  const char * typeName;
  /// data for python type description
  PyTypeObject & typeData;

  /**
  Method to prepare type.
  \return true, if this type was successfully prepared
  */
  virtual bool GetReady();

  /**
  Method to add type to module.
  \module module object
  */
  virtual void AddType(PyObject * module);


  /// global list of types to be registered in module
  static std::vector<PyTypeRegistration*> types;
};

}