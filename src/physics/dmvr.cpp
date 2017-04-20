#include <iostream>
#include <math.h>
#include "shader.h"
#include "physics.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL
#include <soil/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLFWwindow *initialize();
void do_physics();
unsigned activate = 1;

bool keys[1024];
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat deltaTime = 0.0f;

b2Body *body;

int main() {

  // Set up GLFW
  GLFWwindow *window = initialize();

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return -1;
  }

  // Window Initialization
  glViewport(0, 0, 800, 600);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glEnable(GL_DEPTH_TEST);

  // Shaders
  Shader ourShader("src/shader.v", "src/shader.frag");

  // Physics
  body = phys_init();

  // Vertex Data
  GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };


  // 3 Cubes
  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, 0.0f),
    glm::vec3( 0.0f, -1.2f, 0.0f),
    glm::vec3( 0.0f, -2.4f, 0.0f),
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO); // We can also generate multiple VAOs or buffers
  glGenBuffers(1, &VBO);
  // ================================
  // Rectangle Setup
  // ================================
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position Attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)      0);
  glEnableVertexAttribArray(0);
  // TexCoord Attributes
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)      (3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  // Texture 1
  GLuint texture1, texture2;
  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // Texture Prefs
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  // Texture Generation
  int width, height;
  unsigned char *image = SOIL_load_image("images/container.jpg", &width,
      &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Texture 2
  glBindTexture(GL_TEXTURE_2D, texture2);
  // Texture Prefs
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  image = SOIL_load_image("images/awesomeface.png", &width,
      &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);

  // Main Loop Initialization
  ourShader.Use();

  GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
  GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
  GLuint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

  // Main loop
  GLfloat lastFrame = 0.0f;
  GLfloat currentFrame = 0.0f;
  while(!glfwWindowShouldClose(window)) {

    // Handle input
    glfwPollEvents();
    do_physics();

    // Time
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;

    // Fixed update for physics
    if (deltaTime >= 1/60.0) {
      lastFrame = currentFrame;
      step(activate);
    }

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

    // Camera
    glm::mat4 view, projection;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    projection = glm::perspective(70.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Fixed Cubes
    glBindVertexArray(VAO);
    for (GLuint i = 0; i < 3; i++) {
      if (activate == i + 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      glm::mat4 model;
      model = glm::translate(model, cubePositions[i]);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // Moving one
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glm::mat4 model;
    b2Vec2 pos = body->GetPosition();
    model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Cleanup
    glBindVertexArray(0);
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}

// Perform physics update
void do_physics() {
  GLfloat cameraSpeed = 5.0f * deltaTime;
  if (keys[GLFW_KEY_UP])
    cameraPos += cameraSpeed * cameraFront;
  if (keys[GLFW_KEY_DOWN])
    cameraPos -= cameraSpeed * cameraFront;
  if (keys[GLFW_KEY_LEFT])
    cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront,
                cameraUp));
  if (keys[GLFW_KEY_RIGHT])
    cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront,
                cameraUp));
  if (keys[GLFW_KEY_SPACE]) {
    b2Vec2 jumpImpulse(0, body->GetMass() * 2);
    body->ApplyLinearImpulse(jumpImpulse, body->GetWorldCenter(), true);
  }
}

// Record key input
void key_callback(GLFWwindow *window, int key, int scancode, int action,
    int mode) {
  if (action == GLFW_PRESS)
    keys[key] = true;
  else if (action == GLFW_RELEASE)
    keys[key] = false;
  if (keys[GLFW_KEY_ESCAPE])
    glfwSetWindowShouldClose(window, GL_TRUE);
  if (keys[GLFW_KEY_1])
    activate = 1;
  if (keys[GLFW_KEY_2])
    activate = 2;
  if (keys[GLFW_KEY_3])
    activate = 3;
}

// Initialize GLFW window
GLFWwindow *initialize() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr,
          nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }

  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  return window;
}
