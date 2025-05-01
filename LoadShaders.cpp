#include <iostream>
#include "LoadShaders.h"


static const GLchar* ReadShader( const char* filename )
{
    FILE* file = fopen( filename, "rb" );
    if ( !file )
        return NULL;

    fseek( file, 0, SEEK_END );
    int len = ftell( file );
    fseek( file, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, file );
    fclose( file );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}


GLuint LoadShaders(ShaderInfo info)
{
    GLuint shader = glCreateShader(info.type);
    const GLchar* source = ReadShader(info.filename);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    /* code to check compile error */
    // GLint compiled;
    // glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    // if (!compiled) 
    // {
    //     GLsizei len;
    //     glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

    //     GLchar* log = new GLchar[len + 1];
    //     glGetShaderInfoLog(shader, len, &len, log);
    //     std::cerr << "Shader compilation failed: " << log << std::endl;
    //     delete[] log;

    //     return 0;
    // }

    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glLinkProgram(program);

    /* code to check link error */
    // GLint linked;
    // glGetProgramiv( program, GL_LINK_STATUS, &linked );
    // if ( !linked )
    // {
    //     GLsizei len;
    //     glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

    //     GLchar* log = new GLchar[len+1];
    //     glGetProgramInfoLog( program, len, &len, log );
    //     std::cerr << "Shader linking failed: " << log << std::endl;
    //     delete [] log;
        
    //     return 0;
    // }

    glDeleteShader(shader);
    return program;
}