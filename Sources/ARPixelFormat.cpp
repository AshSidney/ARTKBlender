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

#include "ARPixelFormat.h"

#include "PyObjectHelper.h"
#include "PyTypeRegistration.h"

namespace ARTKBlender
{

/// python type structure for ARHandle
PyTypeObject ARPixelFormatType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "ARTKBlender.ARPixelFormat",  /* tp_name */
  sizeof(PyARPixelFormat),   /* tp_basicsize */
  0,                         /* tp_itemsize */
  0,                         /* tp_dealloc */
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
  "ARPixelFormat enumeration", /* tp_doc */
};

// staticka mapa enumeracie
PyTypeRegistrationEnum::EnumMap pixelFormatMap =
  { { "INVALID", -1 },{ "RGB", 0 },{ "BGR", 1 },{ "RGBA", 2 },
  { "BGRA", 3 },{ "ABGR", 4 },{ "MONO", 5 },{ "ARGB", 6 },
  { "UYVY", 7 },{ "YUY2", 8 },{ "RGB_565", 9 },{ "RGBA_5551", 10 },
  { "RGBA_4444", 11 },{ "_420v", 12 },{ "_420f", 13 },{ "NV21", 14 } };

// registration object
static PyTypeRegistrationEnum ARPixelFormatReg("ARPixelFormat", ARPixelFormatType, pixelFormatMap);

}