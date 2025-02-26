#ifndef ILIGHT_HPP
#define ILIGHT_HPP

#include "Display/RGBColor.hpp"
#include "Engine/Rendering/DepthBuffer.hpp"
#include "MaterialProperties.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Vector3.hpp"

#include <functional>
#include <memory>
#include <optional>

namespace Engine
{
    namespace Rendering
    {
        namespace Lighting
        {
            using namespace Math;
            using namespace Display;

            struct Attenuation
            {
                float a;
                float b;
                float c;
            };

            class ILight
            {
            protected:
                ILight()
                {
                }

                virtual ~ILight() = default;

                [[nodiscard]] inline float GetSpecularHighlightAt(const Vector3& position, const Vector3& normal, const Vector3& cam_pos, const Vector3& light_dir, const MaterialProperties& material_properties) const
                {
                    Vector3 view_dir = cam_pos - position;
                    Vector3 half_dir = (view_dir - light_dir).GetNormalized();
                    float specular   = half_dir.GetDotProduct(normal);
                    specular         = std::pow(specular, material_properties.specular_factor);

                    return specular * material_properties.specular_intensity;
                }

            public:
                [[nodiscard]] virtual RGBColor GetColorAt(const Vector3& position, const Vector3& normal, const Vector3& cam_pos, const MaterialProperties& material_properties) const = 0;

                [[nodiscard]] virtual bool IsShadowCaster() const = 0;
                [[nodiscard]] virtual RGBColor GetColor() const   = 0;
                virtual void SetColor(RGBColor color)             = 0;

                [[nodiscard]] virtual Vector3 GetPosition() const = 0;
                virtual void SetPosition(const Vector3& position) = 0;

                // i dislike the need for the referece_wrapper here, but C++ optionals don't allow references
                [[nodiscard]] virtual std::optional<std::reference_wrapper<const Matrix4>> GetProjectionMatrix() const = 0;
                [[nodiscard]] virtual std::optional<std::reference_wrapper<const Matrix4>> GetViewMatrix() const       = 0;
                [[nodiscard]] virtual std::optional<std::reference_wrapper<DepthBuffer>> GetLightDepthBuffer()         = 0;
                [[nodiscard]] virtual std::optional<bool> IsLinearProjection() const                                   = 0;
                [[nodiscard]] virtual std::optional<float> GetBias() const                                             = 0;
                virtual void ClearDepthBuffer()                                                                        = 0;
            };
        }
    }
}

#endif
