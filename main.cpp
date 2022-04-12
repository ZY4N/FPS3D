#define MAIN

#ifdef MAIN

#include <GL/glew.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <geometry/MeshLoaderImpl.hpp>
#include <geometry/meshImpl.hpp>
#include <geometry/meshes.hpp>

#include <graphics/meshEffects.hpp>
#include <graphics/shaderImpl.hpp>
#include <graphics/camera.hpp>

#include <utils/arg_parser.hpp>

// 🍝
extern "C" char _binary_vertex_glsl_start;
extern "C" char _binary_vertex_glsl_end;

extern "C" char _binary_fragment_glsl_start;
extern "C" char _binary_fragment_glsl_end;


using defaultMesh = mesh<vertex_comps::texCoord, vertex_comps::normal>;
using defaultShader = shader<"projectionMat", "viewMat", "modelMat", "colorMerge", "meshColor">;

constexpr std::string_view meshFiles[] {
	"/home/zy4n/3D/GTA/untitled.obj",							// 0
	"/home/zy4n/3D/amongusOBJ/amongus.obj",						// 1
	"/home/zy4n/3D/totem.obj",									// 2
	"/home/zy4n/3D/the-utah-teapot/source/toll/teapot.obj",		// 3
	"/home/zy4n/3D/shapes/cube.obj",							// 4
	"/home/zy4n/3D/shapes/icosahedron.obj",						// 5
	"/home/zy4n/3D/test.obj"									// 6
};


int main(int numArgs, char* args[]) {

	arg_parser input(numArgs, args);

	int meshIndex = 0;
	input.parse("mesh", false, meshIndex);

 
	//----------------------[ window + opengl setup ]----------------------//

	int width = 1280;
	int height = 720;

	sf::Window window(sf::VideoMode(width, height), "FPS3D", sf::Style::Default, sf::ContextSettings(24, 8, 2, 4, 6));
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	if (glewInit() != GLEW_OK) {
		std::cout << "glew initialization failed. Shuting down" << std::endl;
		return -1;
	}

	//glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback([](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam
		) {
			fprintf(
				stderr, "GL_MSG: %s type: 0x%x, severity: 0x%x, msg: %s\n",
				(type == GL_DEBUG_TYPE_ERROR ? "ERROR" : "" ),
				type, severity, message
			);
		},
		0
	);
	
	std::cout << "using: " << glGetString(GL_RENDERER) << std::endl;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);
	glClearDepth(1.f);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window.display();

	glClearColor(0.f, 0.f, 0.f, 0.f);

	
	//----------------------[ shader setup ]----------------------//
	defaultShader shader = defaultShader::createShader(
		&_binary_vertex_glsl_start, (&_binary_vertex_glsl_end - &_binary_vertex_glsl_start) - 1,
		"", 0,
		&_binary_fragment_glsl_start, (&_binary_fragment_glsl_end - &_binary_fragment_glsl_start) - 1
	);
	
	/*
	defaultShader shader = defaultShader::loadShader(
		"/home/zy4n/Documents/Code/C++/FPS3D/shaders/vertex.glsl",
		"",
		"/home/zy4n/Documents/Code/C++/FPS3D/shaders/fragment.glsl"
	);
	*/
	

	float scale = 1;
	const auto updateProjection = [&](){
		static constexpr float halfPi = M_PI / 2.f;
		shader.set<"projectionMat">(glm::perspective(
			halfPi / scale,
			(float)width / height,
			0.1f, 1000.0f
		));	
	};

	updateProjection();

	// not a great solution but works for now... 
	shader.set<"colorMerge">(0.0f);
	shader.set<"meshColor">(glm::fvec4{ 1.0f, 0.0f, 1.0f, 1.0f });


	//----------------------[ mesh loading ]----------------------//

	std::vector<defaultMesh> meshes;
	std::vector<material> materials;

	const auto time = []<typename F>(F&& f) {
		const auto start = std::chrono::high_resolution_clock::now();
		f();
		const auto end = std::chrono::high_resolution_clock::now();
		std::cout << (end - start).count() << "ns" << std::endl;
	};

	//time([&]() { MeshLoader::loadFromOBJ(std::string(meshFiles[meshIndex]), meshes, materials); });


	glm::vec3 min{  FLT_MAX,  FLT_MAX,  FLT_MAX };
	glm::vec3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for (const auto& m : meshes) {
		auto [newMin, newMax] = m.getBoundingBox();
		min = glm::min(min, newMin);
		max = glm::max(max, newMax);
	}
	constexpr glm::vec3 outerBox{ 10, 10, 10 };

	const auto dim = max - min;
	std::cout << "map size: " << dim.x << ' ' << dim.y << ' ' << dim.z << std::endl;

	glm::vec3 mapScale{
		std::abs(dim.x) < glm::epsilon<float>() ? 1 : (outerBox.x / dim.x),
		std::abs(dim.y) < glm::epsilon<float>() ? 1 : (outerBox.y / dim.y),
		std::abs(dim.z) < glm::epsilon<float>() ? 1 : (outerBox.z / dim.z),
	};

	float modelScale = 5; //std::min(std::min(mapScale.x, mapScale.y), mapScale.z);

	const auto transform = glm::scale(
		glm::identity<glm::mat4x4>(),
		{ modelScale, modelScale, modelScale }
	);
	std::vector<renderable> renderables;
	for (defaultMesh& m : meshes) {
		m.initVAO();
		renderables.push_back(m.getRenderable(transform));
	}

	meshColor c({ 1, 0, 0, 1});

	meshes::sphere1X.initVAO();
	renderables.push_back(meshes::sphere1X.getRenderable(transform));
	meshes::sphere2X.initVAO();
	renderables.push_back(meshes::sphere2X.getRenderable(transform));
	
	meshes::sphere3X.initVAO();
	renderables.push_back(meshes::sphere3X.getRenderable(transform));

	for (auto& m : renderables) {
		m.myColor = &c;
	}

	std::cout << "finished loading mesh\n";


	//----------------------[ game loop ]----------------------//

	camera player({ 12, 0, 12 }, { 0, 0, 1 } , { 0, 1, 0 });

	constexpr auto FPS = 60;
	constexpr auto frameTime = std::chrono::microseconds(int(1000000.0f / FPS));
	constexpr float dt = frameTime.count() / 1000.f;

	bool running = true;
	bool lockMouse = true;
	window.setMouseCursorVisible(!lockMouse);

	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	while (running) {
		const auto start = std::chrono::high_resolution_clock::now();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)  {
				running = false;
			} else if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
				width = event.size.width;
				height = event.size.height;
				updateProjection();
			} else if (event.type == sf::Event::MouseWheelMoved) {
				scale = std::max(scale * (1.0f + 0.1f * event.mouseWheel.delta), 1.f);
				updateProjection();
			} else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape: running = false; break;
					case sf::Keyboard::Tab: window.setMouseCursorVisible(!(lockMouse ^= 1)); break;
					default: break;
				}
			}
		}

		if (lockMouse) {
			const int middleX = width / 2;
			const int middleY = height / 2;
			const auto mouseDelta = sf::Mouse::getPosition(window);
			sf::Mouse::setPosition({ middleX, middleY }, window);
			player.update(dt, mouseDelta.x - middleX, mouseDelta.y - middleY);
			shader.set<"viewMat">(player.getViewMatrix());
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& m : renderables) 
			m.render(shader);

		window.display();

		const auto finish = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(frameTime - (finish - start));
	}
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	return 0;
}

