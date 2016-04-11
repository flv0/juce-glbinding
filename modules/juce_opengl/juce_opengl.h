/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#ifndef JUCE_OPENGL_H_INCLUDED
#define JUCE_OPENGL_H_INCLUDED

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

#include "../juce_gui_extra/juce_gui_extra.h"

#define JUCE_OPENGL  1
//#define JUCE_OPENGL3 1

//=============================================================================
/** This macro is a helper for use in GLSL shader code which needs to compile on both OpenGL 2.1 and OpenGL 3.0.
    It's mandatory in OpenGL 3.0 to specify the GLSL version.
*/
#if JUCE_OPENGL3
 #if JUCE_OPENGL_ES
  #define JUCE_GLSL_VERSION "#version 300 es"
 #else
  #define JUCE_GLSL_VERSION "#version 150"
 #endif
#else
 #define JUCE_GLSL_VERSION ""
#endif

//=============================================================================
#if JUCE_OPENGL_ES || defined (DOXYGEN)
 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define JUCE_MEDIUMP  "mediump"

 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define JUCE_HIGHP    "highp"

 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define JUCE_LOWP     "lowp"
#else
 #define JUCE_MEDIUMP
 #define JUCE_HIGHP
 #define JUCE_LOWP
#endif

//=============================================================================
namespace juce
{

class OpenGLTexture;
class OpenGLFrameBuffer;
class OpenGLShaderProgram;

#include "geometry/juce_Quaternion.h"
#include "geometry/juce_Matrix3D.h"
#include "geometry/juce_Vector3D.h"
#include "geometry/juce_Draggable3DOrientation.h"
#include "opengl/juce_OpenGLHelpers.h"
#include "opengl/juce_OpenGLPixelFormat.h"
#include "opengl/juce_OpenGLRenderer.h"
#include "opengl/juce_OpenGLContext.h"
#include "opengl/juce_OpenGLFrameBuffer.h"
#include "opengl/juce_OpenGLGraphicsContext.h"
#include "opengl/juce_OpenGLHelpers.h"
#include "opengl/juce_OpenGLImage.h"
#include "opengl/juce_OpenGLRenderer.h"
#include "opengl/juce_OpenGLShaderProgram.h"
#include "opengl/juce_OpenGLTexture.h"
#include "utils/juce_OpenGLAppComponent.h"

}

#endif   // JUCE_OPENGL_H_INCLUDED
