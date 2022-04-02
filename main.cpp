
#include <GL/glew.h> 
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <cmath>
#include <thread>
#include <chrono>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <MeshLoader.hpp>
#include <mesh.hpp>
#include <meshAttributes.hpp>
#include <shader.hpp>
#include <camera.hpp>
/*
// 🍝
extern "C" char _binary_vertex_glsl_start;
extern "C" char _binary_vertex_glsl_end;

extern "C" char _binary_fragment_glsl_start;
extern "C" char _binary_fragment_glsl_end;
*/

struct smth {
	vertex2D v2;
	uint32_t something;
};

int main() {
	smth arr[3];

	constexpr auto sizeElem = sizeof(smth);
	constexpr auto sizeArr = sizeof(arr);

	int width = 1280;
	int height = 720;

    sf::Window window(sf::VideoMode(width, height), "FPS3D", sf::Style::Default);
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	if (glewInit() != GLEW_OK) {
		std::cout << "glew initialization failed. Shuting down" << std::endl;
		return -1;
	}
	
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	window.display();
	/*
	shader shader = shader::shaderFromOBJ(
		&_binary_vertex_glsl_start, &_binary_vertex_glsl_end,
		&_binary_fragment_glsl_start, &_binary_fragment_glsl_end
	);
	*/
	shader shader = shader::loadShader(
		"/home/zy4n/Documents/Code/C++/FPS3D/shaders/vertex.glsl",
		"/home/zy4n/Documents/Code/C++/FPS3D/shaders/fragment.glsl"
	);

	float scale = 1;

	const auto updateProjection = [&](){
		static constexpr float halfPi = M_PI / 2.f;
		shader.set("projectionMat", glm::perspective(
			halfPi / scale,
			(float)width / height,
			0.1f, 100000.0f
		));	
	};

	updateProjection();
	
	camera player({ 0, 0, 0 }, { 0, 0, 1 } , { 0, 1, 0 });

	
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	shader.set("colorMerge", 0.0f);
	shader.set("meshColor", glm::fvec4{ 1.0f, 0.0f, 1.0f, 1.0f });
	//shader.set("rayDirection", glm::fvec3{ 1.0f, 1.0f, 1.0f});

	std::vector<mesh> meshes;

	
	//MeshLoader::loadFromOBJ("/home/zy4n/3D/GTA/untitled.obj", meshes);
	MeshLoader::loadFromOBJ("/home/zy4n/3D/amongusOBJ/amongus.obj", meshes);
	//MeshLoader::loadFromOBJ("/home/zy4n/3D/totem.obj", meshes);
	//MeshLoader::loadFromOBJ("/home/zy4n/3D/the-utah-teapot/source/toll/teapot.obj", meshes);
	//MeshLoader::loadFromOBJ("/home/zy4n/3D/box.obj/untitled.obj", meshes);
	//MeshLoader::loadFromOBJ("/home/zy4n/3D/test.obj", meshes);
	
	static constexpr float modelScale = 5.0f;
	std::vector<meshInstance> renderables;
	for (const mesh& m : meshes) {
		renderables.push_back(m.getInstance(
			glm::scale(
				glm::identity<glm::mat4x4>(),
				{ modelScale, modelScale, modelScale }
			)
		));
	}

	std::cout << "finished loading mesh\n";

	constexpr auto FPS = 60;
	constexpr auto frameTime = std::chrono::microseconds(int(1000000.0f / FPS));
	constexpr float dt = frameTime.count() / 1000.f;

    bool running = true;
	bool lockMouse = true;
	window.setMouseCursorVisible(!lockMouse);

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
				scale = std::max(scale * (1 + (event.mouseWheel.delta > 0 ? 0.1f : -0.1f)), 1.f);
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
			shader.set("viewMat", player.getViewMatrix());
		}

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& m : renderables) 
			m.render(shader);

        window.display();

		const auto finish = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(frameTime - (finish - start));
    }
	
    return 0;
}
