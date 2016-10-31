# -----------------------------------------------------------------------------
# This source file is part of ARTKBlender library
#
# Copyright (c) 2016 The Zdeno Ash Miklas
#
# ARTKBlender is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Foobar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with ARTKBlender.  If not, see <http://www.gnu.org/licenses/>.
# -----------------------------------------------------------------------------

import ARTKBlender
import ARHandleTest


def test_AR3DHandleConstruct ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return False
  handle = ARTKBlender.AR3DHandle(param)
  return '' if isinstance(handle, ARTKBlender.AR3DHandle) else 'handle isn\'t instance of AR3DHandle'

def checkMatrix (mat, vecs, tVec):
  if len(mat) != 4 or len(mat[0]) != 4 or len(mat[1]) != 4 or len(mat[2]) != 4:
    return 'Matrix isn\'t 4x4'
  if mat[3][0] != 0.0 or mat[3][1] != 0.0 or mat[3][2] != 0.0 or mat[3][3] != 1.0:
    return 'Invalid last row of matrix'
  for i in range(3):
    for j in range(3):
      if abs(vecs[i][j] - mat[j][i]) > 0.1:
        return 'Invalid matrix value in [' + str(j) + '][' + str(i) + ']'
  for j in range(3):
    if abs(1.0 - mat[j][3] / tVec[j]) > 0.1:
      return 'Invalid matrix value in [' + str(j) + '][3]'
  return ''

def test_AR3DHandleCalcMatrix ():
  rslt = ARHandleTest.performMarkerDetection()
  if isinstance(rslt, str):
    return rslt
  handle = rslt[0]
  handle3D = ARTKBlender.AR3DHandle(rslt[1])
  mat = handle3D.getTransMatSquare(handle.markers[0], 100.0)
  return checkMatrix(mat, ((0.9, -0.3, 0.3),(0.4, 0.7, -0.6),(0.0, 0.65, 0.75)), (7.0, 13.0, -270.0))

def test_AR3DHandleCalcMatrixCont ():
  rslt = ARHandleTest.performMarkerDetection()
  if isinstance(rslt, str):
    return rslt
  handle = rslt[0]
  param = rslt[1]
  handle3D = ARTKBlender.AR3DHandle(param)
  mat = handle3D.getTransMatSquare(handle.markers[0], 100.0)
  vecs = ((0.9, -0.3, 0.3),(0.4, 0.7, -0.6),(0.0, 0.65, 0.75))
  rslt = checkMatrix(mat, vecs, (7.0, 13.0, -270.0))
  if rslt != '':
    return rslt
  image = ARHandleTest.loadImage('../../UnitTests/Data/hiro_marker2.raw', param.size)
  if isinstance(image, str):
    return image
  rslt = ARHandleTest.detectMarker(handle, image)
  if rslt != '':
    return rslt
  mat = handle3D.getTransMatSquareCont(handle.markers[0], 100.0, mat)
  return checkMatrix(mat, vecs, (-10.0, 30.0, -270.0))
