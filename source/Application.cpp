#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Renderer.h>
#include <buffers/VertexBuffer.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

void GLAPIENTRY ErrorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    bool debug = false;
    printf("\nMessage from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
        "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);

    exit(-1);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    window = glfwCreateWindow(1024, 600, "Tab", NULL, NULL);
    if (window == GLFW_FALSE)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GLenum error = glewInit();
    if (GLEW_OK != error)
    {
        std::cout << "Error: %s\n" << glewGetErrorString(error);
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;

    float positions[] =
    {
        100.0f, 100.0f, 0.0f, 0.0f,
        100.0f, 200.0f, 0.0f, 1.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.5f, 200.5f, 1.0f, 1.0f
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        1, 2, 3
    };

    float coords[2] = {1024.0f, 600.0f};

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray vertexArrayObject;
    VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<GLfloat>(2);
    layout.Push<GLfloat>(2);

    vertexArrayObject.AddBuffer(vertexBuffer, layout);

    IndexBuffer indexBuffer(indices, 6 * sizeof(GLuint));

    glm::mat4 proj = glm::ortho(0.0f, coords[0], 0.0f, coords[1], -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -100.0f, 0.0f));

    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    Texture texture("res/textures/grass.png");
    texture.Bind();
    shader.SetUniform1i("uTexture", 0);

    vertexArrayObject.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();
    shader.Unbind();
    texture.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsClassic();
    
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glm::vec3 translationA(0, 0, 0);
    glm::vec3 translationB(0, 200, 0);

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(ErrorOccurredGL, NULL);

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        
        ImGui_ImplGlfwGL3_NewFrame();

        shader.Bind();
        texture.Bind();
            
        glm::mat4 modelA = glm::translate(glm::mat4(1.0f), translationA);
        glm::mat4 mvpA = proj * view * modelA;
        shader.SetUniformMat4f("uMVP", mvpA);

        renderer.Draw(vertexArrayObject, indexBuffer, shader);

        glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
        glm::mat4 mvpB = proj * view * modelB;
        shader.SetUniformMat4f("uMVP", mvpB);

        {
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0, coords[1]-100);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0, coords[1]-100);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        renderer.Draw(vertexArrayObject, indexBuffer, shader);

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    vertexArrayObject.Unbind();
    shader.Unbind();
    indexBuffer.Unbind();

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


