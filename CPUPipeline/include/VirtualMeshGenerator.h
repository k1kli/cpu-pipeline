#pragma once
#include "Mesh.h"
#include <string>
#include <memory>
#include "SaveableObject.h"
#include "MeshType.h"
class Mesh;
class VirtualMeshGenerator : public SaveableObject
{
public:
	VirtualMeshGenerator(std::vector<std::string> parameterNames, std::string meshName)
		:parameterNames(parameterNames), meshName(meshName) {}
	Mesh getMesh();
	const std::vector<std::string>& getParameterNames() const;
	const std::vector<float>& getParameterValues() const;
	virtual std::shared_ptr<VirtualMeshGenerator> getInstance() const = 0;
	virtual MeshType getType() const = 0;
	const std::string& getName() const;
	void setParameters(std::vector<float> parameters);
	static VirtualMeshGenerator* loadStatic(SceneDataReader& reader);
	void load(SceneDataReader& reader);
	void save(SceneDataWriter& writer) const;
	virtual ~VirtualMeshGenerator() {}
private:

	Mesh * resMesh = nullptr;

protected:
	std::vector<float> parameters;
	std::vector<std::string> parameterNames;
	std::string meshName;
	virtual void validateParameters() = 0;
	virtual void resizeVectors() = 0;
	virtual void setValues() = 0;
	virtual void setTriangles() = 0;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::uvec3> triangles;
	std::vector<glm::uvec3> trianglesNormals;
	std::vector<glm::vec2> uv;
	std::vector<glm::uvec3> trianglesUV;
};

