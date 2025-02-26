#include "ObjModelLoader.hpp"
#include "Engine/Resources/Util/FileUtils.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace Engine
{
    namespace Resources
    {
        bool ObjModelLoader::LoadStaticModel(const std::string& filename, std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices, ModelLoadingOptions options)
        {
            std::ifstream file_stream(filename, std::ios::in);
            std::string line;

            if (!file_stream.is_open())
                return false;

            std::vector<std::string> line_split;

            std::vector<Vector3> normals;
            std::vector<Vector2> uvs;

            out_vertices.reserve(Util::CountLinesStartsWith(file_stream, std::string("v ")));
            out_indices.reserve(Util::CountLinesStartsWith(file_stream, std::string("f ")) * 3);
            normals.reserve(Util::CountLinesStartsWith(file_stream, std::string("vn ")));
            uvs.reserve(Util::CountLinesStartsWith(file_stream, std::string("vt ")));

            while (std::getline(file_stream, line))
            {
                line_split = Util::SplitString(line, ' ');

                if (line_split.size() > 5 || line_split.empty())
                    continue;

                if (line_split[0] == "v")
                {
                    float x = std::stof(line_split[1]);
                    float y = std::stof(line_split[2]);
                    float z = std::stof(line_split[3]);

                    out_vertices.push_back(Vertex(Vector3(x, y, z)));
                }

                if (line_split[0] == "vt")
                {
                    float u = std::stof(line_split[1]);
                    float v = std::stof(line_split[2]);

                    uvs.push_back(Math::Vector2(u, v));
                }

                if (line_split[0] == "vn")
                {
                    float x = std::stof(line_split[1]);
                    float y = std::stof(line_split[2]);
                    float z = std::stof(line_split[3]);

                    normals.push_back(Vector3(x, y, z));
                }

                if (line_split[0] == "f")
                {
                    // each face value can have textures or normal indices, we're only interested in the vertices (first before /)
                    // - 1 for 0 based array indices
                    std::vector<std::string> face0 = Util::SplitString(line_split[1], '/');
                    std::vector<std::string> face1 = Util::SplitString(line_split[2], '/');
                    std::vector<std::string> face2 = Util::SplitString(line_split[3], '/');

                    uint64_t slashes   = std::count(line_split[1].begin(), line_split[1].end(), '/');
                    bool has_empty_mid = line_split[1].find("//") != std::string::npos;

                    uint32_t v0 = std::stoul(face0[0]) - 1;
                    uint32_t v1 = std::stoul(face1[0]) - 1;
                    uint32_t v2 = std::stoul(face2[0]) - 1;

                    bool has_uv  = false;
                    uint32_t uv0 = 0;
                    uint32_t uv1 = 0;
                    uint32_t uv2 = 0;

                    bool has_normal = false;
                    uint32_t norm0  = 0;
                    uint32_t norm1  = 0;
                    uint32_t norm2  = 0;

                    switch (slashes)
                    {
                    case 1:
                        has_uv = true;
                        uv0    = std::stoul(face0[1]) - 1;
                        uv1    = std::stoul(face1[1]) - 1;
                        uv2    = std::stoul(face2[1]) - 1;
                        break;
                    case 2:
                        if (!has_empty_mid)
                        {
                            has_uv = true;
                            uv0    = std::stoul(face0[1]) - 1;
                            uv1    = std::stoul(face1[1]) - 1;
                            uv2    = std::stoul(face2[1]) - 1;

                            has_normal = true;
                            norm0      = std::stoul(face0[2]) - 1;
                            norm1      = std::stoul(face1[2]) - 1;
                            norm2      = std::stoul(face2[2]) - 1;
                        }
                        else
                        {
                            has_normal = true;
                            norm0      = std::stoul(face0[1]) - 1;
                            norm1      = std::stoul(face1[1]) - 1;
                            norm2      = std::stoul(face2[1]) - 1;
                        }
                        break;
                    }

                    out_indices.push_back(v0);
                    out_indices.push_back(v1);
                    out_indices.push_back(v2);

                    if (has_uv)
                    {
                        out_vertices[v0].SetTextureCoords(uvs[uv0]);
                        out_vertices[v1].SetTextureCoords(uvs[uv1]);
                        out_vertices[v2].SetTextureCoords(uvs[uv2]);
                    }
                    if (has_normal)
                    {
                        out_vertices[v0].SetNormal(normals[norm0]);
                        out_vertices[v1].SetNormal(normals[norm1]);
                        out_vertices[v2].SetNormal(normals[norm2]);
                    }
                }
            }

            file_stream.close();

            switch (options.normal_options)
            {
            case GenerationCondition::ALWAYS:
                Vertex::CalculateNormals(out_vertices, out_indices);
                break;

            case GenerationCondition::IF_MISSING:
                if (normals.empty())
                    Vertex::CalculateNormals(out_vertices, out_indices);
                break;

            case GenerationCondition::NEVER:
            default:
                break;
            }

            // obj does not support tangents
            if (options.tangent_options == GenerationCondition::ALWAYS || options.tangent_options == GenerationCondition::IF_MISSING)
                Vertex::CalculateTangents(out_vertices, out_indices);

            return true;
        }

        bool ObjModelLoader::LoadAnimatedModel(const std::string& filename, std::vector<Frame>& out_frames, std::vector<uint32_t>& out_indices, std::map<std::string, Animation>& out_animations, ModelLoadingOptions options)
        {
            return false;
        }
    }
}
