#include "nau/material/iTexture.h"

#include "nau.h"
#include "nau/loader/iTextureLoader.h"
#ifdef NAU_OPENGL
#include "nau/render/opengl/glTexture.h"
#endif

using namespace nau::render;
using namespace nau::loader;
using namespace nau;


bool
ITexture::Init() {

	// INT
	Attribs.add(Attribute(ID, "ID", Enums::DataType::INT, true, new NauInt(-1)));
	Attribs.add(Attribute(WIDTH, "WIDTH", Enums::DataType::INT, false, new NauInt(1)));
	Attribs.add(Attribute(HEIGHT, "HEIGHT", Enums::DataType::INT, false, new NauInt(1)));
	Attribs.add(Attribute(DEPTH, "DEPTH", Enums::DataType::INT, false, new NauInt(1)));
	Attribs.add(Attribute(SAMPLES, "SAMPLES", Enums::DataType::INT, false, new NauInt(0)));
	Attribs.add(Attribute(LEVELS, "LEVELS", Enums::DataType::INT, false, new NauInt(1)));
	Attribs.add(Attribute(LAYERS, "LAYERS", Enums::DataType::INT, false, new NauInt(1)));
	Attribs.add(Attribute(COMPONENT_COUNT, "COMPONENT_COUNT", Enums::DataType::INT, true, new NauInt(0)));
	Attribs.add(Attribute(ELEMENT_SIZE, "ELEMENT_SIZE", Enums::DataType::INT, true, new NauInt(0)));
	Attribs.add(Attribute(SEED, "SEED", Enums::DataType::INT, true, new NauInt(0)));
	// BOOL
	Attribs.add(Attribute(MIPMAP, "MIPMAP", Enums::DataType::BOOL, false, new NauInt(false)));
	// ENUM
	Attribs.add(Attribute(DIMENSION, "DIMENSION", Enums::DataType::ENUM, true));
	Attribs.add(Attribute(FORMAT, "FORMAT", Enums::DataType::ENUM, true));
	Attribs.add(Attribute(TYPE, "TYPE", Enums::DataType::ENUM, true));
	Attribs.add(Attribute(INTERNAL_FORMAT, "INTERNAL_FORMAT", Enums::DataType::ENUM, false));

	Attribs.add(Attribute(CONTENT, "CONTENT", Enums::DataType::ENUM, false));
	Attribs.listAdd("CONTENT", "NONE", CONTENT_NONE);
	Attribs.listAdd("CONTENT", "RANDOM_UNIFORM", CONTENT_RANDOM_UNIFORM);
	Attribs.listAdd("CONTENT", "RANDOM_NORMAL", CONTENT_RANDOM_NORMAL);
    NauInt n = NauInt(0);
	Attribs.setDefault("CONTENT", n);


	//#ifndef _WINDLL
	// for direct access
	NAU->registerAttributes("TEXTURE", &Attribs);
	// for access through material
	NAU->registerAttributes("TEXTURE_MATERIAL", &Attribs);
	//#endif

	return true;
}


AttribSet ITexture::Attribs;
bool ITexture::Inited = Init();


AttribSet &
ITexture::GetAttribs() {
	return Attribs;
}


ITexture*
ITexture::Create (std::string label, std::string internalFormat,
	int width, int height, int depth, int layers, int levels, int samples) {

#ifdef NAU_OPENGL
		return new GLTexture (label, internalFormat, width, height, depth, layers, levels, samples);
#elif NAU_DIRECTX
	//Put here function for DirectX
#endif
}


ITexture*
ITexture::Create(std::string label, int internalFormat,
	int width, int height, int depth, int layers, int levels, int samples) {

#ifdef NAU_OPENGL
	return new GLTexture(label, internalFormat, width, height, depth, layers, levels, samples);
#elif NAU_DIRECTX
	//Put here function for DirectX
#endif
}


ITexture*
ITexture::Create(std::string label) {

#ifdef NAU_OPENGL
	return new GLTexture(label);
#elif NAU_DIRECTX
	//Put here function for DirectX
#endif
}


ITexture*
ITexture::Create (std::string file, std::string label, bool mipmap) {

	ITextureLoader *loader = ITextureLoader::create(file);

	int success = loader->loadImage (false);
	if (success) {
		std::string aFormat = loader->getFormat();
		std::string sizedFormat = loader->getSizedFormat();
		ITexture *t;

	#ifdef NAU_OPENGL
		t = new GLTexture (label, sizedFormat, aFormat, loader->getType(),
				loader->getWidth(), loader->getHeight(), loader->getDepth(), loader->getData(), mipmap);
	#elif NAU_DIRECTX
		t = new DXTexture (aDimension, aFormat, width, height);
	#endif

	//#ifdef __COMPOSER__

	//	ITexImage *ti = ITexImage::create(t);
	//	unsigned char *data = ti->getRGBData();
	//	ilConvertImage(IL_RGB,IL_UNSIGNED_BYTE);
	//	//iluScale(96,96,1);
	//	wxImage ima = wxImage(loader->getWidth(), loader->getHeight(), data, true);
	//	ima.Rescale(96, 96);
	//	t->bitmap = new wxBitmap(ima.Mirror(false));
	//#endif

		t->m_Filename = file;

		loader->convertToRGBA();
		
		//t->data = (char *)malloc(loader->getHeight() * loader->getWidth() );
		//memcpy(t->data, loader->getData(), loader->getHeight() * loader->getWidth() );
		t->data = (char *)malloc(loader->getHeight() * loader->getWidth() * 4);
		memcpy(t->data, loader->getData(), loader->getHeight() * loader->getWidth() * 4);

		loader->freeImage();
		delete loader;
		return t;
	}
	else {
		delete loader;
		return NULL;
	}
}


ITexture::ITexture(std::string label) :m_Label(label), data(NULL), m_Filename("") {

	registerAndInitArrays(Attribs);
	
}


ITexture::~ITexture() {

	if (data) {
		free(data);
		data = NULL;
	}
}

std::string&
ITexture::getLabel (void) {

	return m_Label;
}


void
ITexture::setLabel (std::string label) {

	m_Label = label;
}


const std::string &
ITexture::getFilename() {

	return m_Filename;
}