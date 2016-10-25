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
    \return pointer to type structure
*/ 
template <class PyType> PyType * getPyType(PyObject * obj)
{
  return reinterpret_cast<PyType*>(obj);
}

/**
    Convert pointer to type structure to python object pointer.
    \return pointer to type structure
*/ 
template <class PyType> PyObject * getPyObject(PyType * obj)
{
  return reinterpret_cast<PyObject*>(obj);
}

/**
    Deallocate pointer to type structure.
    \param pointer to type structure
*/ 
template <class PyType> void deallocPyObject(PyType * obj)
{
  Py_TYPE(obj)->tp_free(getPyObject(obj));
}

}