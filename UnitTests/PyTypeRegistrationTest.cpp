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

#include "PyTypeRegistration.h"
#include "PyObjectHelper.h"

#include "MockPyTypeRegistration.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

// test class for PyTypeRegistration with mock class
TEST_CLASS(PyTypeRegistrationTests)
{
public:
  TEST_METHOD_CLEANUP(ClearTypes)
  {
      MockPyTypeRegistration::getFirstType() = nullptr;
  }

  TEST_METHOD(CreateObjects)
  {
    Assert::IsNull(MockPyTypeRegistration::getFirstType());

    MockPyTypeRegistration pyType1("TypeName1");

    Assert::AreEqual(pyType1.getCallFlags(), short(0));

    Assert::IsTrue(MockPyTypeRegistration::getFirstType() == &pyType1);
    Assert::IsNull(pyType1.next());

    MockPyTypeRegistration pyType2("TypeName2");

    Assert::AreEqual(pyType2.getCallFlags(), short(0));

    Assert::IsTrue(MockPyTypeRegistration::getFirstType() == &pyType2);
    Assert::IsTrue(pyType2.next() == &pyType1);
    Assert::IsNull(pyType1.next());
  }

  TEST_METHOD(Run_GetAllReady)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2");
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsTrue(MockPyTypeRegistration::getAllReady());

    Assert::AreEqual(pyType1.getCallFlags(), short(1));
    Assert::AreEqual(pyType2.getCallFlags(), short(1));
    Assert::AreEqual(pyType3.getCallFlags(), short(1));
  }

  TEST_METHOD(Run_GetAllReady_Fail)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2", false);
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsFalse(MockPyTypeRegistration::getAllReady());

    Assert::AreEqual(pyType1.getCallFlags(), short(0));
    Assert::AreEqual(pyType2.getCallFlags(), short(1));
    Assert::AreEqual(pyType3.getCallFlags(), short(1));
  }

  TEST_METHOD(Run_AddType)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2");
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsTrue(MockPyTypeRegistration::getAllReady());
    MockPyTypeRegistration::addAllTypes(nullptr);

    Assert::AreEqual(pyType1.getCallFlags(), short(3));
    Assert::AreEqual(pyType2.getCallFlags(), short(3));
    Assert::AreEqual(pyType3.getCallFlags(), short(3));

    Assert::IsNull(MockPyTypeRegistration::getFirstType());
  }
};


// test class for PyTypeRegistration using Python
TEST_CLASS(PyTypeRegistrationPythonTests)
{
public:

  TEST_METHOD(RegisterType2Python)
  {
    /// test python data structure
    struct TestPythonObject
    {
      PyObject_HEAD
        /// test data
        size_t testData;
    };

    /// python type structure for ARParam
    PyTypeObject TestPythonType = {
      PyVarObject_HEAD_INIT(NULL, 0)
      "artk.TestPythonType",     /* tp_name */
      sizeof(TestPythonObject),  /* tp_basicsize */
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
      "TestPython objects",      /* tp_doc */
    };

    // module definition
    PyModuleDef TestPyModule =
    {
      PyModuleDef_HEAD_INIT,
      "test",
      "Test Python module",
      -1
    };

    ARTKBlender::PyTypeRegistration pyType("TestPyType", TestPythonType);

    Assert::IsTrue(ARTKBlender::PyTypeRegistration::getAllReady());

    ARTKBlender::PyObjectOwner testMod (PyModule_Create(&TestPyModule));

    ARTKBlender::PyTypeRegistration::addAllTypes(testMod.get());

    PyObject * modDict = PyModule_GetDict(testMod.get());
    PyObject * modType = PyDict_GetItemString(modDict, "TestPyType");

    Assert::IsTrue(PyType_CheckExact(modType));

    Assert::IsTrue(ARTKBlender::getPyType<PyTypeObject>(modType) == &TestPythonType);
  }

  TEST_METHOD(RegisterTypeEnum2Python)
  {
    /// test python data structure
    struct TestPythonObject
    {
      PyObject_HEAD
        /// test data
        size_t testData;
    };

    /// python type structure for ARParam
    PyTypeObject TestPythonType = {
      PyVarObject_HEAD_INIT(NULL, 0)
      "artk.TestPythonType",     /* tp_name */
      sizeof(TestPythonObject),  /* tp_basicsize */
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
      "TestPython objects",      /* tp_doc */
    };

    // module definition
    PyModuleDef TestPyModule =
    {
      PyModuleDef_HEAD_INIT,
      "test",
      "Test Python module",
      -1
    };

    ARTKBlender::PyTypeRegistrationEnum::EnumMap testMap = { {"abc", 2}, {"cde", 5} };
    ARTKBlender::PyTypeRegistrationEnum pyType("TestPyTypeEnum", TestPythonType, testMap);

    Assert::IsTrue(ARTKBlender::PyTypeRegistration::getAllReady());

    PyObject * testMod = PyModule_Create(&TestPyModule);

    ARTKBlender::PyTypeRegistration::addAllTypes(testMod);

    PyObject * modDict = PyModule_GetDict(testMod);
    PyObject * modType = PyDict_GetItemString(modDict, "TestPyTypeEnum");

    Assert::IsTrue(PyType_CheckExact(modType));

    Assert::IsTrue(ARTKBlender::getPyType<PyTypeObject>(modType) == &TestPythonType);

    Assert::AreEqual(PyLong_AsLong(PyDict_GetItemString(TestPythonType.tp_dict, "cde")), long(5));
    Assert::AreEqual(PyLong_AsLong(PyDict_GetItemString(TestPythonType.tp_dict, "abc")), long(2));
  }
};

}