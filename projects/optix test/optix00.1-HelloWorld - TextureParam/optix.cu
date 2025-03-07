
#include "optixParams.h" // our launch params
  
extern "C" {
// name "optixLaunchParams" is hardcoded in Nau
__constant__ LaunchParams optixLaunchParams;

// -----------------------------------------------------------------------------
// Nau requires at least one hit and miss groups

// closest hit program
extern "C" __global__ void __closesthit__phong() { 
  
}
  
// any hit program
extern "C" __global__ void __anyhit__phong() {
	
}

// miss program 
extern "C" __global__ void __miss__phong() {
	
}

//------------------------------------------------------------------------------
// ray gen program 
//------------------------------------------------------------------------------


extern "C" __global__ void __raygen__renderFrame()  {


	const uint3 index = optixGetLaunchIndex();
	
	// compute a test pattern based on pixel ID

    // compute some color based on launch index
	uint3 aux = index - optixGetLaunchDimensions() * 0.5;
    float pixelIntensity = 0.5 + 0.5 * 
        cos((aux.x + optixLaunchParams.frame.frame * 0.1) )  ;

    // compute texture coordinates
    float s = index.x / 512.0;
    float t = index.y / 512.0;  

    // fetch texture color
    float4 c = tex2D<float4>(optixLaunchParams.global->tex,s,t);
	const int r = c.x * 255.0;
	const int g = c.y * 255.0;
	const int b = c.z * 255.0;

    // convert to 32-bit rgba value - explicitly set alpha to 0xff
	const uint32_t rgba = 0xff000000  | (r<<0) | (g<<8) | (b<<16);

    // compute output buffer index based on laaunch index and dimensions
	const unsigned int fbIndex = index.x + (index.y * optixGetLaunchDimensions().x);
    
    // print only once
	if (optixLaunchParams.frame.frame == 0 && index.x == 0 && index.y == 0) {

		// print info to console
		printf("===========================================\n");
		printf("Nau Ray-Tracing Hello World\n");
		printf("Launch size: %i x %i\n", optixGetLaunchDimensions().x, optixGetLaunchDimensions().y);
		printf("Color: %f %f %f\n", 
				optixLaunchParams.global->color.x,
				optixLaunchParams.global->color.y,
				optixLaunchParams.global->color.z
			);
		printf("===========================================\n");
	}
    // fill output buffer -> this will be copied to a render target
	optixLaunchParams.frame.colorBuffer[fbIndex] = rgba;
}
  
} // end of extern
