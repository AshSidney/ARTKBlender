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

#include "AR3DHandle.h"

#include "ARParam.h"
#include "ARMarkerInfo.h"
#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

namespace ARTKBlender
{

/// AR3DHandle object allocation
PyObject * PyAR3DHandle_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  // allocate object
  PyObject * self = type->tp_alloc(type, 0);
  // initialize object structure
  PyAR3DHandle * selfObj = getPyType<PyAR3DHandle>(self);
  selfObj->handle = nullptr;
  // return allocated object
  return self;
}

// AR3DHandle object deallocation
void PyAR3DHandle_dealloc(PyAR3DHandle * self)
{
  // release data
  ar3DDeleteHandle(&self->handle);
  // release object
  deallocPyObject(self);
}

// AR3DHandle object initialization
int PyAR3DHandle_init(PyAR3DHandle * self, PyObject *args, PyObject *kwds)
{
  // parse parameter
  PyObject *param = NULL;
  static char *kwlist[] = { "param", NULL };
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!", kwlist, &ARParamType, &param))
    return -1;

  // create handle
  self->handle = ar3DCreateHandle(getPyType<PyARParam>(param)->param);
  if (self->handle == nullptr)
    return -1;

  return 0;
}

// return tuple with matrix values
static PyObject * buildMatrix(double mat[][4])
{
  // build matrix rows
  return Py_BuildValue("((dddd)(dddd)(dddd)(dddd))", mat[0][0], mat[0][1], mat[0][2], mat[0][3],
    -mat[1][0], -mat[1][1], -mat[1][2], -mat[1][3], -mat[2][0], -mat[2][1], -mat[2][2], -mat[2][3],
    0.0, 0.0, 0.0, 1.0);
}

// detect markers in image data
PyObject * PyAR3DHandle_getTransMatSquare(PyAR3DHandle * self, PyObject * args)
{
  // get arguments
  PyObject * marker;
  double width;
  if (!PyArg_ParseTuple(args, "O!d", &ARMarkerInfoType, &marker, &width))
    Py_RETURN_NONE;

  // process image data to detect markers
  double mat[3][4];
  double err = arGetTransMatSquare(self->handle, getPyType<PyARMarkerInfo>(marker)->marker, width, mat);

  // return matrix tuple
  return buildMatrix(mat);
}

// detect markers in image data
PyObject * PyAR3DHandle_getTransMatSquareCont(PyAR3DHandle * self, PyObject * args)
{
  // get arguments
  PyObject * marker;
  double width;
  double mat[4][4];
  if (!PyArg_ParseTuple(args, "O!d((dddd)(dddd)(dddd)(dddd))", &ARMarkerInfoType, &marker, &width,
      &mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3], &mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
      &mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3], &mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]))
    Py_RETURN_NONE;

  // invert values in the second and third row
  for (size_t i = 1; i < 3; ++i)
    for (size_t j = 0; j < 4; ++j)
      mat[i][j] = -mat[i][j];

  // process image data to detect markers
  double err = arGetTransMatSquareCont(self->handle, getPyType<PyARMarkerInfo>(marker)->marker, mat, width, mat);

  // return matrix tuple
  return buildMatrix(mat);
}


// members descriptions
PyGetSetDef PyAR3DHandle_getseters[] =
{
  /*{ "pixelFormat", (getter)PyARHandle_getPixelFormat, NULL,
  "pixel format", NULL },*/
  { NULL }  /* Sentinel */
};

/// methods descriptions
PyMethodDef PyAR3DHandle_methods[] =
{
  { "getTransMatSquare", (PyCFunction)PyAR3DHandle_getTransMatSquare, METH_VARARGS,
  "Get transformation matrix for detected square marker with specified width." },
  { "getTransMatSquareCont", (PyCFunction)PyAR3DHandle_getTransMatSquareCont, METH_VARARGS,
  "Get transformation matrix for detected square marker with specified width continuing from previous matrix." },
  { NULL }  /* Sentinel */
};


/// python type structure for AR3DHandle
PyTypeObject AR3DHandleType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.AR3DHandle",  /* tp_name */
  sizeof(PyAR3DHandle),      /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)PyAR3DHandle_dealloc,  /* tp_dealloc */
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
  "AR3DHandle objects",      /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  PyAR3DHandle_methods,      /* tp_methods */
  0,                         /* tp_members */
  0, //PyAR3DHandle_getseters,      /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)PyAR3DHandle_init, /* tp_init */
  0,                         /* tp_alloc */
  PyAR3DHandle_new,          /* tp_new */
};


// registration object
static PyTypeRegistration AR3DHandleReg("AR3DHandle", AR3DHandleType);

}