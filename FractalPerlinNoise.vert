#version 130

//Vertex position attribute
in vec2 VertexPos;
in float Time;

//Multicolor attribute
in vec4 MultiColor;
out vec4 multiColor;
out float time;


void main(void)
{

   multiColor  = MultiColor;
   //Position of the vertex, given in the render function of Window.cpp
   gl_Position = vec4(VertexPos.x, VertexPos.y, 0.0, 1.0);
   time = Time;

}