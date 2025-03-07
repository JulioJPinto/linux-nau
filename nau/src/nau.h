#ifndef NAU_H
#define NAU_H


#pragma warning( disable: 4290)

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "iNau.h"
#include "nau/config.h"
#include "nau/attribute.h"
#include "nau/attributeValues.h"

#include "nau/errors.h"
#include "nau/event/eventManager.h"
#include "nau/event/iListener.h"
#include "nau/material/materialLibManager.h"
#include "nau/math/vec3.h"
#include "nau/math/vec4.h"
#include "nau/physics/physicsManager.h"
#include "nau/render/pipeline.h"
#include "nau/render/pass.h"
#include "nau/render/viewport.h"
#include "nau/render/renderManager.h"
#include "nau/resource/resourceManager.h"
#include "nau/scene/iScene.h"
#include "nau/scene/camera.h"
#include "nau/scene/light.h"
//#include "nau/world/iWorld.h"


//#if NAU_LUA == 1
//extern "C" {
//#include <lua/lua.h>
//#include <lua/lauxlib.h>
//#include <lua/lualib.h>
//}
//
//#endif

//#include <AntTweakBar.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <map>

using namespace nau;

//I know Peter, but you'll see that this may come in handy ;)
//#define NAU Nau::GetInstance()
//#define RENDERER NAU->getRenderManager()->getRenderer()
//#define RENDERMANAGER NAU->getRenderManager()
//#define MATERIALLIBMANAGER NAU->getMaterialLibManager()
//#define RESOURCEMANAGER NAU->getResourceManager()
//#define EVENTMANAGER NAU->getEventManager()
//#define APISupport NAU->getAPISupport()

#define NAU_KEY_MOD_NONE	0x0000
#define NAU_KEY_MOD_SHIFT	0x0003
#define NAU_KEY_MOD_CTRL	0x00c0
#define NAU_KEY_MOD_ALT		0x0100

#ifdef _WINDLL
#ifdef nau_EXPORTS
#define nau_API __declspec(dllexport)   
#else  
#define nau_API __declspec(dllimport)   
#endif 
#else
#define nau_API
#endif

struct lua_State;

namespace nau {
	
	const double NO_TIME = -1.0f;

	class Nau : public INau // , public IListener, public AttributeValues
	{

	public:

		float g_Zoom;

		typedef enum  {
			RELEASED,
			PRESSED
		} MouseAction;

		typedef enum  {
			NONE = 0,
			LEFT,
			MIDDLE,
			RIGHT
		} MouseButton;

		typedef enum {
			KEY_MOD_NONE =	0x0000,
			KEY_MOD_SHIFT =	0x0003,
			KEY_MOD_CTRL =	0x00c0,
			KEY_MOD_ALT = 0x0100
		} KeyModifiers;

		typedef enum  { // same order as AntTweakToolbar
			KEY_BACKSPACE = '\b',
			KEY_TAB = '\t',
			KEY_CLEAR = 0x0c,
			KEY_RETURN = '\r',
			KEY_PAUSE = 0x13,
			KEY_ESCAPE = 0x1b,
			KEY_SPACE = ' ',
			KEY_DELETE = 0x7f,
			KEY_UP = 273,      // same codes and order as SDL 1.2 keysym.sym
			KEY_DOWN,
			KEY_RIGHT,
			KEY_LEFT,
			KEY_INSERT,
			KEY_HOME,
			KEY_END,
			KEY_PAGE_UP,
			KEY_PAGE_DOWN,
			KEY_F1,
			KEY_F2,
			KEY_F3,
			KEY_F4,
			KEY_F5,
			KEY_F6,
			KEY_F7,
			KEY_F8,
			KEY_F9,
			KEY_F10,
			KEY_F11,
			KEY_F12,
			KEY_F13,
			KEY_F14,
			KEY_F15,
			KEY_LAST
		} KeySpecial;


