#version 330

uniform	mat4 m_pvm;
uniform	mat4 m_viewModel;
uniform	mat4 m_view;
uniform	mat3 m_normal;

uniform	vec4 l_dir;	// global space

in vec4 position;	// local space
in vec3 normal;		// local space
in vec4 tangent;	// local space
in vec2 texCoord0;

// the data to be sent to the fragment shader
out vec3 ld;
out vec2 tc;


void main () {

	tc = texCoord0;

	vec3 n = normalize(m_normal * normal);
	vec3 t = normalize(vec3(m_viewModel * tangent));
	vec3 b = cross(n, t);

	mat3 tbn_trans = transpose(mat3(t, b, n));
	ld = tbn_trans * vec3(m_viewModel * -l_dir);

	gl_Position = m_pvm * position;	
}
