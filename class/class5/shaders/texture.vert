#version 330

uniform	mat4 m_pvm;
uniform	mat4 m_viewModel;
uniform	mat4 m_view;
uniform	mat3 m_normal;

uniform	vec4 l_dir;	  // global space

in vec4 position;	// local space
in vec3 normal;		// local space
in vec2 texCoord0;	// the texture coordinates

// the data to be sent to the fragment shader

out vec2 texCoord;
out vec4 eye;
out vec3 n;
out vec3 ld;

void main () {
	texCoord = texCoord0;
	n = normalize(m_normal * normal);
	eye = -(m_viewModel * position);
	ld = normalize(vec3(m_view * -l_dir));

	gl_Position = m_pvm * position;	
}