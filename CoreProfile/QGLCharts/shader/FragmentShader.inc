#ifndef FRAGMENT_SHADER_INC
#define FRAGMENT_SHADER_INC

static const char *standardFgSource =
"#version 430 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor; \n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vertexColor;\n"
"}\n";

static const char *pointFgSource =
        "#version 430 core \n"
        "in vec4 vertexColor;\n"
        "out vec4 fragColor;\n"
        "\n"
        "void main() {\n"
        "    if (length(gl_PointCoord - vec2(0.5)) > 0.5) {\n"
        "        discard;\n"
        "    }\n"
        "    fragColor = vertexColor;\n"
        "}\n";

#endif
