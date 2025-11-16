#version 460 compatibility

uniform vec2 iResolution;

out vec2 pos;

void main(){

	vec4 texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	vec4 vertex = gl_ModelViewProjectionMatrix * gl_Vertex;
	pos = gl_Vertex.xy / iResolution.xy;
	
	gl_Position = vertex;
    gl_TexCoord[0] = texCoord;
    gl_FrontColor = gl_Color;
}
