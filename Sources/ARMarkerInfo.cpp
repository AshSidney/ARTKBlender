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

#include "ARMarkerInfo.h"

#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

namespace ARTKBlender
{

/// ARMarkerInfo object allocation
PyObject * PyARMarkerInfo_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  // allocate object
  PyObject * self = type->tp_alloc(type, 0);
  // initialize object structure
  PyARMarkerInfo * selfObj = getPyType<PyARMarkerInfo>(self);
  selfObj->marker = nullptr;
  // return allocated object
  return self;
}

// ARMarkerInfo object deallocation
void PyARMarkerInfo_dealloc(PyARMarkerInfo * self)
{
  // release object
  deallocPyObject(self);
}

// get ID of detected pattern
PyObject * PyARMarkerInfo_getID(PyARMarkerInfo * self, void * closure)
{
  return PyLong_FromLong(self->marker->id);
}


// members descriptions
PyGetSetDef PyARMarkerInfo_getseters[] =
{
  { "id", (getter)PyARMarkerInfo_getID, NULL,
  "pattern ID", NULL },
  { NULL }  /* Sentinel */
};

/// methods descriptions
PyMethodDef PyARMarkerInfo_methods[] =
{
  /*{ "load", (PyCFunction)PyARParam_load, METH_VARARGS,
  "Loads data from file, return true, if successful" },*/
  { NULL }  /* Sentinel */
};


/// python type structure for ARMarkerInfo
PyTypeObject ARMarkerInfoType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.ARMarkerInfo",  /* tp_name */
  sizeof(PyARMarkerInfo),      /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)PyARMarkerInfo_dealloc,  /* tp_dealloc */
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
  "ARMarkerInfo objects",    /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  0, //PyAR3DHandle_methods,        /* tp_methods */
  0,                         /* tp_members */
  PyARMarkerInfo_getseters,  /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  0,                         /* tp_init */
  0,                         /* tp_alloc */
  PyARMarkerInfo_new,        /* tp_new */
};


// registration object
static PyTypeRegistration ARMarkerInfoReg("ARMarkerInfo", ARMarkerInfoType);

}