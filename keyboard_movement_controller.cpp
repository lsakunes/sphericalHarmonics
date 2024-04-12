#include "keyboard_movement_controller.hpp"

namespace sve {
	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, SveGameObject& gameObject) {
		glm::vec3 rotate{0};
		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
		}

		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir(sin(yaw), 0.f, cos(yaw));
		const glm::vec3 rightDir(forwardDir.z, 0.f, -forwardDir.x);
		const glm::vec3 upDir(0.f, -1.f, 0.f);

		glm::vec3 moveDir(0.f);
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir += -rightDir;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir += -upDir;

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
			gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
		}
	}
	void KeyboardMovementController::rotateLight(GLFWwindow* window, float dt, glm::vec2& lightDirection) {
		float rotation = getRotationOnUnitCircleFromPoint(lightDirection);
		float rotationChange = 0;
		if (glfwGetKey(window, keys.lightCW) == GLFW_PRESS) rotationChange += 1;
		if (glfwGetKey(window, keys.lightCCW) == GLFW_PRESS) rotationChange -= 1;
		
		rotationChange *= lightSpeed * dt;
		rotation += rotationChange;
		rotation = glm::mod(rotation, glm::two_pi<float>());
		glm::vec2 point = getPointOnUnitCircleFromRotation(rotation);
		lightDirection.x = point.x;
		lightDirection.y = point.y;
	}

	float KeyboardMovementController::getRotationOnUnitCircleFromPoint(glm::vec2 point){
		float angle = atan2(point.y, point.x);
		return angle;
	}

	glm::vec2 KeyboardMovementController::getPointOnUnitCircleFromRotation(float angle){
		return glm::vec2(cos(angle), sin(angle));
	}

}