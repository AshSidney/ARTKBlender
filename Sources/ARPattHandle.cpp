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

#include "ARPattHandle.h"

#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

namespace ARTKBlender
{

/// ARHandle object allocation
PyObject * PyARPattHandle_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  // allocate object
  PyObject * self = type->tp_alloc(type, 0);
  // initialize object structure
  PyARPattHandle * selfObj = getPyType<PyARPattHandle>(self);
  selfObj->handle = arPattCreateHandle();
  // return allocated object
  return self;
}

// ARPattHandle object deallocation
void PyARPattHandle_dealloc(PyARPattHandle * self)
{
  // release data
  arPattDeleteHandle(self->handle);
  // release object
  deallocPyObject(self);
}

// load pattern data file to ARPattHandle object
PyObject * PyARPattHandle_load(PyARPattHandle * self, PyObject * args)
{
  // get file name
  const char * fileName = nullptr;
  if (!PyArg_ParseTuple(args, "s", &fileName))
    return Py_False;

  // load data from file and return pattern ID
  return PyLong_FromLong(arPattLoad(self->handle, fileName));
}


// members descriptions
PyGetSetDef PyARPattHandle_getseters[] =
{
  /*{ "pixelFormat", (getter)PyARHandle_getPixelFormat, NULL,
  "pixel format", NULL },*/
  { NULL }  /* Sentinel */
};

/// methods descriptions
PyMethodDef PyARPattHandle_methods[] =
{
  { "load", (PyCFunction)PyARPattHandle_load, METH_VARARGS,
  "Loads pattern data from file, return index of pattern or -1 if failed" },
  { NULL }  /* Sentinel */
};


/// python type structure for ARHandle
PyTypeObject ARPattHandleType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.ARPattHandle",  /* tp_name */
  sizeof(PyARPattHandle),    /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)PyARPattHandle_dealloc,  /* tp_dealloc */
  0,                         /* tp_print */
  0,                         /* tp_getattr */
  0,                         /* tp_setattr */
  0,                         /* tp_reserved */
  0,                         /* tp_repr */
  0,                         /* tp_as_number */
  0,                         /* tp_as_sequence */
  0,                         /* tp_as_mapping */
  0,                         /* tp_hash  */
  0,                         /* tp_call */
  0,                         /* tp_str */
  0,                         /* tp_getattro */
  0,                         /* tp_setattro */
  0,                         /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT,        /* tp_flags */
  "ARPattHandle objects",    /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  PyARPattHandle_methods,    /* tp_methods */
  0,                         /* tp_members */
  0, //PyARPattHandle_getseters,      /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  0,                         /* tp_init */
  0,                         /* tp_alloc */
  PyARPattHandle_new,        /* tp_new */
};


// registration object
static PyTypeRegistration ARPattHandleReg("ARPattHandle", ARPattHandleType);

}