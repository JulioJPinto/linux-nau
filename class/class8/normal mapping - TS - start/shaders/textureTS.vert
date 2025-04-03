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
out vec2 tc;
out vec3 norm, tang, ld;


void main () {

	tc = texCoord0;

    // move all vectors to camera space
	norm = normalize(m_normal * normal);
	tang = vec3(m_viewModel * tangent);

	ld = vec3(m_view * -l_dir); 

	gl_Position = m_pvm * position;	
}
