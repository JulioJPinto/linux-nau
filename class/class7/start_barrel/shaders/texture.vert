#version 330

uniform	mat4 m_pvm;
uniform	mat4 m_viewModel;
uniform	mat4 m_view;
uniform	mat3 m_normal;

uniform	vec4 l_dir;	// global space

in vec4 position;	// local space
in vec3 normal;		// local space
in vec2 texCoord0;
in vec4 tangent;

// the data to be sent to the fragment shader
out Data {
	vec3 eye;
	vec2 texCoord;
	vec3 l_dir;
    vec3 normal;
	vec3 tangent;
	vec3 pos;
	flat int tex;
} DataOut;

float snoise(vec3 p);

void main () {

	int index = gl_InstanceID;
	int h = index / 10000;
	index = index % 10000;
	int col = index % 100;
	int row = index / 100;

	vec4 pos = vec4(col * 2 * 0.68, 2 * h, row * 2 * 0.68, 0) + position;
	DataOut.pos = vec3(pos);

	float noise = snoise(vec3(col * 17, row * 7, h * 13));
	DataOut.tex = int(noise * 5.0);

	// pass through texture coordinates
	DataOut.texCoord = texCoord0;
	
	// all vectors to camera space
	DataOut.normal = normalize(m_normal * normal);
	DataOut.eye = vec3(-(m_viewModel * position));
	DataOut.tangent = vec3(m_viewModel * tangent);
	DataOut.l_dir = vec3(m_view * -l_dir);

	gl_Position = m_pvm * pos;	
}
