// fshader.fs

#version 330 core
out vec4 FragColor;

uniform vec4 selectedColor; // Declare the uniform variable

void main()
{
     FragColor = selectedColor;
}