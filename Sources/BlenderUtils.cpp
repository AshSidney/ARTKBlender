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

#include "BlenderUtils.h"

#include <cstring>
#include "Blender/bgl.h"

namespace ARTKBlender
{

// implementation of image data access methods

ImageBufferHolder::ImageBufferHolder (PyObject * source)
  : sourceObj(source, true), data(nullptr), dataSize(0)
{}

ImageBufferHolder::~ImageBufferHolder (void)
{}


BlenderBufferHolder::BlenderBufferHolder (PyObject * source)
  : ImageBufferHolder(source)
{
  Buffer * buffer = getPyType<Buffer>(sourceObj.get());
  data = reinterpret_cast<ARUint8>(buffer->buf.asbyte);
  //TODO dataSize = 1;
}

BlenderBufferHolder::~BlenderBufferHolder (void)
{}

PyTypeObject * BlenderBufferHolder::bglBufferType = nullptr;

bool BlenderBufferHolder::isSuitable (PyObject * source)
{
  // try to initialize pointer to bgl.Buffer type
  if (bglBufferType == nullptr)
  {
    if (strcmp(obj->ob_type->tp_name, "bgl.Buffer") == 0)
      bglBufferType = obj->ob_type;
    else
      return false;
  }
  // check if type is correct
  return obj->ob_type == bglBufferType;
}


PythonBufferHolder::PythonBufferHolder (PyObject * source)
  : ImageBufferObject(source)
{
  // get buffer from object
  if (PyObject_GetBuffer(sourceObj.get(), &pyBuffer, PyBUF_SIMPLE) == 0)
  {
    data = reinterpret_cast<ARUint8*>(pyBuffer.buf);
    dataSize = pyBuffer.length;
  }
}

PythonBufferHolder::~PythonBufferHolder (void)
{
  PyBuffer_Release(pyBuffer);
}

bool PythonBufferHolder::isSuitable (PyObject * source)
{
  return PyObject_CheckBuffer(source) == 1;
}


std::unique_ptr<ImageBufferHolder> getBufferHolder (PyObject * source)
{
  // check if it's blender buffer
  if (BlenderBufferHolder::isSuitable(source)
    return std::unique_ptr<ImageBufferHolder>(new BlenderBufferHolder(source));
  // check if it's python buffer
  if (PythonBufferHolder::isSuitable(source)
    return std::unique_ptr<ImageBufferHolder>(new PythonBufferHolder(source));
  // otherwise return empty pointer
  return std::unique_ptr<ImageBufferHolder>();
}

}
