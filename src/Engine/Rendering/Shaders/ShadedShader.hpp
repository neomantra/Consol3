#ifndef SHADEDSHADER_HPP
#define SHADEDSHADER_HPP

#include "IShader.hpp"

#include "Display/RGBColor.hpp"
#include "Engine/Rendering/DepthBuffer.hpp"
#include "Engine/Rendering/Lighting/LightingSystem.hpp"
#include "Engine/Rendering/Lighting/MaterialProperties.hpp"
#include "Engine/Rendering/Texture.hpp"
#include "Engine/Rendering/Vertex.hpp"
#include "Math/Matrix4.hpp"

#include <cstdint>
#include <functional>
#include <memory>

namespace Engine
{
    namespace Rendering
    {
        namespace Shaders
        {
            using namespace Display;
            using namespace Lighting;

            class ShadedShader : public IShader
            {
            private:
                std::shared_ptr<LightingSystem> lighting_system;
                Vector3 camera_position;

                std::shared_ptr<Texture> texture;
                std::shared_ptr<Texture> normal_map;

                // set by the vertex shader for the fragment shader
                Vertex vert_v0_model;
                Vertex vert_v1_model;
                Vertex vert_v2_model;

                int vert_lights_count = 0;
                // the same vertices but in light space, in multiple lights
                Vertex vert_v0_light[10];
                Vertex vert_v1_light[10];
                Vertex vert_v2_light[10];

                // a pointer here is prefered to avoid calling make_shared on every triangle
                DepthBuffer* vert_light_depthbuffer[10];

                // non linear projections need to account for perspective
                bool vert_light_islinearprojection[10];

                Vector3 frag_position_lights[10];

                bool has_normal_map;
                MaterialProperties material_properties;

            public:
                virtual bool VertexShader(Vertex& v0, Vertex& v1, Vertex& v2, const MVPTransform& mvp_mats) override;
                virtual RGBColor FragmentShader(RGBColor color, const Triangle& triangle, float barcoord0, float barcoord1, float barcoord2) override;

                void SetLightingSystem(std::shared_ptr<LightingSystem> lighting_system);

                void SetTexture(std::shared_ptr<Texture> texture);
                void SetNormalMap(std::shared_ptr<Texture> normal_map);
                void DisableNormalMap();
                void SetCameraPosition(const Vector3& cam_pos);
                void SetMaterialProperties(const MaterialProperties& material_properties);
            };
        }
    }
}

#endif
