module;

#include <GL/glfw3.h>
#include <assimp/scene.h>

export module Graphics:Renderer;

import Common;
import Graphics.Components;
import Graphics.Resources;

import <glm/mat4x4.hpp>;

import <string>;
import <memory>;
import <vector>;
import <thread>;
import <unordered_map>;

export class Renderer
{
	friend class Window;

public:
	~Renderer();

	void RenderAndDisplayScene();

	void RenderFrame();
	void RenderShadows(std::shared_ptr<LightSourceComponent> p_caster);
	void RenderFrame(ShaderProgram& p_shader);
	void RenderWireframe();

	void DisplayScene();

	Camera& GetMainCamera();

	void SetBackgroundColor(float p_red, float p_green, float p_blue, float p_alpha = 1.f);
	void SetSkybox(const std::string& p_frontPath,
				   const std::string& p_rightPath,
				   const std::string& p_leftPath,
				   const std::string& p_topPath,
				   const std::string& p_bottomPath,
				   const std::string& p_backPath);

	void AddObject(const Entity& p_object);
	void AddLightSource(const Entity& p_object);
	void SetShadowCaster(const Entity& p_object);

	void Set2DMode(float p_width, float p_height);
	void SetPerspective(float p_fov, float p_nearPlane, float p_farPlane);

	void LoadModel(ModelComponent& p_model, const std::string& p_path, bool p_flipUVs = true);

	/*********************************************************************/

	std::shared_ptr<ShaderProgram> GenerateShader(const std::string& p_vertexShaderPath,
												  const std::string& p_fragmentShaderPath,
												  const std::string& p_geometryShaderPath = "");

	std::shared_ptr<Texture2D> GenerateTexture2D(const std::string& p_texturePath, const bool p_repeat = true);
	std::shared_ptr<Cubemap> GenerateCubemap(const std::string& p_frontPath,
											 const std::string& p_rightPath,
											 const std::string& p_leftPath,
											 const std::string& p_topPath,
											 const std::string& p_bottomPath,
											 const std::string& p_backPath);

	std::shared_ptr<Mesh> GenerateMesh(const std::string& p_name,
									   const std::vector<Vertex>& p_vertices = {},
									   const std::vector<unsigned int>& p_indices = {},
									   const std::shared_ptr<Material>& p_material = nullptr,
									   bool p_genNormal = false);

	std::shared_ptr<ShaderProgram> DefaultShader();

private:
	Renderer(GLFWwindow* p_window);

	void DrawSkybox();
	void SetShadowVariables(ShaderProgram& p_shader);
	void ProcessAssimpNode(aiNode* p_node, const aiScene* p_scene, const std::string& p_path, ModelComponent& p_model);

	void LockCamera(bool p_lock);

	/*********************************************************************/

	GLFWwindow* _window;
	std::shared_ptr<Mesh> GenerateMesh(aiMesh* mesh, const aiScene* scene, const std::string& p_name);

	std::vector<std::weak_ptr<ModelComponent>> _models;
	Entity _skybox;

	std::vector<std::weak_ptr<LightSourceComponent>> _lightSources;
	Entity _defaultLight;

	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _shaders;
	std::shared_ptr<ShaderProgram> _defaultShader;
	std::shared_ptr<ShaderProgram> _wireframeShader;

	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> _meshes;

	std::weak_ptr<LightSourceComponent> _shadowCaster;
	std::shared_ptr<ShaderProgram> _shadowsShader;
	std::unique_ptr<FrameBuffer> _shadowBuffer;
	static unsigned int _shadowWidth, _shadowHeight;


	//TODO: add to camera?
	glm::mat4 _projectionMatrix;
	//TODO: multicamera?
	Camera _mainCamera;

	double _lastTime;
	float _deltaTime;
	double _fpsDelta;
	unsigned int _frames;
	bool _cameraLock;
};