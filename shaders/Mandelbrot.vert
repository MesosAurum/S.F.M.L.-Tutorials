#version 460 compatibility

uniform vec2 iResolution;
uniform float iTime;

out vec2 pos;

void main(){

	vec4 texCoord = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	vec4 vertex = gl_ModelViewProjectionMatrix * gl_Vertex;
	pos = 4.0 * gl_Vertex.xy / iResolution - 2.0;

	float zoom = exp(-0.1 * iTime);
	vec2 offset = vec2(-0.743643887037151, 0.131825904205330);

	pos *= zoom;
	pos += offset;

	gl_Position = vertex;
	gl_TexCoord[0] = texCoord;
	gl_FrontColor = gl_Color;
}
