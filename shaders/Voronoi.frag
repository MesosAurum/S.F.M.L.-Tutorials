#version 460 core

in vec2 pos;

uniform float iTime;

float random(vec2 u){
	
	float a = dot(u, vec2(12.9898, 78.233));
	return fract(sin(a) * 43758.5453);
}
vec2 random2(vec2 u) {
	
	float a = dot(u, vec2(127.1, 311.7));
	float b = dot(u, vec2(269.5, 183.3));
	return fract(sin(vec2(a, b)) * 43758.5453);
}
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
	
	//Our current position in clip-space.
	vec2 uv = pos;
	vec3 color = vec3(0.0);

	//Zooming out, the larger the number the more we zoom out.
	uv *= 24.0;

	//The tile coordinates of the uv.
	vec2 iUv = floor(uv);
	//The uv coords in the tile.
	vec2 fUv = fract(uv);

	//Will be the smallest distance to our uv.
	//Changed to 2.0 for more coverage when using different distance functions.
	float minDist = 2.0;
	//Will be the closest point to our uv.
	vec2 minPoint;
	for(int i = -1; i <= 1; ++i){
		for(int j = -1; j <= 1; ++j){
			
			//The neighboring cell coordinates (can be the cell the uv is within).
			vec2 neighbor = vec2(float(i), float(j));
			//The point we will be checking our distance to.
			//iUv + neighbor is our neigbor point's coordinates.
			vec2 point = random2(iUv + neighbor);

			//Weight will "round" the sides of some (random) cells.
			float weight = random(point);

			//Moves the points around in their cell.
			point = 0.5 * (sin(6.2831853 * point + iTime) + 1.0);

			//Tile coordinate + point in neighbor tile - uv position in uv's tile.
			vec2 diff = neighbor + point - fUv;

			//Exponent for Minkowsky distances.
			//0.2 - 0.5 stars
			//1.0 = Manhattan
			//2.0 = Euclidean
			//Limit as the exponent approaches infinity = Chess
			float p = 0.5;

			//The distance from our uv to the neighboring point.
			//Euclidean distance.
			float dist = length(diff);

			//Manhattan/Taxicab distance.
			//dist = abs(diff.x) + abs(diff.y);
			//Chess distance.
			//dist = max(abs(diff.x), abs(diff.y));
			//Minkowsky distance.
			//dist = pow(pow(abs(diff.x), p) + pow(abs(diff.y), p), 1.0 / p);

			dist += weight;

			//minDist = min(minDist, dist);//For Worley noise.
			//For Voronoi noise.
			if(dist < minDist){
			
				minDist = dist;
				//Hold onto the closest point to our uv.
				minPoint = point;
			}
		}
	}
	//For shading the distance from an edge of a cell to the center point of that cell.
	//color = vec3(minDist);
	//For the color of voronoi noise.
	color.gb = minPoint;

	//The color we get out.
	fragColor = vec4(color, 1.0);
}
void main() {

	mainImage(gl_FragColor, gl_FragCoord.xy);
}
