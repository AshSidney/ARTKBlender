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

#include "PyTypeRegistration.h"

namespace UnitTests
{

/**
    Mock class for PyTypeRegistration allows to test its static functions.
*/
class MockPyTypeRegistration : public ARTKBlender::PyTypeRegistration
{
public:
  /**
      Constructor for mock object.
      \param name name of python type
  */
  MockPyTypeRegistration(const char * name, bool ready = true)
    : PyTypeRegistration(name, testPyType), isReady(ready), callFlags(0)
  {}

  /**
      Method to prepare type. In mock class is empty.
      \return true, if this type was successfully prepared
  */
  virtual bool getReady(void)
  {
    ++callFlags;
    return isReady;
  }

  /**
      Method to add type to module. In mock class is empty.
      \module module object
  */
  virtual void addType(PyObject * module)
  {
    callFlags += 2;
  }

  /**
      Method to get next type in list.
      \return next type in list or nullptr, if this is the last one
  */
  MockPyTypeRegistration * next(void)
  {
    return static_cast<MockPyTypeRegistration*>(nextType);
  }

  /**
      Provide call flags.
      \return flags value
  */
  short getCallFlags(void)
  {
    return callFlags;
  }

  /**
      Provide the first item in list of registration objects.
      \return pointer to the first object
  */
  static PyTypeRegistration *& getFirstType()
  {
    return firstType;
  }

protected:
  /// testing python type data
  PyTypeObject testPyType;
  /// flag to be returned by getReady() method
  bool isReady;
  /// flags indicating getReady() and addType() methods were called
  short callFlags;
};

}