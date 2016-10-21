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

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

/**
*   Mock class for PyTypeRegistration allows to test its static functions.
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
  virtual bool GetReady()
  {
    ++callFlags;
    return isReady;
  }

  /**
      Method to add type to module. In mock class is empty.
      \module module object
  */
  virtual void AddType(PyObject * module)
  {
    callFlags += 2;
  }

  /**
      Provide call flags.
      \return flags value
  */
  short GetCallFlags()
  {
    return callFlags;
  }

  /**
      Provide list of registration objects.
      \return vector of objects
  */
  static std::vector<PyTypeRegistration*> & GetTypes()
  {
    return types;
  }

protected:
  /// testing python type data
  PyTypeObject testPyType;
  /// flag to be returned by GetReady() method
  bool isReady;
  /// flags indicating GetReady() and AddType() methods were called
  short callFlags;
};


// test class for PyTypeRegistration with mock class
TEST_CLASS(PyTypeRegistrationTests)
{
public:
  TEST_METHOD_CLEANUP(ClearTypes)
  {
    MockPyTypeRegistration::GetTypes().clear();
  }

  TEST_METHOD(CreateObjects)
  {
    Assert::IsTrue(MockPyTypeRegistration::GetTypes().empty());

    MockPyTypeRegistration pyType1("TypeName1");

    Assert::AreEqual(pyType1.GetCallFlags(), short(0));

    Assert::IsFalse(MockPyTypeRegistration::GetTypes().empty());
    Assert::AreEqual(MockPyTypeRegistration::GetTypes().size(), size_t(1));
    Assert::IsTrue(MockPyTypeRegistration::GetTypes()[0] == &pyType1);

    MockPyTypeRegistration pyType2("TypeName2");

    Assert::AreEqual(pyType1.GetCallFlags(), short(0));

    Assert::AreEqual(MockPyTypeRegistration::GetTypes().size(), size_t(2));
    Assert::IsTrue(MockPyTypeRegistration::GetTypes()[0] == &pyType1);
    Assert::IsTrue(MockPyTypeRegistration::GetTypes()[1] == &pyType2);
  }

  TEST_METHOD(Run_GetAllReady)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2");
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsTrue(MockPyTypeRegistration::GetAllReady());

    Assert::AreEqual(pyType1.GetCallFlags(), short(1));
    Assert::AreEqual(pyType2.GetCallFlags(), short(1));
    Assert::AreEqual(pyType3.GetCallFlags(), short(1));
  }

  TEST_METHOD(Run_GetAllReady_Fail)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2", false);
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsFalse(MockPyTypeRegistration::GetAllReady());

    Assert::AreEqual(pyType1.GetCallFlags(), short(1));
    Assert::AreEqual(pyType2.GetCallFlags(), short(1));
    Assert::AreEqual(pyType3.GetCallFlags(), short(0));
  }

  TEST_METHOD(Run_AddType)
  {
    MockPyTypeRegistration pyType1("TypeName1");
    MockPyTypeRegistration pyType2("TypeName2");
    MockPyTypeRegistration pyType3("TypeName3");

    Assert::IsTrue(MockPyTypeRegistration::GetAllReady());
    MockPyTypeRegistration::AddAllTypes(nullptr);

    Assert::AreEqual(pyType1.GetCallFlags(), short(3));
    Assert::AreEqual(pyType2.GetCallFlags(), short(3));
    Assert::AreEqual(pyType3.GetCallFlags(), short(3));
  }
};

/// test python data structure
struct TestPythonObject
{
  PyObject_HEAD
  /// test data
  size_t testData;
};

/// python type structure for ARParam
static PyTypeObject TestPythonType = {
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
static PyModuleDef TestPyModule =
{
  PyModuleDef_HEAD_INIT,
  "test",
  "Test Python module",
  -1
};


// test class for PyTypeRegistration using Python
TEST_CLASS(PyTypeRegistrationPythonTests)
{
public:
  TEST_METHOD_INITIALIZE(InitTest)
  {
    Py_Initialize();
  }

  TEST_METHOD_CLEANUP(ClearTest)
  {
    MockPyTypeRegistration::GetTypes().clear();
    Py_Finalize();
  }

  TEST_METHOD(RegisterType2Python)
  {
    ARTKBlender::PyTypeRegistration pyType("TestPyType", TestPythonType);

    Assert::IsTrue(ARTKBlender::PyTypeRegistration::GetAllReady());

    PyObject * testMod = PyModule_Create(&TestPyModule);

    ARTKBlender::PyTypeRegistration::AddAllTypes(testMod);

    PyObject * modDict = PyModule_GetDict(testMod);
    PyObject * modType = PyDict_GetItemString(modDict, "TestPyType");

    Assert::IsTrue(PyType_CheckExact(modType));
  }
};

}