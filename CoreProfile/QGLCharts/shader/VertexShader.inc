#ifndef VERTEX_SHADER_INC
#define VERTEX_SHADER_INC

static const char *vertexShaderSource =
"#version 430 core \n"
"layout (location = 0) in vec3 aPos; \n"
"layout (location = 1) in vec4 aColor;\n"
"out vec4 vertexColor;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"    vertexColor = aColor;\n"
"}\n";

#endif
