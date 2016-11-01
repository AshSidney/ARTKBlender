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

#include "ARHandle.h"

#include "ARParam.h"
#include "ARPattHandle.h"
#include "ARMarkerInfo.h"
#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

#include <bgl.h>


namespace ARTKBlender
{

/// ARHandle object allocation
PyObject * PyARHandle_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  // allocate object
  PyObject * self = type->tp_alloc(type, 0);
  // initialize object structure
  PyARHandle * selfObj = getPyType<PyARHandle>(self);
  selfObj->handle = nullptr;
  selfObj->paramLT = nullptr;
  selfObj->attachPatt = new PyObjectOwner;
  selfObj->markers = new PyObjectOwner(PyTuple_New(0));
  selfObj->updateMarkers = false;
  // return allocated object
  return self;
}

// ARHandle object deallocation
void PyARHandle_dealloc(PyARHandle * self)
{
  // release data
  arPattDetach(self->handle);
  arDeleteHandle(self->handle);
  arParamLTFree(&self->paramLT);
  delete self->attachPatt;
  delete self->markers;
  // release object
  deallocPyObject(self);
}

// ARHandle object initialization
int PyARHandle_init(PyARHandle * self, PyObject *args, PyObject *kwds)
{
  // parse parameter
  PyObject *param = NULL;
  int pixFmt = -1;
  static char *kwlist[] = { "param", "pixelFormat", NULL };
  if (!PyArg_ParseTupleAndKeywords(args, kwds, "O!i", kwlist, &ARParamType, &param, &pixFmt))
    return -1;

  // check pixel format
  if (pixFmt < AR_PIXEL_FORMAT_INVALID || pixFmt > AR_PIXEL_FORMAT_MAX)
    return -1;

  // create lookup table
  self->paramLT = arParamLTCreate(getPyType<PyARParam>(param)->param, AR_PARAM_LT_DEFAULT_OFFSET);
  if (self->paramLT == nullptr)
    return -1;

  // create handle
  self->handle = arCreateHandle(self->paramLT);
  if (self->handle == nullptr)
    return -1;

  // set pixel format
  if (arSetPixelFormat(self->handle, AR_PIXEL_FORMAT(pixFmt)) < 0)
    return -1;

  return 0;
}


// get pixel format
PyObject * PyARHandle_getPixelFormat(PyARHandle * self, void * closure)
{
  return Py_BuildValue("i", self->handle->arPixelFormat);
}

// get attached pattern handle
PyObject * PyARHandle_getAttachPatt(PyARHandle * self, void * closure)
{
  return self->attachPatt->returnValue();
}

// attach pattern handle
int PyARHandle_setAttachPatt (PyARHandle * self, PyObject *value, void *closure)
{
  // check value
  if (value != NULL && !isInstance(value, ARPattHandleType))
  {
    PyErr_SetString(PyExc_TypeError, "Value has to be ARPattHandle object");
    return -1;
  }

  // if value is null, detach pattern handle
  if (value == NULL)
  {
    if (arPattDetach(self->handle) < 0)
    {
      PyErr_SetString(PyExc_TypeError, "Detaching of pattern handle failed");
      return -1;
    }
  }
  // otherwise attach pattern handle
  else if (arPattAttach(self->handle, getPyType<PyARPattHandle>(value)->handle) < 0)
  {
    PyErr_SetString(PyExc_TypeError, "Attaching of pattern handle failed");
    return -1;
  }

  // set new value
  *self->attachPatt = PyObjectOwner(value, true);

  return 0;
}

// get detected markers list
PyObject * PyARHandle_getMarkers(PyARHandle * self, void * closure)
{
  // check if markers should be updated
  if (self->updateMarkers)
  {
    self->updateMarkers = false;
    // create new tuple of markers
    size_t markerCount = arGetMarkerNum(self->handle);
    PyObjectOwner pyMarkers (PyTuple_New(markerCount));
    // get marker data
    if (markerCount > 0)
    {
      ARMarkerInfo * markers = arGetMarker(self->handle);
      for (size_t i = 0; i < markerCount; ++i)
      {
        PyARMarkerInfo * pyMarker = PyObject_New(PyARMarkerInfo, &ARMarkerInfoType);
        pyMarker->marker = &markers[i];
        PyTuple_SetItem(pyMarkers.get(), i, getPyObject(pyMarker));
      }
    }
    // set new tuple
    *self->markers = pyMarkers;
  }

  // return list of markers
  return self->markers->returnValue();
}

// check if object is instance of Blender's bgl.Buffer, then return pointer to raw data, otherwise nullptr
static ARUint8 * get_bgl_Buffer_data (PyObject * obj)
{
  static PyTypeObject * bglBufferType = nullptr;
  // initialize pointer to bgl.Buffer type
  if (bglBufferType == nullptr)
  {
    if (strcmp(obj->ob_type->tp_name, "bgl.Buffer") == 0)
      bglBufferType = obj->ob_type;
    else
      return nullptr;
  }
  // check if type is correct
  if (obj->ob_type != bglBufferType)
    return nullptr;
  // return pointer to buffer
  return reinterpret_cast<ARUint8*>(getPyType<Buffer>(obj)->buf.asbyte);
}

// detect markers in image data
PyObject * PyARHandle_detect(PyARHandle * self, PyObject * args)
{
  // get image data
  PyObject * image;
  if (!PyArg_ParseTuple(args, "O", &image))
    Py_RETURN_FALSE;

  // get image buffer
  ARUint8 * buffer = get_bgl_Buffer_data(image);

  // process image data to detect markers
  if (arDetectMarker(self->handle, buffer) < 0)
    Py_RETURN_FALSE;

  // set flag to update markers
  self->updateMarkers = true;

  Py_RETURN_TRUE;
}


// members descriptions
PyGetSetDef PyARHandle_getseters[] =
{
  { "pixelFormat", (getter)PyARHandle_getPixelFormat, NULL,
  "pixel format", NULL },
  { "attachPatt", (getter)PyARHandle_getAttachPatt, (setter)PyARHandle_setAttachPatt,
  "attached pattern handle", NULL },
  { "markers", (getter)PyARHandle_getMarkers, NULL,
  "list of detected markers", NULL },
  { NULL }  /* Sentinel */
};

/// methods descriptions
PyMethodDef PyARHandle_methods[] =
{
  { "detect", (PyCFunction)PyARHandle_detect, METH_VARARGS,
  "Detects markers in image data, return true, if successful" },
  { NULL }  /* Sentinel */
};


/// python type structure for ARHandle
PyTypeObject ARHandleType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.ARHandle",    /* tp_name */
  sizeof(PyARHandle),        /* tp_basicsize */
  0,                         /* tp_itemsize */
  (destructor)PyARHandle_dealloc,  /* tp_dealloc */
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
  "ARHandle objects",        /* tp_doc */
  0,                         /* tp_traverse */
  0,                         /* tp_clear */
  0,                         /* tp_richcompare */
  0,                         /* tp_weaklistoffset */
  0,                         /* tp_iter */
  0,                         /* tp_iternext */
  PyARHandle_methods,        /* tp_methods */
  0,                         /* tp_members */
  PyARHandle_getseters,      /* tp_getset */
  0,                         /* tp_base */
  0,                         /* tp_dict */
  0,                         /* tp_descr_get */
  0,                         /* tp_descr_set */
  0,                         /* tp_dictoffset */
  (initproc)PyARHandle_init, /* tp_init */
  0,                         /* tp_alloc */
  PyARHandle_new,            /* tp_new */
};


// registration object
static PyTypeRegistration ARHandleReg("ARHandle", ARHandleType);

}