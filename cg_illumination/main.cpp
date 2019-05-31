#include <iostream>
#include <cmath>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

// SOIL
#include <SOIL.h>

// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

// Other includes
#include "camera.h"
#include "shader.h"
#include "vertex.h"
#include "primitives.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "object.h"
#include "planets.h"

// Function prototypes
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void DoMovement(GLfloat &cooldown);

// Window dimensions
GLuint WIDTH = 1800, HEIGHT = 1000;

// Camera
Camera camera(glm::vec3(0.f, 0.f, 7.f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
GLfloat camera_speed = 3.0f;

// Deltatime
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

// Rotation
GLfloat angle_x = 0.0f;
GLfloat rot_speed = 40.0f;
glm::quat rot_quat = glm::angleAxis(angle_x, glm::vec3(1, 0, 0));

// Tesselation
GLfloat level = 5.f;
bool line_mode = false;

// The MAIN function, from here we start the application and run the game loop
int main(){
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Illumination", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// Initialize GLAD to setup the OpenGL Function pointers
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Build and compile shader program
	Shader shader(4, 1, "main.vert.glsl", "main.frag.glsl");

	// Load and create textures
	std::vector<Texture*> textures;
	textures.push_back(new Texture("white.jpg", GL_TEXTURE_2D));

	// Create materials
	Material* material = new Material(glm::vec3(0.1f), glm::vec3(0.7f), glm::vec3(0.5f), 0, 1);

	// Lights
	glm::vec3 light(5.f, 5.f, 5.f);
	shader.setVec3f(light, "lightPos0");

	// Set up vertex data (and buffer(s)) and attribute pointers
	std::vector<Mesh*> meshes;
	Mesh* model = new Mesh("eight.txt");
	meshes.push_back(model);
	Object surface(glm::vec3(0.f), material, textures[0], textures[0], meshes);

	// enable transparency
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat cooldown = 0.f;

	// Game loop
	while(!glfwWindowShouldClose(window)){
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		cooldown -= deltaTime;
		if(cooldown < 0)
			cooldown = 0;
		DoMovement(cooldown);

		// Render
		// Clear the colorbuffer
		glClearColor(.0f, .0f, .0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Camera/View transformation
		glm::mat4 view(1);
		view = camera.getViewMatrix();
		// Projection
		glm::mat4 projection(1);
		projection = camera.getProjectionMatrix((GLfloat)WIDTH, (GLfloat)HEIGHT);
		shader.setMat4fv(view, "view");
		shader.setMat4fv(projection, "projection");
		shader.setVec3f(camera.getPosition(), "cameraPos");

		// Apply keyboard rotation
		surface.rotateAroundOrigin(glm::eulerAngles(rot_quat));

		// Toggle display mode
		if(line_mode){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}else{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Render control points
		surface.render(&shader);
		
		rot_quat = glm::angleAxis(angle_x, glm::vec3(1, 0, 0));
		glClear(GL_DEPTH_BUFFER_BIT);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height){
	WIDTH = width;
	HEIGHT = height;
	glViewport(0, 0, width, height);
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;
	}
}

// Move through the scene
void DoMovement(GLfloat &cooldown){
	// Camera lateral movement
	if(keys[GLFW_KEY_W]){
		camera.Move(FORWARD, deltaTime * camera_speed);
	}
	if(keys[GLFW_KEY_S]){
		camera.Move(BACKWARD, deltaTime * camera_speed);
	}
	if(keys[GLFW_KEY_A]){
		camera.Move(LEFT, deltaTime * camera_speed);
	}
	if(keys[GLFW_KEY_D]){
		camera.Move(RIGHT, deltaTime * camera_speed);
	}
	if(keys[GLFW_KEY_Q]){
		camera.Move(DOWN, deltaTime * camera_speed);
	}
	if(keys[GLFW_KEY_E]){
		camera.Move(UP, deltaTime * camera_speed);
	}
	// Camera rotations
	if(keys[GLFW_KEY_R]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(1, 0, 0));
	}
	if(keys[GLFW_KEY_F]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(1, 0, 0));
	}
	if(keys[GLFW_KEY_T]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(0, 1, 0));
	}
	if(keys[GLFW_KEY_G]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(0, 1, 0));
	}
	if(keys[GLFW_KEY_Y]){
		rot_quat = glm::rotate(rot_quat, rot_speed * deltaTime, glm::vec3(0, 0, 1));
	}
	if(keys[GLFW_KEY_H]){
		rot_quat = glm::rotate(rot_quat, -rot_speed * deltaTime, glm::vec3(0, 0, 1));
	}
	// Reset
	if(keys[GLFW_KEY_Z]){
		camera.ResetPosition();
		rot_quat = glm::angleAxis(angle_x, glm::vec3(1, 0, 0));
	}
	// Change model speed
	if(keys[GLFW_KEY_X] && cooldown == 0){
		cooldown = .2f;
		level /= 2.f;
	}
	if(keys[GLFW_KEY_C] && cooldown == 0){
		cooldown = .2f;
		level --;
	}
	if(keys[GLFW_KEY_V] && cooldown == 0){
		cooldown = .2f;
		level++;
	}
	if(keys[GLFW_KEY_B] && cooldown == 0){
		cooldown = .2f;
		level *= 2.f;
	}
	// Toggle line mode
	if(keys[GLFW_KEY_N] && cooldown == 0){
		cooldown = .2f;
		line_mode = !line_mode;
	}
}

// Camera tilt via mouse
void MouseCallback(GLFWwindow* window, double xpos, double ypos){
	if(firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Zoom
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
	camera.ProcessMouseScroll(yoffset);
}