		static nau_API nau::INau* Create (void);
		static nau_API nau::INau* GetInstance (void);
		
//#ifdef _WINDLL
		static nau_API void SetInstance(Nau *inst);
//#endif

		nau_API bool init(bool trace);
		// Returns the class name
		nau_API std::string &getName();

		void setProjectName(std::string name);
		const std::string &getProjectName();

		std::string getProjectFolder();

		// Lua Stuff
#if NAU_LUA == 1

		void initLua();
		void initLuaScript(std::string file, std::string name);
		void compileLuaScripts();
		const std::string &getLuaLog();
		void callLuaScript(std::string name);
		bool callLuaTestScript(std::string name);

		static void luaGetValues(lua_State *l, void *arr, int card, Enums::DataType bdt);
		static void luaStackDump(lua_State *m_LuaState);
		static int luaGet(lua_State *l);
		static int luaSet(lua_State *l);
		static int luaGetBuffer(lua_State *l);
		static int luaSetBuffer(lua_State *l);
		static int luaSaveTexture(lua_State *l);
		static int luaSaveBuffer(lua_State *l);
		static int luaSaveProfile(lua_State *l);
		static int luaScreenshot(lua_State* l);

		static bool luaCheckScriptName(std::string fileName, std::string scriptName);
		static std::map<std::string, std::string> LuaScriptNames;
#endif


		// Attributes

		// Global gets and sets
		// note: gets and set perform no validation
		// if in doubt call validate first

		// Fully validate - context must refer to an existing object
		//bool validateAttribute(std::string type, std::string context, std::string component);
		// Only validates the existence of the component in a particular type/context of object
		bool validateShaderAttribute(std::string type, std::string context, std::string component);
		// returns Enums::COUNT_DATATYPE if attribute does not exist
		Enums::DataType getAttributeDataType(std::string type, std::string context, std::string component);
		bool setAttributeValue(std::string type, std::string context,
				 std::string component, int number,
				 Data *values);
		void *getAttributeValue(std::string type, std::string context,
			std::string component, int number=0);

		/// returns the attribute, usefull to get attr info, such as max, min, etc
		/// type is assumed to be valid. Use validateObjectType to check
		std::unique_ptr<Attribute> &getAttribute(const std::string &type, const std::string &component);

		AttributeValues *getObjectAttributes(const std::string &type, const std::string &context, 
			int number=0);
		AttributeValues *getCurrentObjectAttributes(const std::string &type, int number = 0);

		AttributeValues *createObject(const std::string &objType, const std::string &name);

		bool validateObjectType(const std::string & type);
		void getValidObjectTypes(std::vector<std::string>* v);

		void getValidObjectNames(const std::string & type, std::vector<std::string>* v);

		bool validateObjectName(const std::string & type, const std::string & v);

		bool validateObjectContext(const std::string & type, const std::string & context);

		bool validateObjectComponent(const std::string & type, const std::string & component);
		void getValidObjectComponents(const std::string &type, std::vector<std::string>* v);

		void registerAttributes(std::string s, AttribSet *attrib);
		bool validateUserAttribType(std::string s);
		bool validateUserAttribName(std::string context, std::string name);
		AttribSet *getAttribs(std::string context);
		void deleteUserAttributes();
		void getObjTypeList(std::vector<std::string> *);

		// Events
		void eventReceived(const std::string &sender, const std::string &eventType, 
			const std::shared_ptr<IEventData> &evt);

		// Mouse and Keyboard interface
		nau_API int keyPressed(int key, int modifiers);
		nau_API int mouseButton(Nau::MouseAction action, Nau::MouseButton buttonID, int x, int y);
		nau_API int mouseMotion(int x, int y);
		nau_API int mouseWheel(int pos);

		// Camera and Viewport
		void setActiveCameraName(const std::string &aCamName);
		nau::scene::Camera *getActiveCamera();
		std::shared_ptr<Viewport> getDefaultViewport ();
		std::shared_ptr<Camera> getDefaultCamera();

		// provides linear depth at the center pixel
		float getDepthAtCenter();

