#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Camera::Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up)
{
	LookAt(pos, forward, up);
}



void Camera::SetPerspective(float fov_y, float aspect, float near, float far)
{
	m_nearPlane = near;
	m_farPlane = far;
	m_fov = fov_y;
	float e = 1 / tan(glm::radians(fov_y)/2);
	/*float aaa[16] = {
	   e, 0, 0, 0,
	   0, e/aspect, 0, 0,
	   0, 0, -(far+near)/(far-near), -2*far*near/(far-near),
	   0, 0, -1, 0
	};*/
	m_projection_matrix[0][0] = e;
	m_projection_matrix[0][1] = 0;
	m_projection_matrix[0][2] = 0;
	m_projection_matrix[0][3] = 0;

	m_projection_matrix[1][0] = 0;
	m_projection_matrix[1][1] = e / aspect;
	m_projection_matrix[1][2] = 0;
	m_projection_matrix[1][3] = 0;

	m_projection_matrix[2][0] = 0;
	m_projection_matrix[2][1] = 0;
	m_projection_matrix[2][2] = -(far + near) / (far - near);
	m_projection_matrix[2][3] = -1.0f;

	m_projection_matrix[3][0] = 0;
	m_projection_matrix[3][1] = 0;
	m_projection_matrix[3][2] = -2.0f * far * near / (far - near);
	m_projection_matrix[3][3] = 0;
	/*m_projection_matrix = glm::transpose(glm::mat4(
		e, 0, 0, 0,
		0, e / aspect, 0, 0,
		0, 0, -(far + near) / (far - near), -2 * far * near / (far - near),
		0, 0, -1, 0));*/
	//memcpy(glm::value_ptr(m_projection_matrix), aaa, sizeof(aaa));
}

void Camera::SetViewport(float offset_x, float offset_y, float screen_width, float screen_height)
{
	m_viewport_matrix[0][0] = screen_width/2;
	m_viewport_matrix[0][1] = 0;
	m_viewport_matrix[0][2] = 0;
	m_viewport_matrix[0][3] = 0;

	m_viewport_matrix[1][0] = 0;
	m_viewport_matrix[1][1] = screen_height/2;
	m_viewport_matrix[1][2] = 0;
	m_viewport_matrix[1][3] = 0;

	m_viewport_matrix[2][0] = 0;
	m_viewport_matrix[2][1] = 0;
	m_viewport_matrix[2][2] = 0.5;
	m_viewport_matrix[2][3] = 0;

	m_viewport_matrix[3][0] = offset_x + screen_width/2;
	m_viewport_matrix[3][1] = offset_y + screen_height / 2;
	m_viewport_matrix[3][2] = 0.5;
	m_viewport_matrix[3][3] = 1;
}

void Camera::LookAt(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
	m_position = pos;
	m_up_vector = up;
	m_forward_vector = front;
	glm::vec3 F = glm::normalize(-front);
	glm::vec3 R = glm::normalize(glm::cross(up, F));
	glm::vec3 U = glm::normalize(glm::cross(F, R));
	m_world_matrix = glm::transpose(glm::mat4(
		R.x, R.y, R.z, 0,
		U.x, U.y, U.z, 0,
		F.x, F.y, F.z, 0,
		0, 0, 0, 1
	)) * glm::transpose(glm::mat4(
		1, 0, 0, -m_position.x,
		0, 1, 0, -m_position.y,
		0, 0, 1, -m_position.z,
		0, 0, 0, 1
	));
}

const glm::vec3& Camera::GetPosition() const
{
	return m_position;
}
const glm::vec3& Camera::GetUp() const
{
	return m_up_vector;
}
const glm::vec3& Camera::GetForward() const
{
	return m_forward_vector;
}
