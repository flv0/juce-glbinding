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

OpenGLShaderProgram::OpenGLShaderProgram (const OpenGLContext& c) noexcept
    : context (c), programID (0)
{
}

OpenGLShaderProgram::~OpenGLShaderProgram() noexcept
{
    release();
}

GLuint OpenGLShaderProgram::getProgramID() const noexcept
{
    // This method can only be used when the current thread has an active OpenGL context.
    jassert (OpenGLHelpers::isContextActive());

    if (programID == 0)
        programID = glCreateProgram();

    return programID;
}

void OpenGLShaderProgram::release() noexcept
{
    if (programID != 0)
    {
        glDeleteProgram (programID);
        programID = 0;
    }
}

double OpenGLShaderProgram::getLanguageVersion()
{
    return String::fromUTF8 ((const char*) glGetString (GL_SHADING_LANGUAGE_VERSION))
            .retainCharacters("1234567890.").getDoubleValue();
}

bool OpenGLShaderProgram::addShader (const String& code, GLenum type)
{
    GLuint shaderID = glCreateShader (type);

    const GLchar* c = code.toRawUTF8();
    glShaderSource (shaderID, 1, &c, nullptr);

    glCompileShader (shaderID);

    GLint status = 0;
    glGetShaderiv (shaderID, GL_COMPILE_STATUS, &status);

    if (status == 0)
    {
        GLchar infoLog [16384];
        GLsizei infoLogLength = 0;
        glGetShaderInfoLog (shaderID, sizeof (infoLog), &infoLogLength, infoLog);
        errorLog = String (infoLog, (size_t) infoLogLength);

       #if JUCE_DEBUG && ! JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR
        // Your GLSL code contained compile errors!
        // Hopefully this compile log should help to explain what went wrong.
        DBG (errorLog);
        jassertfalse;
       #endif

        return false;
    }

    glAttachShader (getProgramID(), shaderID);
    glDeleteShader (shaderID);
    JUCE_CHECK_OPENGL_ERROR
    return true;
}

bool OpenGLShaderProgram::addVertexShader (const String& code)    { return addShader (code, GL_VERTEX_SHADER); }
bool OpenGLShaderProgram::addFragmentShader (const String& code)  { return addShader (code, GL_FRAGMENT_SHADER); }

bool OpenGLShaderProgram::link() noexcept
{
    // This method can only be used when the current thread has an active OpenGL context.
    jassert (OpenGLHelpers::isContextActive());

    GLuint progID = getProgramID();

    glLinkProgram (progID);

    GLint status = 0;
    glGetProgramiv (progID, GL_LINK_STATUS, &status);

    if (status == 0)
    {
        GLchar infoLog [16384];
        GLsizei infoLogLength = 0;
        glGetProgramInfoLog (progID, sizeof (infoLog), &infoLogLength, infoLog);
        errorLog = String (infoLog, (size_t) infoLogLength);

       #if JUCE_DEBUG && ! JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR
        // Your GLSL code contained link errors!
        // Hopefully this compile log should help to explain what went wrong.
        DBG (errorLog);
        jassertfalse;
       #endif
    }

    JUCE_CHECK_OPENGL_ERROR
    return status != 0;
}

void OpenGLShaderProgram::use() const noexcept
{
    // The shader program must have been successfully linked when this method is called!
    jassert (programID != 0);

    glUseProgram (programID);
}

GLint OpenGLShaderProgram::getUniformIDFromName (const char* uniformName) const noexcept
{
    // The shader program must be active when this method is called!
    jassert (programID != 0);

    return (GLint) glGetUniformLocation (programID, uniformName);
}

