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
#include <AR/ar.h>
#include "PyObjectHelper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTests
{

// test class for PyARParam type using Python
TEST_CLASS(PyARParamPythonTests)
{
public:

  TEST_METHOD(PythonTest)
  {
    AssertPythonModule("ARParamTest");
  }

  TEST_METHOD(LoadDataTest)
  {
    // load test data
    ARParam testParam;
    const char dataFile[] = "../../UnitTests/Data/camera_para.dat";
    Assert::AreEqual(arParamLoad(dataFile, 1, &testParam), 0);
    // fill data to arguments
    Assert::AreEqual(AR_DIST_FACTOR_NUM_MAX, 9);
    ARTKBlender::PyObjectOwner args(Py_BuildValue("(s(ii)((dddd)(dddd)(dddd))(ddddddddd))",
      dataFile, testParam.xsize, testParam.ysize,
      testParam.mat[0][0], testParam.mat[0][1], testParam.mat[0][2], testParam.mat[0][3],
      testParam.mat[1][0], testParam.mat[1][1], testParam.mat[1][2], testParam.mat[1][3],
      testParam.mat[2][0], testParam.mat[2][1], testParam.mat[2][2], testParam.mat[2][3],
      testParam.dist_factor[0], testParam.dist_factor[1], testParam.dist_factor[2],
      testParam.dist_factor[3], testParam.dist_factor[4], testParam.dist_factor[5],
      testParam.dist_factor[6], testParam.dist_factor[7], testParam.dist_factor[8]));
    // check these data with data loaded in python script
    AssertPythonFunction("ARParamTest", "testARParamLoadData", args.get());
  }
};

}
