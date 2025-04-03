#version 330

uniform sampler2D diffuseY, diffuseG, diffuseR, diffuseB, diffuseBl, normalMap, specularMap, rust;
uniform float shininess = 80;
uniform float rusting = 0.2; 

layout (location = 0) out vec4 normal;
layout (location = 1) out vec4 tangent;
layout (location = 2) out vec4 texCoord;
layout (location = 3) out vec4 pos;

in Data {
	vec2 texCoord;
    vec3 normal;
	vec3 tangent;
	vec3 pos;
	flat int tex;
} DataIn;

out vec4 color;

float perlinNoise(vec3 pos);

void main() {

    normal = vec4(normalize(DataIn.normal) * 0.5 + 0.5, 1);
    tangent = vec4(normalize(DataIn.tangent) * 0.5 + 0.5, 1);
    texCoord = vec4(DataIn.texCoord, 0, 1);
    pos = vec4(DataIn.pos, DataIn.tex);
}