#else

#include <iostream>
#include <utils/intX.hpp>
#include <geometry/meshes.hpp>
#include <glm/vec3.hpp>
#include <array>


int main() {

	for (u32 i = 1; i < 10; i++) {
		std::cout << "predictEdgeVertexCount: " << meshes::predictEdgeVertexCount(i) << std::endl;
		std::cout << "predictEdgeCount:" << meshes::predictEdgeCount(i) << std::endl;
		std::cout << "predictVertexCount: " << meshes::predictVertexCount(i) << std::endl;
	}

	/*
	auto vertexBuffer = meshes::icosahedron.getVertexBuffer();
	auto indexBuffer = meshes::icosahedron.getIndexBuffer();
	static constexpr float epsilon = 0.01;

	for (ssize_t i = 0; i < vertexBuffer.size(); i++) {
		const auto& aPos = vertexBuffer[i].get<0>();
		const auto& aTex = vertexBuffer[i].get<1>();
		bool reset = false;
		for (ssize_t j = vertexBuffer.size() - 1; j >= 0; j--) {
			if (i == j) continue;

			const auto& bPos = vertexBuffer[j].get<0>();
			const auto& bTex = vertexBuffer[j].get<1>();

			const auto delta = aPos - bPos;
			const auto deltaT = aTex - bTex;

			if (
				std::abs(delta.x) <= epsilon &&
				std::abs(delta.y) <= epsilon &&
				std::abs(delta.z) <= epsilon &&
				std::abs(deltaT.x) <= epsilon &&
				std::abs(deltaT.y) <= epsilon
			) {
				vertexBuffer.erase(vertexBuffer.begin() + j);	
				for (u16& index : indexBuffer) {
					if (index == j) {
						index = i;
					} else if (index > j) {
						index--;
					}
				}
				reset = true;
			}
		}
		if (reset) {
			i = 0;
		}
	}



	for (const auto& vertex : vertexBuffer) {
		const float* numbers = (const float*)&vertex;
		std::cout << "{ { ";
		for (size_t j = 0; j < 3; j++) {
			std::cout << numbers[j] << ", ";
		}
		std::cout << "}, { ";

		for (size_t j = 0; j < 2; j++) {
			std::cout << numbers[3 + j] << ", ";
		}

		std::cout << "}, { ";

		for (size_t j = 0; j < 3; j++) {
			std::cout << numbers[5 + j] << ", ";
		}
		std::cout << "} },\n"; 
	}

	for (u16 index : indexBuffer) {
		std::cout << index << ", ";
	}
	std::cout << std::endl;


	std::cout << "verts " << vertexBuffer.size() << " " << meshes::icosahedron.getVertexBuffer().size() << std::endl;
	std::cout << "idxs " << indexBuffer.size() << " " << meshes::icosahedron.getIndexBuffer().size() << std::endl;
	*/
}

#endif
