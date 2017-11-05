#ifndef ISCENE_H
#define ISCENE_H

#include "nau/attribute.h"
#include "nau/attributeValues.h"
#include "nau/event/iListener.h"
#include "nau/geometry/frustum.h"
#include "nau/scene/camera.h"
#include "nau/scene/light.h"

#include <string>
#include <vector>


#ifdef _WINDLL
#ifdef nau_EXPORTS
#define nau_API __declspec(dllexport)   
#else  
#define nau_API __declspec(dllimport)   
#endif 
#else
#define nau_API
#endif

namespace nau 
{
	namespace scene 
	{
		class SceneObject;

		class IScene : public AttributeValues, public IListener
		{
		public:
			MAT4_PROP(TRANSFORM, 0);

			FLOAT4_PROP(SCALE, 0);
			FLOAT4_PROP(ROTATE, 1);
			FLOAT4_PROP(TRANSLATE, 2);

			FLOAT3_PROP(BB_MIN, 0);
			FLOAT3_PROP(BB_MAX, 1);
			FLOAT3_PROP(BB_CENTER, 2);

			ENUM_PROP(TRANSFORM_ORDER, 0);

			UINT_PROP(TRIANGLE_COUNT, 0);

			typedef enum {
				T_R_S,
				T_S_R,
				R_T_S,
				R_S_T,
				S_R_T,
				S_T_R
			} TransformationOrder;

			static AttribSet Attribs;
			static AttribSet nau_API &GetAttribs(); 

		protected:

			IScene(void) : m_Compiled(false), m_Visible(true) {
				registerAndInitArrays(Attribs);
				m_EmptySOptr = NULL;
			};

			std::string m_Name;
			bool m_Compiled;
			//ITransform *m_Transform;
			//mat4 m_Transform;
			bool m_Visible;
			std::set<std::string> m_MaterialNames;
			std::string m_Type;

			std::shared_ptr<SceneObject> m_EmptySOptr;


			void updateTransform();

			static bool Init();
			static bool Inited;

		public:

			nau_API virtual void setPropf4(Float4Property prop, vec4& aVec);
			nau_API virtual void setPrope(EnumProperty prop, int v);
			nau_API vec3 &getPropf3(Float3Property prop);

			nau_API virtual void setName(std::string name) {
				m_Name = name; 
			};

			nau_API virtual std::string &getName() {
				return m_Name;
			};
		  
			nau_API virtual void add (std::shared_ptr<SceneObject> &aSceneObject) = 0;

			virtual void show (void) {m_Visible = true; }			
			virtual void hide (void) {m_Visible = false; }
			virtual bool isVisible (void) {return m_Visible; }

			nau_API virtual void findVisibleSceneObjects(std::vector<std::shared_ptr<SceneObject>> *v,
														nau::geometry::Frustum &aFrustum,
														Camera &aCamera, 
														bool conservative = false) = 0;
			nau_API virtual void getAllObjects(std::vector<std::shared_ptr<SceneObject>> *) = 0;
			nau_API virtual std::shared_ptr<SceneObject> &getSceneObject (std::string name) = 0;
			nau_API virtual std::shared_ptr<SceneObject> &getSceneObject (int index) = 0;

			nau_API virtual const std::set<std::string> &getMaterialNames() = 0;

			nau_API virtual void build (void) = 0;

			nau_API virtual void compile (void) = 0;
			nau_API virtual void recompile();
			nau_API bool isCompiled() { return m_Compiled;}

			nau_API virtual void unitize() = 0;

			nau_API virtual nau::math::mat4 &getTransform() = 0;
			nau_API virtual void setTransform(nau::math::mat4 &t) = 0;
			nau_API virtual void transform(nau::math::mat4 &t) = 0;

			nau_API virtual nau::geometry::IBoundingVolume& getBoundingVolume (void) = 0;

			nau_API virtual const std::string &getType (void);

			nau_API virtual ~IScene(void) {};

		};
	};
};

#endif
