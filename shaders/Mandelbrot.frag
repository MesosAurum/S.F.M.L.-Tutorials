#version 460 core

in vec2 pos;

int maxIteration = 1000;

vec3 hueSatLight(float h, float s, float l){
	
	vec3 n = vec3(0.0, 8.0, 4.0);
	vec3 k = mod(n + 0.0333333 * h, 12.0);
	vec3 minimum = min(min(k - 3.0, 9.0 - k), 1.0);
	float a = s * min(l, l - 1.0);

	return l - a * max(vec3(-1.0), minimum);
}
vec3 palette(float i){
	
	vec3 hSL;
	float it = i / maxIteration;
	hSL.x = mod(pow(360.0 * it, 1.5), 360.0);
	hSL.y = 1.0;
	hSL.z = it;

	return hueSatLight(hSL.x, hSL.y, hSL.z);
}
void mainImage(out vec4 fragColor, in vec2 fragCoord){
	
	vec2 c = pos;
	vec2 z = vec2(0.0);

	float iteration = 0;

	float x2 = 0.0;
	float y2 = 0.0;
	while(x2 + y2 <= 256.0 && iteration < maxIteration){
		
		z = vec2(x2 - y2, 2.0 * z.x * z.y) + c;
		x2 = z.x * z.x;
		y2 = z.y * z.y;

		++iteration;
	}
	if(iteration < maxIteration){
		
		float logZn = 0.5 * log2(x2 + y2);
		float invLog2 = 1.0 / log2(2.0);
		float nu = log2(logZn * invLog2) * invLog2;

		iteration += 1.0 - nu;
	}
	vec3 color0 = palette(floor(iteration));
	vec3 color1 = palette(floor(iteration) + 1.0);
	vec3 color = mix(color0, color1, fract(iteration));

	fragColor = vec4(color, 1.0);
}
void main(){

	mainImage(gl_FragColor, gl_FragCoord.xy);
}
