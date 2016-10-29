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

#include "ARParam.h"

#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

namespace ARTKBlender
{

/// ARParam object allocation
PyObject * PyARParam_new (PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  // allocate object
  PyObject * self = type->tp_alloc(type, 0);
  // initialize object structure
  PyARParam * selfObj = getPyType<PyARParam>(self);
  selfObj->param = new ARParam();
  // return allocated object
  return self;
}

// ARParam object deallocation
void PyARParam_dealloc (PyARParam * self)
{
  // release ARParam
  delete self->param;
  // release object
  deallocPyObject(self);
}

// get image size
PyObject * PyARParam_getSize (PyARParam * self, void * closure)
{
  return Py_BuildValue("(ii)", self->param->xsize, self->param->ysize);
}

// set image size
int PyARParam_setSize(PyARParam * self, PyObject *value, void *closure)
{
  // check new value
  bool valid = value != NULL && PyTuple_Check(value) && PyTuple_Size(value) == 2;
  int size[2];
  if (valid)
  {
    // get values from tuple
    for (size_t i = 0; i < 2; ++i)
    {
      PyObject * sizeObj = PyTuple_GetItem(value, i);
      if (PyLong_Check(sizeObj))
        size[i] = PyLong_AsLong(sizeObj);
      else
        valid = false;
    }
  }
  if (valid)
  {
    // store new size values
    arParamChangeSize(self->param, size[0], size[1], self->param);
    return 0;
  }
  else
  {
    // report value error
    PyErr_SetString(PyExc_TypeError, "Tuple of two integers is required");
    return -1;
  }
}

// get matrix
PyObject * PyARParam_getMatrix(PyARParam * self, void * closure)
{
  return Py_BuildValue("((dddd)(dddd)(dddd))",
    self->param->mat[0][0], self->param->mat[0][1], self->param->mat[0][2], self->param->mat[0][3],
    self->param->mat[1][0], self->param->mat[1][1], self->param->mat[1][2], self->param->mat[1][3],
    self->param->mat[2][0], self->param->mat[2][1], self->param->mat[2][2], self->param->mat[2][3]);
}

// get matrix
PyObject * PyARParam_getDistFactor(PyARParam * self, void * closure)
{
  return Py_BuildValue("(ddddddddd)",
    self->param->dist_factor[0], self->param->dist_factor[1], self->param->dist_factor[2],
    self->param->dist_factor[3], self->param->dist_factor[4], self->param->dist_factor[5],
    self->param->dist_factor[6], self->param->dist_factor[7], self->param->dist_factor[8]);
}

// load data file to ARParam object
PyObject * PyARParam_load (PyARParam * self, PyObject * args)
{
  // get file name
  const char * fileName = nullptr;
  if (!PyArg_ParseTuple(args, "s", &fileName))
    return Py_False;

  // load data from file
  return arParamLoad(fileName, 1, self->param) == 0 ? Py_True : Py_False;
}


// members descriptions
PyGetSetDef PyARParam_getseters[] =
{
  { "size", (getter)PyARParam_getSize, (setter)PyARParam_setSize,
    "image size", NULL },
  { "matrix", (getter)PyARParam_getMatrix, NULL,
    "projection matrix", NULL },
  { "distFactor", (getter)PyARParam_getDistFactor, NULL,
    "distorsion parameters", NULL },
    { NULL }  /* Sentinel */
};

/// methods descriptions
PyMethodDef PyARParam_methods[] =
{
  { "load", (PyCFunction)PyARParam_load, METH_VARARGS,
    "Loads data from file, return true, if successful" },
  { NULL }  /* Sentinel */
};


/// python type structure for ARParam
PyTypeObject ARParamType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.ARParam",     /* tp_name */
  sizeof(PyARParam),         /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)PyARParam_dealloc,  /* tp_dealloc */
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
  "ARParam objects",         /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  PyARParam_methods,         /* tp_methods */
  0,                         /* tp_members */
  PyARParam_getseters,       /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  0,                         /* tp_init */
  0,                         /* tp_alloc */
  PyARParam_new,             /* tp_new */
};


// registration object
static PyTypeRegistration ARParamReg("ARParam", ARParamType);

}