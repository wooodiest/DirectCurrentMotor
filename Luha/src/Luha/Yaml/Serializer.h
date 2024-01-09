#pragma once

#include <yaml-cpp/yaml.h>
#include "imgui.h"
#include <glm/glm.hpp>

namespace YAML {

    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs);
        static bool decode(const Node& node, glm::vec2& rhs);
    };

    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs);
        static bool decode(const Node& node, glm::vec3& rhs);
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs);
        static bool decode(const Node& node, glm::vec4& rhs);
    };

    template<>
    struct convert<ImVec2>
    {
        static Node encode(const ImVec2& rhs);
        static bool decode(const Node& node, ImVec2& rhs);
    };

    template<>
    struct convert<ImVec4>
    {
        static Node encode(const ImVec4& rhs);
        static bool decode(const Node& node, ImVec4& rhs);
    };

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec2& v);
    YAML::Emitter& operator<<(YAML::Emitter& out, const ImVec4& v);

}








