/*
-----------------------------------------------------------------------------
This source file is part of ARTKBlender library

Copyright (c) 2016 The Zdeno Ash Miklas

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#include <Python.h>

// module methods
static PyMethodDef moduleMethods[] =
{
  { NULL }  /* Sentinel */
};

// module definition
static PyModuleDef ARTKBlenderModule = {
  PyModuleDef_HEAD_INIT,
  "artkblender",
  "ARTK Python interface providing access to ARToolKit objects for Blender (Game Engine).",
  -1,
  NULL, NULL, NULL, NULL, NULL
};


// initialization of module
PyMODINIT_FUNC PyInit_ARTKBlender (void)
{
  // prepare classes
  /*if (PyType_Ready(&WaverType) < 0) return;
  if (PyType_Ready(&EmiterType) < 0) return;*/

  PyObject * module = PyModule_Create(&ARTKBlenderModule);
  if (module == NULL)
    return NULL;

  /*Py_INCREF(&WaverType);
  PyModule_AddObject(m, "Waver", (PyObject*)&WaverType);
  Py_INCREF(&EmiterType);
  PyModule_AddObject(m, "Emiter", (PyObject*)&EmiterType);*/

  return module;
}
