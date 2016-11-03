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

#include "CppUnitTest.h"

#include "BlenderUtils.h"
#include "PyTypeRegistration.h"
#include "PyObjectHelper.h"
#include "../Blender/bgl.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

// test bgl.Buffer registration data
PyTypeObject BGL_bufferType =
{
  PyVarObject_HEAD_INIT(NULL, 0)
  "bgl.Buffer",              /* tp_name */
  sizeof(Buffer),            /* tp_basicsize */
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
  "Test implementation of bgl.Buffer",      /* tp_doc */
};


// test buffer content
char testData[] = "| first  | second | third  | fourth";

// function to create python buffer
PyObject * createPythonBuffer()
{
  return PyBytes_FromStringAndSize(testData, sizeof(testData));
}

// function to create blender buffer
PyObject * createBlenderBuffer()
{
  ARTKBlender::PyTypeRegistration pyType("Buffer", BGL_bufferType);
  Assert::IsTrue(ARTKBlender::PyTypeRegistration::getAllReady());
  Buffer * src = PyObject_New(Buffer, &BGL_bufferType);
  src->buf.asbyte = testData;
  src->ndimensions = 1;
  static int dimension = sizeof(testData);
  src->dimensions = &dimension;
  return (PyObject*)src;
}


// test class for ImageBufferHolder
TEST_CLASS(ImageBufferHolderTests)
{
public:
  TEST_METHOD(ImageBufferHolder_SourceOwner)
  {
    PyObject * src = PyBytes_FromString(testData);
    Assert::IsTrue(src->ob_refcnt == 1);
    {
      ARTKBlender::ImageBufferHolder testHolder(src);
      Assert::Assert::IsTrue(src->ob_refcnt == 2);
    }
    Assert::IsTrue(src->ob_refcnt == 1);
    Py_DECREF(src);
  }

  TEST_METHOD(PythonBufferHolder_SourceOwner)
  {
    PyObject * src = createPythonBuffer();
    Assert::IsTrue(ARTKBlender::PythonBufferHolder::isSuitable(src));
    ARTKBlender::PythonBufferHolder testHolder(src);
    Assert::AreEqual(testHolder.getSize(), sizeof(testData));
    Assert::IsTrue(testHolder.isValid(0));
    Assert::IsTrue(testHolder.isValid(sizeof(testData)));
    Assert::IsFalse(testHolder.isValid(sizeof(testData) + 2));
    for (size_t i = 0; i < sizeof(testData); ++i)
      Assert::IsTrue(testHolder.getData()[i] == testData[i]);
    Py_DECREF(src);
  }

  TEST_METHOD(BlenderBufferHolder_SourceOwner)
  {
    PyObject * src = createBlenderBuffer();
    Assert::IsTrue(ARTKBlender::BlenderBufferHolder::isSuitable(src));
    ARTKBlender::BlenderBufferHolder testHolder(src);
    Assert::AreEqual(testHolder.getSize(), sizeof(testData));
    Assert::IsTrue(testHolder.isValid(0));
    Assert::IsTrue(testHolder.isValid(sizeof(testData)));
    Assert::IsFalse(testHolder.isValid(sizeof(testData) + 2));
    for (size_t i = 0; i < sizeof(testData); ++i)
      Assert::IsTrue(testHolder.getData()[i] == testData[i]);
    Py_DECREF(src);
  }

  TEST_METHOD(PythonBufferHolder_getImageHolder)
  {
    ARTKBlender::PyObjectOwner src (createPythonBuffer());
    auto holder = ARTKBlender::getBufferHolder(src.get());
    Assert::IsNotNull(dynamic_cast<ARTKBlender::PythonBufferHolder*>(holder.get()));
    Assert::IsNull(dynamic_cast<ARTKBlender::BlenderBufferHolder*>(holder.get()));
  }

  TEST_METHOD(BlenderBufferHolder_getImageHolder)
  {
    ARTKBlender::PyObjectOwner src(createBlenderBuffer());
    auto holder = ARTKBlender::getBufferHolder(src.get());
    Assert::IsNull(dynamic_cast<ARTKBlender::PythonBufferHolder*>(holder.get()));
    Assert::IsNotNull(dynamic_cast<ARTKBlender::BlenderBufferHolder*>(holder.get()));
  }

  TEST_METHOD(NoBufferHolder_getImageHolder)
  {
    ARTKBlender::PyObjectOwner src(PyUnicode_FromString(testData));
    auto holder = ARTKBlender::getBufferHolder(src.get());
    Assert::IsNull(holder.get());
  }
};


}