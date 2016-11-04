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
#include <AR/ar.h>
#include <memory>

#include "PyObjectHelper.h"

namespace ARTKBlender
{

/**
    Base class to hold python buffer containing image data.
*/
class ImageBufferHolder
{
public:
  /**
      Constructor retrieves buffer from source data.
      /param source source python object
  */
  ImageBufferHolder (PyObject * source);

  /**
      Destructor releases allocated data.
  */
  virtual ~ImageBufferHolder (void);

  /**
      Provides pointer to buffer.
      \return pointer to image in buffer
  */
  ARUint8 * getData (void) const
  {
    return data;
  }

  /**
      Provides size of buffer.
      \return size of buffer in bytes
  */
  size_t getSize (void) const
  {
      return dataSize;
  }

  /**
      Validates buffer and its size.
      \param required data size, if 0, size is not checked.
      \return true, if buffer is valid and has required size.
  */
  bool isValid (size_t reqSize) const
  {
    return data != nullptr && (dataSize == reqSize || reqSize == 0);
  }
  
protected:
  /// source object containing buffer
  PyObjectOwner sourceObj;
  /// pointer to image data
  ARUint8 * data;
  /// size of image data
  size_t dataSize;
};


/**
    Class holding image buffer from Blender's bgl.Buffer.
*/
class BlenderBufferHolder : public ImageBufferHolder
{
public:
  /**
      Constructor retrieves buffer from source data.
      /param source source python object
  */
  BlenderBufferHolder (PyObject * source);

  /**
      Destructor releases allocated data.
  */
  virtual ~BlenderBufferHolder (void);

  /**
      Function to check if source is Blender's blg.Buffer.
  */
  static bool isSuitable (PyObject * source);

protected:
  /// pointer to bgl.Buffer type
  static PyTypeObject * bglBufferType;
};

    
/**
    Class holding python's buffer.
*/
class PythonBufferHolder : public ImageBufferHolder
{
public:
  /**
      Constructor retrieves buffer from source data.
      /param source source python object
  */
  PythonBufferHolder (PyObject * source);

  /**
      Destructor releases allocated data.
  */
  virtual ~PythonBufferHolder (void);

  /**
      Function to check if source is python buffer.
  */
  static bool isSuitable (PyObject * source);

protected:
  /// python buffer structure
  Py_buffer pyBuffer;
};


/**
    Function to create appropriate type of buffer holder
    \param source source python object
    \return pointer to image holder
*/
std::unique_ptr<ImageBufferHolder> getBufferHolder (PyObject * source);

}
