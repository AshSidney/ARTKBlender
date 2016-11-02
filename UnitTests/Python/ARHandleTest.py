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


def test_ARHandleConstruct ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return 'Parameters load failed'
  handle = ARTKBlender.ARHandle(param, ARTKBlender.ARPixelFormat.RGB)
  if not isinstance(handle, ARTKBlender.ARHandle):
    return 'handle isn\'t instance of ARHandle'
  return '' if handle.pixelFormat == ARTKBlender.ARPixelFormat.RGB else 'improper pixel format'

def test_ARHandleConstructOtherPixelFormat ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return 'Parameters load failed'
  handle = ARTKBlender.ARHandle(param, ARTKBlender.ARPixelFormat.MONO)
  if not isinstance(handle, ARTKBlender.ARHandle):
    return 'handle isn\'t instance of ARHandle'
  return '' if handle.pixelFormat == ARTKBlender.ARPixelFormat.MONO else 'improper pixel format'

def test_ARHandleAttachPattern ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return 'Parameters load failed'
  handle = ARTKBlender.ARHandle(param, ARTKBlender.ARPixelFormat.RGB)
  if handle.attachPatt is not None:
    return 'No pattern should be attached'
  pattHandle = ARTKBlender.ARPattHandle()
  if pattHandle.load('../../UnitTests/Data/hiro.patt') != 0:
    return 'Invalid pattern ID'
  handle.attachPatt = pattHandle
  if handle.attachPatt != pattHandle:
    return 'Invalid pattern attached'
  del handle.attachPatt
  return '' if handle.attachPatt is None else 'No pattern should be attached'

def loadImage (imgName, imgSize, pixSize):
  imgFile = open(imgName, 'rb')
  image = imgFile.read()
  imgFile.close()
  if len(image) != imgSize[0] * imgSize[1] * pixSize:
    return 'Image data have invalid size = ' + str(len(image))
  return image

def detectMarker (handle, image):
  if not handle.detect(image):
    return 'Marker detection failed'
  if len(handle.markers) != 1:
    return 'One marker should be available'
  if handle.markers[0].id != 0:
    return 'Invalid detected pattern ID'
  if handle.markers[0].cf < 0.6:
    return 'Detected pattern confidence too low'
  return ''

def performMarkerDetection ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return 'Parameters load failed'
  param.size = (254, 207)
  image = loadImage('../../UnitTests/Data/hiro_marker.raw', param.size, 3)
  if isinstance(image, str):
    return image
  handle = ARTKBlender.ARHandle(param, ARTKBlender.ARPixelFormat.RGB)
  handle.attachPatt = ARTKBlender.ARPattHandle()
  pattID = handle.attachPatt.load('../../UnitTests/Data/hiro.patt')
  if pattID != 0:
    return 'Invalid pattern ID'
  if len(handle.markers) != 0:
    return 'No markers should be available'
  rslt = detectMarker(handle, image)
  if rslt != '':
    return rslt
  return (handle, param)

def test_ARHandleDetect ():
  rslt = performMarkerDetection()
  if isinstance(rslt, str):
    return rslt
  return ''

def test_ARHandleDetect_Camera ():
  param = ARTKBlender.ARParam()
  if not param.load('../../UnitTests/Data/camera_para.dat'):
    return 'Parameters load failed'
  param.size = (640, 480)
  image = loadImage('../../UnitTests/Data/test_image.raw', param.size, 4)
  if isinstance(image, str):
    return image
  handle = ARTKBlender.ARHandle(param, ARTKBlender.ARPixelFormat.RGBA)
  handle.attachPatt = ARTKBlender.ARPattHandle()
  if not handle.detect(image):
    return 'First detection failed'
  pattID = handle.attachPatt.load('../../UnitTests/Data/4x4_42.patt')
  if pattID != 0:
    return 'Invalid pattern ID'
  if not handle.detect(image):
    return 'Second detection failed'
  return '' if len(handle.markers) > 0 else 'No marker detected'