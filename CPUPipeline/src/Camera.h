#pragma once
#include <glm/glm.hpp>
#include "../SaveableObject.h"
#include "../SceneDataReader.h"
#include "../SceneDataWriter.h"

class Camera : public SaveableObject
{
public:
	Camera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up);
	Camera(SceneDataReader& reader) { load(reader); };
	~Camera() = default;

	glm::mat4 GetViewMatrix() const { return m_world_matrix; }
	glm::mat4 GetProjectionMatrix() const { return m_projection_matrix; };
	glm::mat4 GetViewportMatrix() const { return m_viewport_matrix; };
	float GetNearPlane() const { return m_nearPlane; }
	float GetFarPlane() const { return m_farPlane; }
	float GetFov() const { return m_fov; }

	void SetPerspective(float fov, float aspect, float near, float far);
	void SetViewport(float offset_x, float offset_y, float screen_width, float screen_height);

	void LookAt(glm::vec3 pos, glm::vec3 front, glm::vec3 up);

	const glm::vec3& GetPosition() const;

	const glm::vec3& GetUp() const;

	const glm::vec3& GetForward() const;
	void save(SceneDataWriter& writer) const;
	void load(SceneDataReader& reader);

private:
	glm::vec3 m_position;
	glm::vec3 m_up_vector;
	glm::vec3 m_forward_vector;

	float m_nearPlane;
	float m_farPlane;
	float m_fov;
	float m_aspect;

	glm::mat4 m_world_matrix;
	glm::mat4 m_projection_matrix;
	glm::mat4 m_viewport_matrix;
};

