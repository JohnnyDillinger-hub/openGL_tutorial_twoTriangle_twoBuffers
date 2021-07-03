#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//������ ������ ���������� � ���������� ��� ������.������� � OpenGL 3.3 � ����, ������ ������ GLSL �������������
//������ OpenGL(��������, GLSL ������ 420 ������������� ������ OpenGL 4.2).�� ����� �������������� ���������,
//��� ���������� ������� �� core - profile.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.xyz, 1.0);\n"
"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in  vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

int main()
{

    glfwInit(); // �������������� GLFW

    /*������ �������� - ������� glfwWindowHint() ��������� �� ��, ����� �� ���������� GLFW �� ����� ���������,
    � ��� ������ ��� �������� ������� ��������� �����, ������������� ������ ������� �������� ������� �������� GLFW_;*/

    /*������ �������� � ��� ����� �����, ������� ������������� �������� ������ ���������.*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ������ ������ GLFW (������ �����)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // ������ ������ GLFW (������ �����)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*glfwCreateWindow()

     - ������ ��� ���������, ������� ��� ���������, �������� ������� � ������� ����;
     - � ������� �������� ��������� �� ��������� ��� ���� � "OpenGL for Ravesli.com"
       (�� ������ ���������� ����� ������ ���);*/
       //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL for Ravesli.com", NULL, NULL);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);

    if (window == NULL)
    {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;

        /*glfwMakeContextCurrent(window) - �������� GLFW ������� �������� ������ ����
          �������� ���������� � ������� ������.*/
        glfwMakeContextCurrent(window);
    }

    glfwMakeContextCurrent(window);
    /* ��� ����, ����� �������� GLFW, ��� �� ����� �������� ������� framebuffer_size_callback() ������ ���,
    ����� ���������� ��������� �������� ���� */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // �������������� GLAD
    /*� �������� ��������� �� �������� GLAD - �������, ������� ��������� ������ ���������� �� OpenGL - �������(�������
    ����� ���������� � ����������� �� ������������ ���� ��)

    � �� �� �����, ���������� GLFW �������� ������� ���������� � ���� ������� glfwGetProcAddress(), �������
    �������������� ����� ���������� ������ ��� ��� ������ �������, � ����������� �� ������������ ������������ �������.
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;

    }

    /* ��� ����, ����� OpenGL ��� ������������ ������, �� ������ ����������� c������������� ��� �� ������
       ��������� ���� �� ����� ���������� ���������.������, ��� ��� ����� �������, � ��� ������� ������
       ������� � ����������� �� ���� ���������������.��� ����� ��� ���������� ���������� ������� �������
       ��� unsigned int � �������� ��� ������ � ������� ������� glCreateShader()

       ����� �������� ��� �������, ������� �� ����� �������, � �������� ��������� ������� glCreateShader().
       ��������� ��� ����� ��������� ������, �� � �������� ��������� �������� GL_VERTEX_SHADER*/
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // ����� ����������� �������� ��� ������� � ������� ������� � ����������� ���
    /*glShaderSource() :

        �������� �1 : ������ ������� ��� ����������.

        �������� �2 : ���������� �����, ������� �� �������� � �������� ��������� ����(� ����� ������, ��� ���� ������).

        �������� �3 : ����������� �������� ��� ���������� �������.

        �������� �4 : ���� �������� �� ��������� ������ NULL.*/
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // ��������� �� ������� ������ �� ����� ���������� ���������� ������� 
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    // �� ���� ���, � �������� ���� �������, �� ���������� ��������� GL_FRAGMENT_SHADER
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // ��������� �� ������� ������ �� ����� ���������� ������������ ������� 
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /*������ ��������� ��������� � ��� ��������� ��������� ������ ���������� ��������, ������������ ������.
    ����� ������������ ������� ���������������� �������, �� ������ ������� �� � �������� ���������
    ��������� � �����, ��� ���������� ��������, ������������ �.������� �������������� ��������� ���������
    ����� �������������� ��� ���������� ������� ����������.*/

    /* ������� ������ ��������� ���������
    *
     glCreateProgram() - ������� ��������� ��������� � ���������� ������������� ������ �� ����� ���������
     ������ ��������� ���������*/
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    /*������ ��� ����� ���������� ����� ���������������� ������� � ������� ��������� � � ������� �������
    glLinkProgram() ������� ��*/
    glLinkProgram(shaderProgram);

    // ��������� �� ������� ������ ��� ������ ��������� ���������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // ������� ��������� � ����������� ������, ��� ��� ��� ��� ��� �� �����, ��� ��� ��������� � ��������� ���������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���������� ������ (� �������) � ��������� ��������� ���������
    float vertices1[] = {

        -0.9f, -0.5f, 0.0f,  // ����� �������   (1 �����������)
         0.1f, -0.5f, 0.0f,  // ������ �������  (1 �����������)
        -0.4f,  0.5f, 0.0f,  // ������� ������� (1 �����������)  

    };

    float vertices2[] = {

        -0.3f,  0.5f, 0.0f,  // ����� �������   (2 �����������)
         0.7f,  0.5f, 0.0f,  // ������ �������  (2 �����������)
         0.2f, -0.5f, 0.0f,  // ������ �������  (2 �����������)

    };

    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    /*������� glVertexAttribPointer() ����� �������� ����� ����������, ������� ������� ����������� ��������� �� ������� 
    �� ��� :

        �������� �1 : ���������, ����� ������� ������� �� ����� ���������.�������, ��� �� ������� ������������ �������
        ���������� �������� � ��������� ������� ��� ������ layout(location = 0).��������� �����, �������������� 
        ���������� �������� ��������������� � 0, �, ��������� �� ����� �������� ������ � ���� ������� �������, �� 
        �������� � �������� ��������� �������� 0.

        �������� �2 : ���������� ������ ���������� ��������.�� ����� ��� vec3, ������� ������� �� 3 - � ��������.

        �������� �3 : ��������� ��� ������ GL_FLOAT(�.�.vec * � GLSL ������� �� �������� ���� � ��������� ������).

        �������� �4 : ���������, ����� �� ��, ����� ���� ������ ���� �������������.���� �� ������ ������������� ���� 
        ������(int, byte) � ���������� ��� ��� �������� GL_TRUE, �� ������������� unsigned ������ ���������� � 0 
        (��� � - 1 ��� ����� ������ signed) � � 1 ��� �������������� � ��� float.��� �� ����� ��������� � ������ 
        �������, ������� �� ������� �������� ������� ��������� ��� GL_FALSE.
        
         �������� �5: �������� ��� ��� (����. �stride�) � ������� ��� � ������������ ����� ����������������� ����������
         ������. ��������� ��������� ����� ������ �������������� ������� ��������� �� ���������� 3*sizeof(float), �� 
         ��������� ��� �������� ��� ���. �������� ��������, ��������� �� �����, ��� ������ ������ �������� (���
         �������� ������������ ����� ��������� ��������� �������� �������), �� ����� �� ����� ������� �������� ���� 
         ��� 0, ����� ��������� OpenGL ������ ���������� ��� (��� �������� ������ �����, ����� �������� ������
         ���������). ������ ���, ����� �� ����� ���� � ������� ����������� ��������� ���������, ����� ��������� 
         ���������� ���������� ����� ������ ��������� (������� �������� ������� �� ���������� ��������� �����).

         �������� �6: ����� ��� void* � ������� ����� �������� � ���� ����������. ��� �������� ���� �����, ���
         ���������� ������ � ������. ��������� ������ ��������� � ������ ������� ������, �� ��� �������� ����� 0.
         ����� �� ���������� ���� �������� ����� ��������.*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*�������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer() ��������������� VBO ���
    ����������� ��������� �������� ������ ��� ���������� ��������, ��� ��� ����� ����� �� ����� ��������
    ��������� �������*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*�� ������ �������� �������� V�� ����� �����, ����� ������ ������ V�� �������� �� �������� ���� VAO (��
    �������� �������� ����� ���������).
    ����������� ������ VAO ������� ����� glBindVertexArray(), ������� �� ������ �� ������� �������� VAO
   (��� VBO), ����� ��� �� ��������� ��������*/
    glBindVertexArray(0);

    // ���������������� ��������� ������ ��� ��������� ��������� � ������ �������
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* ������� ����� ����������� ������ ������� �������� ���������� �������� ������ ���� ���� ���������. ������ �
    ��������� ��������� � ��� ���������� ������� ������� ����. � ����� �� �������� ������� ��� ���������� �������
    800 � ������� 600 ��������, ������� ��������� � ����� GLFW-�����.

    �� ����� ����, OpenGL ���������� ������ �� ������� glViewport(), ����� ������������� ������������ �� 2D-����������
    � ���������� �� ����� ������. ��������, ������������ ����� � ������������ (-0.5; 0.5) ����� (����� ����������
    ��������������) ���������� � ����� � ������������ (200; 450) �� ������. �������� ��������, ��� ������������
    ���������� � OpenGL ��������� � ��������� �� -1 �� 1, ������� �� ���������� �������� ������������ ��������� (-1, 1)
    �� ��������������� ��������� ��������� (0, 800) � (0, 600) �� ������.*/
    // glViewport(0, 0, 800, 600);


    // glfwWindowShouldClose() ���������, �������� �� �� GLFW ������� ����������
    while (!glfwWindowShouldClose(window))
    {

        // ��������� �����
        processInput(window);

        // ���������� ����������
        /* glClearColor(red, green, blue, alpha) - ���������, �����(-��) ����� ����� ������� �� ������� ������ */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        /*
        ������� ������� glClear(), �� �����, ����� ��� �������� ���� �� ����� ������. ��������� � ��������
        ���������� ��� ���������� ��������� �����, �� ����� ������� ����� ������� glClear() ����� ������ �����
        ��������� ��������:

            GL_COLOR_BUFFER_BIT � ������� ������ �����;

            GL_DEPTH_BUFFER_BIT � ������� ������ �������;

            GL_STENCIL_BUFFER_BIT � ������� ������ ���������.
        */
        glClear(GL_COLOR_BUFFER_BIT);

        /* ����������: �������� ��������, ��� �� ����� ������ � ���� ��� ������� ������ � ������� ������� glClearColor().
           ������ ���, ����� �� �������� glClear() �, ��� �����, ������� �������� �����, ���� �������� ����� �����
           �������� ������, �������� ��� ������ ������� glClearColor().� ���������� �� �������
           ����� - ������ - ����������� ����. */

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /*glfwSwapBuffers() ������ ������� �������� �����(������� 2D - �����, ���������� �������� ����� ��� �������
          ������� � ���� GLFW), ������� ������������ ��� ���������� �� ����� ������ �������� ����������, � �������
          ��� �� �����*/
        glfwSwapBuffers(window);

        /*glfwPollEvents() ������ �� ���, ������������ �� �����-���� ������� (��������, ���� � ���������� ���
            ����������� �����), ��������� ��������� ���� � �������� ��������������� ������� (������� �� �����
                ���������������� � ������� callback-�������)*/
        glfwPollEvents();

    }

    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();

    return 0;
}

/*
������ � ��� ������, ����� ������������ �������� ������� ����, ������ ���� �������������� � ������ ������� ����
���������. ��� ����� ���������� ���������� callback-������� (��� �������� ��������� ������), ������� ����������
��� ������ ��������� ������� ����.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);

}

void processInput(GLFWwindow* window)
{

    // glfwGetKey(window, GLFW_KEY_ESCAPE) - ����������� ������� �������, ��������� ������ ����������
    // 1 �������� - ������ ����������������� ����� (� ����� ������ ���� �������� ����� window, �.�. ���� ����)
    // 2 �������� - �������, ������� ������� �� ����������� 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // ��������� ����
        glfwSetWindowShouldClose(window, true);
    }

}