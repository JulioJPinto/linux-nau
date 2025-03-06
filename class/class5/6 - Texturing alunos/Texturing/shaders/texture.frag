#version 330

uniform sampler2D texEarth;
uniform sampler2D texEarthSpec;
uniform sampler2D texEarthNight;
uniform sampler2D texEarthClouds;

uniform float time;

uniform float shininess = 128;

#define PI 3.14159265359
#define TIME_FACTOR 0.0001

in  vec2 texCoord;
in	vec4 eye;
in	vec3 n;
in	vec3 ld;

out vec4 colorOut;

float snoise(vec4 p);

float perlinNoise(vec4 p) {
	float c = 0, amp = 1;
	vec4 freq = p;

	for(int i = 0; i < 5; i++) { 
		c += snoise(freq) * amp;
		freq *= 2;
		amp *= 0.5;
	}

	c = c * 0.5 + 0.5;
	return c;
}

void main() {

	float specularMap = texture(texEarthSpec, texCoord).r;
	//float clouds = texture(texEarthClouds, texCoord).r;

	vec2 t = vec2(2 * PI * texCoord.s + time * TIME_FACTOR, PI * texCoord.t);
	vec3 sc = vec3(sin(t.s) * sin(t.t), cos(t.t), cos(t.s) * sin(t.t));


	//float perlin = perlinNoise(vec4(texCoord * vec2(time * 0.00001, 1.0), 0,0) * 16);
	float perlin = perlinNoise(vec4(sc, time * TIME_FACTOR) * 8);
	float clouds = smoothstep(0.4, 1, perlin);
	clouds = clouds * perlin;

	// set the specular term to black
	vec4 spec = vec4(0.0);

	// normalize both input vectors
	vec3 nd = normalize(n);
	vec3 e = normalize(vec3(eye));

	float intensity = max(dot(nd,ld), 0.0);

	// if the vertex is lit compute the specular color
	if (intensity > 0.0) {
		// compute the half vector
		vec3 h = normalize(ld + e);	
		// compute the specular intensity
		float intSpec = max(dot(h,nd), 0.0);
		// compute the specular term into spec
		spec = vec4(1) * pow(intSpec,shininess) * specularMap;	
	}

	vec4 day = texture(texEarth, texCoord);
	vec4 night = texture(texEarthNight, texCoord);

	vec4 colorDay = clouds * intensity + (1 - clouds) * intensity * day + specularMap * spec;
	vec4 colorNight = (1-clouds) *night;

	float f = smoothstep(0.0, 0.1, intensity);

	colorOut = mix(colorNight, colorDay, f);
}