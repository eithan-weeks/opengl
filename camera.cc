#include "camera.hh"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

static void pitch_constraint(float& pitch) {
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

static void fov_constraint(float& fov) {
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 135.0f)
		fov = 135.0f;
}

static void updateCameraVectors(
	glm::vec3 const& worldup,
	float const yaw,
	float const pitch,
	glm::vec3& front,
	glm::vec3& up,
	glm::vec3& right
) {
	front = glm::normalize(glm::vec3(
		std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch)),
		std::sin(glm::radians(pitch)),
		std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch))
	));
	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));
}

camera::camera(
	float ux_, float uy_, float uz_,
	float px_, float py_, float pz_,
	float yaw_, float pitch_,
	float fov_,
	float moveSpeed_, float rotateSpeed_, float zoomSpeed_
) :
	worldup {glm::normalize(glm::vec3(ux_, uy_, uz_))},
	position {px_, py_, pz_},
	yaw {yaw_},
	pitch {pitch_},
	fov {fov_},
	moveSpeed {moveSpeed_},
	rotateSpeed {rotateSpeed_},
	zoomSpeed {zoomSpeed_}
{
	pitch_constraint(pitch);
	fov_constraint(fov);
	updateCameraVectors(worldup, yaw, pitch, front, up, right);
}

void camera::set_move_callback(void (*moving_)(bool[])) {
	moving = moving_;
}

void camera::update(float deltatime) {
	if (!moving)
		return;
	bool state[move::count];
	moving(state);
	float const moveVelocity {moveSpeed * deltatime};
	float const rotateVelocity {rotateSpeed * deltatime};
	float const zoomVelocity {zoomSpeed * deltatime};
	bool vector_change {false};
	bool pitch_change {false};
	bool fov_change {false};
	if (state[move::forward])
		position += front * moveVelocity;
	if (state[move::backward])
		position -= front * moveVelocity;
	if (state[move::right])
		position += right * moveVelocity;
	if (state[move::left])
		position -= right * moveVelocity;
	if (state[move::up])
		position += up * moveVelocity;
	if (state[move::down])
		position -= up * moveVelocity;
	if (state[move::rotateright]) {
		yaw += rotateVelocity;
		vector_change = true;
	}
	if (state[move::rotateleft]) {
		yaw -= rotateVelocity;
		vector_change = true;
	}
	if (state[move::rotateup]) {
		pitch += rotateVelocity;
		vector_change = true;
		pitch_change = true;
	}
	if (state[move::rotatedown]) {
		pitch -= rotateVelocity;
		vector_change = true;
		pitch_change = true;
	}
	if (state[move::zoomin]) {
		fov -= zoomVelocity;
		fov_change = true;
	}
	if (state[move::zoomout]) {
		fov += zoomVelocity;
		fov_change = true;
	}
	if (fov_change)
		fov_constraint(fov);
	if (pitch_change)
		pitch_constraint(pitch);
	if (vector_change)
		updateCameraVectors(worldup, yaw, pitch, front, up, right);
}

glm::mat4 camera::GetViewMatrix() {
	return glm::lookAt(position, position + front, up);
}
