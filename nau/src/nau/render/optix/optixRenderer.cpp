#include "nau/render/optix/optixRenderer.h"
#include "nau.h"

using namespace nau::render::optixRender;

optix::Context OptixRenderer::s_Context = NULL;
unsigned int OptixRenderer::s_EntryPointCount; //= 1;
std::map<OptixRenderer::ProgramTypes, optix::Program> OptixRenderer::s_Program;
unsigned int OptixRenderer::s_RayTypeCount = 0;


int
OptixRenderer::Init() {

	if (s_Context == NULL) {
		s_EntryPointCount = 0;
		try {
			s_Context = optix::Context::create();
		}
		catch (optix::Exception& e) {
			s_Context = NULL;
			NAU_THROW("Optix Creating Context Error [%s]", e.getErrorString().c_str());
		}
	}
	return 0;
}


void
OptixRenderer::Terminate() {

	if (s_Context) {
		s_Program.clear();
		s_RayTypeCount = 0;
		s_EntryPointCount = 0;
		s_Context->destroy();
		s_Context = NULL;
	}
}


int 
OptixRenderer::GetNextAvailableRayType() {

	++s_RayTypeCount;
	s_Context->setRayTypeCount(s_RayTypeCount);
	return (s_RayTypeCount-1);
}


int 
OptixRenderer::GetNextAvailableEntryPoint() {

	++s_EntryPointCount;
	s_Context->setEntryPointCount(s_EntryPointCount);
	return(s_EntryPointCount-1);
}




optix::Context &
OptixRenderer::GetContext() {

	return s_Context;
}


void
OptixRenderer::SetProgram(ProgramTypes aType, int rayType, std::string filename, std::string proc) {

	try {
		s_Program[aType] = s_Context->createProgramFromPTXFile( filename, proc);
	}
	catch(optix::Exception& e) {
	
		NAU_THROW("Optix Creating Program Error (file: %s proc: %s) [%s]", filename.c_str(), proc.c_str(), e.getErrorString().c_str());
	}

	try {
		switch(aType) {
	
			case RAY_GEN: s_Context->setRayGenerationProgram(rayType, s_Program[aType] );
				break;
			case EXCEPTION: s_Context->setExceptionProgram( rayType, s_Program[aType] );
				break;
		}
	}
	catch(optix::Exception& e) {
	
		NAU_THROW("Optix Setting Program Error (file: %s proc: %s) [%s]", filename.c_str(), proc.c_str(), e.getErrorString().c_str());
	}
}