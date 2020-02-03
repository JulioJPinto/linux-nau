#ifndef OPTIXMATERIAL_H
#define OPTIXMATERIAL_H

#include "nau/config.h"
#if NAU_OPTIX = 1

#include <map>
#include <string>

#include <optix.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>


namespace nau {
  namespace render {
   namespace optixRender {
		
		
	class OptixMaterial {

	public:

		OptixMaterial();
		void setMaterialProgram(unsigned int aProgram, int rayType, std::string filename, std::string proc);
		void applyMaterialPrograms(optix::Material);

	private:
		

		std::map<int, optix::Program> m_ClosestHitProgram;
		std::map<int, optix::Program> m_AnyHitProgram;
		optix::Context m_Context;
	};
   };
  };
};

#endif

#endif