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

#include "PyTestHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

// test class for PyTypeRegistration using Python
TEST_CLASS(PyARParamPythonTests)
{
public:

  TEST_METHOD(PythonTest)
  {
    PyTestHelper pyTest;
    auto rslt = pyTest.RunTests("ARParamTest");
    Assert::AreEqual(rslt.size(), size_t(0));
  }
};

}
