#version 150

in vec2 vertices;

out vec2 tex_coords;

uniform vec2 camera;
uniform float scale;

void main(void){

	gl_Position = vec4((vertices.x-camera.x)*scale,(vertices.y-camera.y)*scale,0.0,1.0);
	vec2 centerTexCoords = vertices * 0.5 + 0.5;
	tex_coords = centerTexCoords;
	
	
}