		// Profile Reset
		nau_API void setProfileResetRequest();
		nau_API bool getProfileResetRequest();

		// Tracing
		nau_API void setTrace(int frames);
		nau_API bool getTraceStatus();


		// Stepping
		// Executes the whole pipeline
		nau_API void step ();
		// Executes the next pass
		// only to be used when in paused mode
		nau_API void stepPass();
		// Executes the pipeline from the current pass to the end
		// only to be used when in paused mode
		nau_API void stepCompleteFrame();
		// executes n passes from the pipeline. It may loop.
		// only to be used when in paused mode
		nau_API void stepPasses(int n);


		void loadAsset (std::string aFilename, std::string sceneName, std::string params = "") ;//throw (std::string);
		void writeAssets (std::string fileType, std::string aFilename, std::string sceneName);
		nau_API void enablePhysics (void);
		nau_API void disablePhysics (void);

		// Window stuff
		void setWindowSize (unsigned int width, unsigned int height);
		unsigned int getWindowHeight();
		unsigned int getWindowWidth();



		// Readers 
		nau_API void readModel (std::string filename) throw (std::string);
		void appendModel(std::string filename);
		nau_API void readProjectFile (std::string file, int *width, int *height);
		nau_API void readDirectory (std::string dirName);
		nau_API void saveProject(std::string filename);

		// Managers 
		nau::render::RenderManager* getRenderManager (void);
		nau::resource::ResourceManager* getResourceManager (void);
		nau::material::MaterialLibManager* getMaterialLibManager (void);
		nau::event_::EventManager* getEventManager (void);
		nau::render::IRenderer *getRenderer(void);
		nau::physics::PhysicsManager *getPhysicsManager();
		IAPISupport * getAPISupport(void);

		/* Render Flags */
		nau_API void setRenderFlag(RenderFlags aFlag, bool aState);
		nau_API bool getRenderFlag(RenderFlags aFlag);

		nau_API ~Nau(void);
		nau_API void clear();


	private:
		Nau();


#if NAU_LUA == 1
		static std::string LuaCurrentScript;
		static std::set<string> LuaFilesWithIssues;
		static lua_State *LuaState;
#endif
		static Nau *Instance;

		std::string m_ProjectName;
		float m_StartTime;

		std::string m_Name;

		bool m_ProfileResetRequest;
		int m_TraceFrames;
		bool m_TraceOn;

		MouseButton m_MouseButton;

		/* 	Attributes	*/
		typedef AttribSet *AttribSetPointer;
		AttribSetPointer a;
		std::map<std::string, AttribSet*> m_Attributes;

		/*
		 * Rendering Flags
		 */
		std::vector<bool> m_RenderFlags;
		bool m_UseTangents, m_UseTriangleIDs, m_CoreProfile;
		/*
		 * Managers
		 */
		nau::render::RenderManager *m_pRenderManager;
		nau::resource::ResourceManager *m_pResourceManager;
		nau::material::MaterialLibManager *m_pMaterialLibManager;
		nau::event_::EventManager *m_pEventManager;
		nau::render::IAPISupport *m_pAPISupport;
		nau::physics::PhysicsManager *m_pPhysicsManager;

		std::string m_AppFolder;
		std::string m_ProjectFolder;
		std::string m_ActiveCameraName;
		unsigned int m_WindowWidth, m_WindowHeight;
		std::shared_ptr<Viewport> m_Viewport;
		std::shared_ptr<Camera> m_Camera;
		IState *m_DefaultState;

		bool m_Inited;

		bool m_Physics;
		//nau::world::IWorld *m_pWorld;
		
		//double m_CurrentTime;
		double m_LastFrameTime;

		double CLOCKS_PER_MILISEC;
		double INV_CLOCKS_PER_MILISEC;

		// different perspective and camera position depending on whether
		// the model is unitized
		void loadFilesAndFoldersAux(std::string sceneName, bool unitize);

		int loadedScenes;

	};
};

#endif //NAU_H
