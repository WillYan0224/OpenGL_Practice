#shader vertex
#version 330 core

layout(location = 0) in vec4 positions; // Input

void main()
{
	gl_Position = positions;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; // Output

uniform vec4 u_Color; // u_XXXはuniform変数 Declear Uniform


void main()
{
	
	color = u_Color;
};