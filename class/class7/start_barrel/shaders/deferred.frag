tangenttc#version 330

uniform sampler2D diffuseY, diffuseG, diffuseR, diffuseB, diffuseBl, normalMap, specularMap, rust;
uniform sampler2D normal, tangent, texCoord, pos;
uniform mat4 m_view;
uniform float shininess = 80;
uniform float rusting = 0.2; 

in vec2 tc;
in vec3 ld;

out vec4 color;

float perlinNoise(vec3 pos);

void main() {

    vec4 n4 = texture(normal, tc);

    if (n4.w == 0) discard;

    vec3 n = n4.xyz * 2 - 1;
    vec2 tc_obj = texture(texCoord, tc).st;
    vec3 t = texture(tangent, tc).xyz * 2 - 1;
    vec4 p = texture(pos, tc);

    int c = int(p.w);

    vec3 pos_cam = p.xyz;
    

	if (c == 0) {
		color = texture(diffuseY, tc_obj);
		return;
	} else if (c == 1) {
		color = texture(diffuseG, tc_obj);
		return;
	} else if (c == 2) {
		color = texture(diffuseR, tc_obj);
	} else if (c == 3) {
		color = texture(diffuseB, tc_obj);
	} else {
		color = texture(diffuseBl, tc_obj);
	}

	// normalize both input vectors
	// vec3 e = normalize(DataIn.eye);
	vec3 l = normalize(ld);
    // vec3 n = normalize(DataIn.normal);
	//vec3 t = normalize(DataIn.tangent);

	vec3 b = normalize(cross(n,t));
	if (dot(cross(t,b),n) < 0.0) {
		b = -b;
	}

	mat3 tbn = mat3(t,b,n);
	vec3 texNormal = texture(normalMap, tc_obj).xyz * 2.0 - 1.0;
	n = normalize(tbn * texNormal);

	float intensity = max(dot(n, l), 0.0);

    float texSpecular = texture(specularMap, tc_obj).r;
	vec4 spec = vec4(0.0);
	// if the vertex is lit compute the specular color
	if (intensity > 0.0 ) {
		// compute the half vector
		vec3 h = normalize(l - pos_cam);	
		// compute the specular intensity
		float intSpec = max(dot(h,n), 0.0);
		// compute the specular term into spec
		spec = vec4(pow(intSpec,shininess)) * texSpecular;
	}

	vec4 texRust = texture(rust, tc_obj);
	float noise = cos(perlinNoise(pos_cam * vec3(1,4,1) * 4));
	noise = smoothstep(0.7, 0.8, noise);

	color = mix(texRust, color, noise);


	color = max(intensity * color + spec * noise  + color * 0.5,0);
}

