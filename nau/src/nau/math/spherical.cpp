#include "nau/math/spherical.h"
#include "nau/math/utils.h"

using namespace nau::math;


// Tolerance factor to prevent division by 0
static const double tol = FLT_EPSILON;




   
vec2
Spherical::toSpherical(float x, float y ,float z) 
{
	vec2 result;
	vec3 aux(x,y,z);
	aux.normalize();

	result.y = asinf(aux.y);

	if (aux.z >= 0)
		result.x = asinf(aux.x / sqrtf(aux.x*aux.x + aux.z*aux.z));
	else
		result.x = (float)M_PI - asinf(aux.x / sqrtf(aux.x*aux.x + aux.z*aux.z));

	return result;
}


vec3 
Spherical::toCartesian(float alpha, float beta)
{
	vec3 v;
	float beta_aux, alpha_aux;

	if (beta > M_PI * 0.5f) {
		beta_aux = (float)M_PI - beta;
		alpha_aux = alpha - (float)M_PI;
	}
	else if (beta < -(float)M_PI * 0.5f) {
	
		beta_aux = (float)M_PI - beta;
		alpha_aux = alpha - (float)M_PI; 
	}
	else {
		beta_aux = beta;
		alpha_aux = alpha;
	}

	v.x = cosf(beta_aux) * sinf(alpha_aux);
	v.z = cosf(beta_aux) * cosf(alpha_aux);
	v.y = sinf(beta_aux);

	return v;
}

vec3
Spherical::getRightVector(float alpha, float beta) 
{
	float alpha_aux;
	vec3 v;
	if (beta > M_PI * 0.5f) {
	
		alpha_aux = alpha - (float)M_PI * 0.5f;
	}
	else if  (beta < -M_PI * 0.5f) {
	
		alpha_aux = alpha - (float)M_PI * 0.5f;
	}
	else {
		alpha_aux = alpha - (float)M_PI * 0.5f;
	}

	v.x = cosf(0.0f) * sinf(alpha_aux);
	v.z = cosf(0.0f) * cosf(alpha_aux);
	v.y = sinf(0.0f);

	return v;
}

vec3
Spherical::getNaturalUpVector(float alpha, float beta) 
{
	float alpha_aux, beta_aux;
	vec3 v;
	// 2nd quadrant
	if (beta > M_PI * 0.5f) {
	
		alpha_aux = alpha;
		beta_aux = (float)M_PI - beta;

	}
	// 3rd quadrant
	else if (beta < -M_PI * 0.5f) {
	
		alpha_aux = alpha  - (float)M_PI;
		beta_aux = (float)M_PI - beta;

	} 
	// 1st quadrant
	else if ( beta >= 0.0f) {

		alpha_aux = alpha - (float)M_PI;
		beta_aux = beta;
	}
	// 4th quadrant
	else {
		alpha_aux = alpha;
		beta_aux = beta;
	}

	v.x = cosf((float)(M_PI * 0.5f - fabs(beta_aux))) * sinf(alpha_aux);
	v.z = cosf((float)(M_PI * 0.5f - fabs(beta_aux))) * cosf(alpha_aux);
	v.y = sinf((float)(M_PI * 0.5f - fabs(beta_aux)));

	return v;

	//if (m_ElevationAngle >= 0)
	//	alpha = m_ZXAngle - M_PI;
	//else 
	//	alpha = m_ZXAngle;
	//m_UpVector.x = cos( float(M_PI * 0.5 - fabs(m_ElevationAngle)) ) * sin( alpha );
	//m_UpVector.z = cos( float(M_PI * 0.5 - fabs(m_ElevationAngle)) ) * cos( alpha );
	//m_UpVector.y = sin( float(M_PI * 0.5 - fabs(m_ElevationAngle)) );
}


float
Spherical::capBeta(float beta) 
{
	if (beta > M_PI * 0.48)
		return (float)M_PI * 0.48f;
	else if (beta < -M_PI * 0.48)
		return - (float)M_PI * 0.48f;
	else
		return beta;
}
   

