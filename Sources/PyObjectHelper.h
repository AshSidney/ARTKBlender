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

namespace ARTKBlender
{

/**
    Template functions to assist Python type implementation.
*/

/**
    Convert python object pointer to pointer to type structure.
    \param obj pointer to python object
    \return pointer to type structure
*/ 
template <class PyType> PyType * getPyType(PyObject * obj)
{
  return reinterpret_cast<PyType*>(obj);
}

/**
    Convert pointer to type structure to python object pointer.
    \param obj pointer to type structure
    \return pointer to type structure
*/ 
template <class PyType> PyObject * getPyObject(PyType * obj)
{
  return reinterpret_cast<PyObject*>(obj);
}

/**
    Deallocate pointer to type structure.
    \param obj pointer to type structure
    \param pointer to type structure
*/ 
template <class PyType> void deallocPyObject(PyType * obj)
{
  Py_TYPE(obj)->tp_free(getPyObject(obj));
}

/**
    Check python object pointer is of required type.
    \param obj pointer to python object
    \param pyTypeDef reference to python type definition
    \return true if object is of required type
*/
template <class PyType> bool isInstance(PyObject * obj, PyType & pyTypeDef)
{
  return PyObject_IsInstance(obj, (PyObject *)&pyTypeDef) == 1;
}


/**
    Class to hold ownership of python object and release it when destructed.
*/
class PyObjectOwner
{
public:
  /**
      Constructor is initialized by pointer to python object.
      \param obj    python object
      \param incRef increase reference count of python object
  */
  PyObjectOwner (PyObject * obj = nullptr, bool incRef = false) : pyObject(obj)
  {
    if (incRef && pyObject != nullptr)
      Py_INCREF(pyObject);
  }

  /**
      Destructor releases python object.
  */
  ~PyObjectOwner (void)
  {
    if (pyObject != nullptr)
      Py_DECREF(pyObject);
  }

  /**
      Copy constructor.
      \param obj    source object
  */
  PyObjectOwner(const PyObjectOwner & obj) : pyObject(obj.pyObject)
  {
    if (pyObject != nullptr)
      Py_INCREF(pyObject);
  }

  /**
      Move constructor.
      \param obj    source object
  */
  PyObjectOwner(PyObjectOwner && obj) : pyObject(obj.pyObject)
  {
    obj.pyObject = nullptr;
  }

  /**
      Assignment operator.
      \param obj source object
      \return    reference to this object
  */
  PyObjectOwner & operator= (const PyObjectOwner & obj)
  {
    if (pyObject != nullptr)
      Py_DECREF(pyObject);
    pyObject = obj.pyObject;
    if (pyObject != nullptr)
      Py_INCREF(pyObject);
    return *this;
  }

  /**
      Provide access to python object.
      \return reference to pointer to python object
  */
  PyObject *& get (void)
  {
    return pyObject;
  }

  /**
     Checks if python object is available.
     \return true, if object is null
  */
  bool isNull (void) const
  {
    return pyObject == nullptr;
  }

  /**
     Prepare python object for returning from function (increase its ref count).
     \return pointer to python object, if null then None object is returned
  */
  PyObject * returnValue (void) const
  {
    if (pyObject == nullptr)
      Py_RETURN_NONE;
    Py_INCREF(pyObject);
    return pyObject;
  }

protected:
  /// owned python object
  PyObject * pyObject;
};

}