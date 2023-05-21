#ifndef CAMERA_HH
#define CAMERA_HH

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct camera {
	struct move { enum t {
		forward, backward, right, left, up, down, rotateright, rotateleft, rotateup, rotatedown, zoomin, zoomout, count
	}; };

	glm::vec3 worldup;

	glm::vec3 position;
	float yaw;
	float pitch;
	float fov;

	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float moveSpeed;
	float rotateSpeed;
	float zoomSpeed;

	void (*moving)(bool[]) {nullptr};

	/* world up x, y, z, position x, y,z, xz-plane yaw, xy-plane pitch, fov, move speed, rotate speed, zoom speed */
	camera(float, float, float, float, float, float, float, float, float, float, float, float);

	void set_move_callback(void (*)(bool[]));
	void update(float);

	glm::mat4 GetViewMatrix();
};

#endif
