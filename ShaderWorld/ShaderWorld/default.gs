#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 10) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void build_house(vec4 position)
{    
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = projection * view * model * (position + vec4(-0.2f, -0.2f, 0.0f, 0.0f));    // 1:bottom-left   
    EmitVertex();   
    gl_Position = projection * view * model * (position + vec4( 0.2f, -0.2f, 0.0f, 0.0f));    // 2:bottom-right
    EmitVertex();
    gl_Position = projection * view * model * (position + vec4(-0.2f,  0.2f, 0.0f, 0.0f));    // 3:top-left
    EmitVertex();
    gl_Position = projection * view * model * (position + vec4( 0.2f,  0.2f, 0.0f, 0.0f));    // 4:top-right
    EmitVertex();
    gl_Position = projection * view * model * (position + vec4( 0.0f,  0.4f, 0.0f, 0.0f));    // 5:top
	fColor = vec3(1.0f, 1.0f, 1.0f);
    EmitVertex();

	gl_Position = projection * view * model * (position + vec4( 0.0f,  0.4f, -1.0f, 0.0f));    // 5:top
	EmitVertex();
	gl_Position = projection * view * model * (position + vec4( 0.2f,  0.2f, -1.0f, 0.0f));    // 4:top-right
	fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    EmitVertex();
	gl_Position = projection * view * model * (position + vec4(-0.2f,  0.2f, -1.0f, 0.0f));    // 3:top-left
    EmitVertex();
	gl_Position = projection * view * model * (position + vec4( 0.2f, -0.2f, -1.0f, 0.0f));    // 2:bottom-right
    EmitVertex();
	gl_Position = projection * view * model * (position + vec4(-0.2f, -0.2f, -1.0f, 0.0f));    // 1:bottom-left   
    EmitVertex();   

    EndPrimitive();
}

void main() {    
    build_house(gl_in[0].gl_Position);
}