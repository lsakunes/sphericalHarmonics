#ifndef KEYBOARD_MOVEMENT_CONTROLLER_HPP
#define KEYBOARD_MOVEMENT_CONTROLLER_HPP
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "sve_game_object.hpp"
#include "sve_window.hpp"

namespace sve {
	class KeyboardMovementController {
	public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
            int lightCW = GLFW_KEY_X;
            int lightCCW = GLFW_KEY_Z;
        };

        void moveInPlaneXZ(GLFWwindow* window, float dt, SveGameObject& gameObject);
        void rotateLight(GLFWwindow* window, float dt, glm::vec2& lightDirection);

        KeyMappings keys{};
        float moveSpeed{ 3.f };
        float lightSpeed{ 5.f };
        float lookSpeed{ 1.5f };

    private:
        float getRotationOnUnitCircleFromPoint(glm::vec2 point);
        glm::vec2 getPointOnUnitCircleFromRotation(float angle);
	};

}

#endif