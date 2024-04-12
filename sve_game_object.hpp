#ifndef SVE_GAME_OBJECT_HPP
#define SVE_GAME_OBJECT_HPP

#include "sve_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>

namespace sve {

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

    class SveGameObject {
    public:
        using id_t = unsigned int;

        static SveGameObject createGameObject() {
            static id_t currentId = 0;
            return SveGameObject{ currentId++ };
        }

        SveGameObject(const SveGameObject&) = delete;
        SveGameObject& operator=(const SveGameObject&) = delete;
        SveGameObject(SveGameObject&&) = default;
        SveGameObject& operator=(SveGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<SveModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};

    private:
        SveGameObject(id_t objId) : id{ objId } {}

        id_t id;
    };
}  // namespace sve

#endif