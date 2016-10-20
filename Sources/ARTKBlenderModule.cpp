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

#include "PyTypeRegistration.h"

namespace ARTKBlender
{

// module methods
static PyMethodDef moduleMethods[] =
{
  { NULL }  /* Sentinel */
};

// module definition
static PyModuleDef ARTKBlenderModule =
{
  PyModuleDef_HEAD_INIT,
  "artk",
  "ARTK Python interface providing access to ARToolKit objects for Blender (Game Engine).",
  -1,
  moduleMethods
};


// initialization of module
PyMODINIT_FUNC PyInit_ARTKBlender (void)
{
  // prepare classes
  if (!PyTypeRegistration::GetAllReady())
    return NULL;

  // create module
  PyObject * module = PyModule_Create(&ARTKBlenderModule);
  if (module == NULL)
    return NULL;

  // add types to module
  PyTypeRegistration::AddAllTypes(module);

  return module;
}

}