void OpenGLShaderProgram::setUniform (const char* name, GLfloat n1) noexcept                                       { glUniform1f  (getUniformIDFromName (name), n1); }
void OpenGLShaderProgram::setUniform (const char* name, GLint n1) noexcept                                         { glUniform1i  (getUniformIDFromName (name), n1); }
void OpenGLShaderProgram::setUniform (const char* name, GLfloat n1, GLfloat n2) noexcept                           { glUniform2f  (getUniformIDFromName (name), n1, n2); }
void OpenGLShaderProgram::setUniform (const char* name, GLfloat n1, GLfloat n2, GLfloat n3) noexcept               { glUniform3f  (getUniformIDFromName (name), n1, n2, n3); }
void OpenGLShaderProgram::setUniform (const char* name, GLfloat n1, GLfloat n2, GLfloat n3, float n4) noexcept     { glUniform4f  (getUniformIDFromName (name), n1, n2, n3, n4); }
void OpenGLShaderProgram::setUniform (const char* name, GLint n1, GLint n2, GLint n3, GLint n4) noexcept           { glUniform4i  (getUniformIDFromName (name), n1, n2, n3, n4); }
void OpenGLShaderProgram::setUniform (const char* name, const GLfloat* values, GLsizei numValues) noexcept         { glUniform1fv (getUniformIDFromName (name), numValues, values); }
void OpenGLShaderProgram::setUniformMat2 (const char* name, const GLfloat* v, GLint num, GLboolean trns) noexcept  { glUniformMatrix2fv (getUniformIDFromName (name), num, trns, v); }
void OpenGLShaderProgram::setUniformMat3 (const char* name, const GLfloat* v, GLint num, GLboolean trns) noexcept  { glUniformMatrix3fv (getUniformIDFromName (name), num, trns, v); }
void OpenGLShaderProgram::setUniformMat4 (const char* name, const GLfloat* v, GLint num, GLboolean trns) noexcept  { glUniformMatrix4fv (getUniformIDFromName (name), num, trns, v); }

//==============================================================================
OpenGLShaderProgram::Attribute::Attribute (const OpenGLShaderProgram& program, const char* name)
    : attributeID ((GLuint) glGetAttribLocation (program.getProgramID(), name))
{
   #if JUCE_DEBUG && ! JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR
    jassert ((GLint) attributeID >= 0);
   #endif
}

//==============================================================================
OpenGLShaderProgram::Uniform::Uniform (const OpenGLShaderProgram& program, const char* const name)
    : uniformID (glGetUniformLocation (program.getProgramID(), name)), context (program.context)
{
   #if JUCE_DEBUG && ! JUCE_DONT_ASSERT_ON_GLSL_COMPILE_ERROR
    jassert (uniformID >= 0);
   #endif
}

void OpenGLShaderProgram::Uniform::set (GLfloat n1) const noexcept                                    { glUniform1f (uniformID, n1); }
void OpenGLShaderProgram::Uniform::set (GLint n1) const noexcept                                      { glUniform1i (uniformID, n1); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2) const noexcept                        { glUniform2f (uniformID, n1, n2); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2, GLfloat n3) const noexcept            { glUniform3f (uniformID, n1, n2, n3); }
void OpenGLShaderProgram::Uniform::set (GLfloat n1, GLfloat n2, GLfloat n3, float n4) const noexcept  { glUniform4f (uniformID, n1, n2, n3, n4); }
void OpenGLShaderProgram::Uniform::set (GLint n1, GLint n2, GLint n3, GLint n4) const noexcept        { glUniform4i (uniformID, n1, n2, n3, n4); }
void OpenGLShaderProgram::Uniform::set (const GLfloat* values, GLsizei numValues) const noexcept      { glUniform1fv (uniformID, numValues, values); }

void OpenGLShaderProgram::Uniform::setMatrix2 (const GLfloat* v, GLint num, GLboolean trns) const noexcept { glUniformMatrix2fv (uniformID, num, trns, v); }
void OpenGLShaderProgram::Uniform::setMatrix3 (const GLfloat* v, GLint num, GLboolean trns) const noexcept { glUniformMatrix3fv (uniformID, num, trns, v); }
void OpenGLShaderProgram::Uniform::setMatrix4 (const GLfloat* v, GLint num, GLboolean trns) const noexcept { glUniformMatrix4fv (uniformID, num, trns, v); }
