#version 330

uniform sampler2D diffuseY, diffuseG, diffuseR, diffuseB, diffuseBl, normalMap, specularMap, rust;
uniform float shininess = 80;
uniform float rusting = 0.2; 

in Data {
	vec3 eye;
	vec2 texCoord;
	vec3 l_dir;
    vec3 normal;
	vec3 tangent;
	vec3 pos;
	flat int tex;
} DataIn;

out vec4 color;

float perlinNoise(vec3 pos);

void main() {

	if (DataIn.tex == 0) {
		color = texture(diffuseY, DataIn.texCoord);
		return;
	} else if (DataIn.tex == 1) {
		color = texture(diffuseG, DataIn.texCoord);
		return;
	} else if (DataIn.tex == 2) {
		color = texture(diffuseR, DataIn.texCoord);
	} else if (DataIn.tex == 3) {
		color = texture(diffuseB, DataIn.texCoord);
	} else {
		color = texture(diffuseBl, DataIn.texCoord);
	}

	// normalize both input vectors
	vec3 e = normalize(DataIn.eye);
	vec3 l = normalize(DataIn.l_dir);
    vec3 n = normalize(DataIn.normal);
	vec3 t = normalize(DataIn.tangent);

	vec3 b = normalize(cross(n,t));
	if (dot(cross(t,b),n) < 0.0) {
		b = -b;
	}

	mat3 tbn = mat3(t,b,n);
	vec3 texNormal = texture(normalMap, DataIn.texCoord).xyz * 2.0 - 1.0;
	n = normalize(tbn * texNormal);

	float intensity = max(dot(n, l), 0.0);

    float texSpecular = texture(specularMap, DataIn.texCoord).r;
	vec4 spec = vec4(0.0);
	// if the vertex is lit compute the specular color
	if (intensity > 0.0 ) {
		// compute the half vector
		vec3 h = normalize(l + e);	
		// compute the specular intensity
		float intSpec = max(dot(h,n), 0.0);
		// compute the specular term into spec
		spec = vec4(pow(intSpec,shininess)) * texSpecular;
	}

	vec4 texRust = texture(rust, DataIn.texCoord);
	float noise = cos(perlinNoise(DataIn.pos * vec3(1,4,1) * 4));
	noise = smoothstep(0.7, 0.8, noise);

	color = mix(texRust, color, noise);


	color = max(intensity * color + spec * noise  + color * 0.5,0);
}

