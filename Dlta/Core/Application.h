#pragma once

namespace dlta {
	using namespace glm;

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "Glfw Error %d: %s\n", error, description);
	}

	void resize_framebuffer(GLFWwindow* window, int width, int height);

	class Application {
	public:
		void (*update_loop)() = NULL;
		//Transform cam;
		GLFWwindow* window;
		Texture tex;
		Texture tex2;
		Skybox skybox;
		Shader errorShader;
		Shader testShader;
		Shader testShader2;
		vector<Entity*> entities;
		vector<MeshRenderer*> renderers;
		vector<SpriteRenderer*> renderers2d;
		postprocessing::EffectStack postProcessEffects;
		int width;
		int height;
		Renderer renderer;

		Application(int width, int height, const std::string& title = "engine", bool postprocessing = true) {
			this->width = width;
			this->height = height;

			glfwSetErrorCallback(glfw_error_callback);

			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

			string windowTitle = "Dlta editor - " + title;
			window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

			if (window == NULL) {
				glfwTerminate();
				panic("Failed to create GLFW window");
			}

			glfwMakeContextCurrent(window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) panic("Failed to initialize GLAD");

			glViewport(0, 0, width, height);

			//glEnable(GL_MULTISAMPLE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// lock the framerate to screen refresh rate
			glfwSwapInterval(1);

			glfwSetWindowUserPointer(window, this);

			// hide and center cursor
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			glfwSetFramebufferSizeCallback(window, resize_framebuffer);
			glfwSetKeyCallback(window, Input::key_change);
			glfwSetMouseButtonCallback(window, Input::mouse_change);
			glfwSetScrollCallback(window, Input::scroll_change);

			errorShader = Shader("assets/shaders/error.vert", "assets/shaders/error.frag");
			testShader = Shader("assets/shaders/test.vert", "assets/shaders/test.frag");
			testShader2 = Shader("assets/shaders/test2.vert", "assets/shaders/test2.frag");

			Gizmos::init();
			Gizmos::shader = Shader("assets/shaders/gizmo.vert", "assets/shaders/gizmo.frag");
			/*tex = Texture::load("assets/container.jpg");
			tex2 = Texture::load("assets/icon.png", false, false, false);

			tex.bind(0);
			tex2.bind(1);*/

#if DLTA_EDITOR
			renderer = Renderer(width, height, false, postprocessing);
#else
			renderer = Renderer(width, height, true, postprocessing);
#endif

			// register editor windows
#if DLTA_EDITOR
			new StatOverlay();
			new EffectEditor(postProcessEffects);
			new GameView(renderer.screen.color.id);
#endif

			// Initialize input
			// this ensures the mouse delta becomes zero on the first frame
			Input::Update(window, vec2(width, height));
		}

		void run() {

#if DLTA_EDITOR
			EditorGUI::Init(window);
#endif

			for (auto entity : entities) {
				entity->start();
			}

			while (!glfwWindowShouldClose(window)) {

				Time::Update(glfwGetTime());
				Input::Update(window, vec2(width, height));

				if (update_loop != NULL) {
					update_loop();
				}

				for (auto entity : entities) {
					entity->update();
				}

				render();
			}

#if DLTA_EDITOR
			EditorGUI::Cleanup();
#endif
			glfwDestroyWindow(window);
			glfwTerminate();
		}

		float rand01() {
			return ((float)rand()) / (float)RAND_MAX;
		}

		void render() {
			if (Camera::main == NULL) {
				cout << "no active camera" << endl;
				return;
			}

			renderer.render(skybox, renderers2d, renderers, postProcessEffects.stack, testShader);

			const Texture& tex = renderer.getResultTexture();

#if DLTA_EDITOR
			// draw editor gui
			EditorGUI::Update(window, tex.id);
#endif

			// swap buffers
			glfwSwapBuffers(window);

			Input::Clear();
			glfwPollEvents();
			//glfwWaitEvents();
		}

		static void panic(const std::string& msg) {

			std::cout << msg << std::endl;
			exit(-1);
		}

		void quit() {
			glfwSetWindowShouldClose(window, true);
		}
	};

	void resize_framebuffer(GLFWwindow* window, int width, int height) {
		Application* r = static_cast<Application*>(glfwGetWindowUserPointer(window));
		// TODO: bring back?
		/*r->width = width;
		r->height = height;
		glViewport(0, 0, width, height);*/

		// noticed this allocates a ton of ram...
		//r->render();
	}
}
