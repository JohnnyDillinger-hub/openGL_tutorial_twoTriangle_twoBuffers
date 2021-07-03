#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Каждый шейдер начинается с объявления его версии.Начиная с OpenGL 3.3 и выше, номера версий GLSL соответствуют
//версии OpenGL(например, GLSL версии 420 соответствует версии OpenGL 4.2).Мы также недвусмысленно упоминаем,
//что используем функции из core - profile.
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

    glfwInit(); // инициализируем GLFW

    /*первый аргумент - функции glfwWindowHint() указывает на то, какой из параметров GLFW мы хотим настроить,
    а все вместе они образуют большое множество опций, отличительной чертой которых является наличие префикса GLFW_;*/

    /*второй аргумент — это целое число, которое устанавливает значение нашего параметра.*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // задаем версию GLFW (первая цифра)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // задаем версию GLFW (вторая цифра)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*glfwCreateWindow()

     - первые два аргумента, которые она принимает, являются шириной и высотой окна;
     - с помощью третьего аргумента мы указываем имя окну — "OpenGL for Ravesli.com"
       (вы можете установить любое другое имя);*/
       //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL for Ravesli.com", NULL, NULL);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL for Ravesli.com", NULL, NULL);

    if (window == NULL)
    {

        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;

        /*glfwMakeContextCurrent(window) - сообщаем GLFW сделать контекст нашего окна
          основным контекстом в текущем потоке.*/
        glfwMakeContextCurrent(window);
    }

    glfwMakeContextCurrent(window);
    /* Для того, чтобы сообщить GLFW, что мы хотим вызывать функцию framebuffer_size_callback() всякий раз,
    когда происходит изменение размеров окна */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // инициализируем GLAD
    /*В качестве параметра мы передаем GLAD - функцию, которая загружает адреса указателей на OpenGL - функции(которые
    могут отличаться в зависимости от используемой вами ОС)

    В то же время, библиотека GLFW содержит удобный инструмент в виде функции glfwGetProcAddress(), которая
    самостоятельно может определить нужные нам для работы функции, в зависимости от используемой операционной системы.
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {

        std::cout << "Failed to initialize GLAD" << std::endl;

        return -1;

    }

    /* Для того, чтобы OpenGL мог использовать шейдер, он должен динамически cкомпилировать его из своего
       исходного кода во время выполнения программы.Первое, что нам нужно сделать, — это создать объект
       шейдера с ссылающимся на него идентификатором.Для этого тип переменной вершинного шейдера объявим
       как unsigned int и создадим сам шейдер с помощью функции glCreateShader()

       Далее передаем тип шейдера, который мы хотим создать, в качестве аргумента функции glCreateShader().
       Поскольку нам нужен вершинный шейдер, то в качестве аргумента передаем GL_VERTEX_SHADER*/
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Затем прикрепляем исходный код шейдера к объекту шейдера и компилируем его
    /*glShaderSource() :

        Аргумент №1 : Объект шейдера для компиляции.

        Аргумент №2 : Количество строк, которые мы передаем в качестве исходного кода(в нашем случае, это одна строка).

        Аргумент №3 : Фактический исходный код вершинного шейдера.

        Аргумент №4 : Этот аргумент мы оставляем равным NULL.*/
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // проверяем на наличие ошибок во время компиляции вершинного шейдера 
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    }

    // на этот раз, в качестве типа шейдера, мы используем константу GL_FRAGMENT_SHADER
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // проверяем на наличие ошибок во время компиляции фрагментного шейдера 
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /*Объект шейдерной программы — это финальная связанная версия нескольких шейдеров, объединенных вместе.
    Чтобы использовать недавно скомпилированные шейдеры, мы должны связать их с объектом шейдерной
    программы и затем, при рендеринге объектов, активировать её.Шейдеры активированной шейдерной программы
    будут использоваться при выполнении вызовов рендеринга.*/

    /* Создаем объект шейдерной программы
    *
     glCreateProgram() - создает шейдерную программу и возвращает идентификатор ссылки на вновь созданный
     объект шейдерной программы*/
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    /*Теперь нам нужно прикрепить ранее скомпилированные шейдеры к объекту программы и с помощью функции
    glLinkProgram() связать их*/
    glLinkProgram(shaderProgram);

    // Проверяем на наличие ошибок при сборке шейдерной программы
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // удаляем вершинный и фрагментный шейдер, так как они нам уже не нужны, они уже находятся в шейдерной программе
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Указывание вершин (и буферов) и настройка вершинных атрибутов
    float vertices1[] = {

        -0.9f, -0.5f, 0.0f,  // левая вершина   (1 треугольник)
         0.1f, -0.5f, 0.0f,  // правая вершина  (1 треугольник)
        -0.4f,  0.5f, 0.0f,  // верхняя вершина (1 треугольник)  

    };

    float vertices2[] = {

        -0.3f,  0.5f, 0.0f,  // левая вершина   (2 треугольник)
         0.7f,  0.5f, 0.0f,  // правая вершина  (2 треугольник)
         0.2f, -0.5f, 0.0f,  // нижняя вершина  (2 треугольник)

    };

    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    /*Функция glVertexAttribPointer() имеет довольно много параметров, поэтому давайте внимательно пройдемся по каждому 
    из них :

        Аргумент №1 : Указываем, какой атрибут вершины мы хотим настроить.Помните, что мы указали расположение позиции
        вершинного атрибута в вершинном шейдере при помощи layout(location = 0).Благодаря этому, местоположение 
        вершинного атрибута устанавливается в 0, и, поскольку мы хотим передать данные в этот атрибут вершины, мы 
        передаем в качестве параметра значение 0.

        Аргумент №2 : Определяем размер вершинного атрибута.Он имеет тип vec3, поэтому состоит из 3 - х значений.

        Аргумент №3 : Указываем тип данных GL_FLOAT(т.к.vec * в GLSL состоит из значений типа с плавающей точкой).

        Аргумент №4 : Указываем, хотим ли мы, чтобы наши данные были нормализованы.Если мы вводим целочисленные типы 
        данных(int, byte) и установили для них значение GL_TRUE, то целочисленные unsigned данные приводятся к 0 
        (или к - 1 для типов данных signed) и к 1 при преобразовании в тип float.Это не имеет отношения к нашему 
        примеру, поэтому мы оставим значение данного аргумента как GL_FALSE.
        
         Аргумент №5: Известен как шаг (англ. «stride») и говорит нам о пространстве между последовательными атрибутами
         вершин. Поскольку следующий набор данных местоположения вершины находится на расстоянии 3*sizeof(float), то 
         указываем это значение как шаг. Обратите внимание, поскольку мы знаем, что массив плотно упакован (нет
         никакого пространства между следующим значением атрибута вершины), мы могли бы также указать величину шага 
         как 0, чтобы позволить OpenGL самому определить шаг (это работает только тогда, когда значения плотно
         упакованы). Всякий раз, когда мы имеем дело с большим количеством вершинных атрибутов, нужно тщательно 
         определить расстояние между каждым атрибутом (примеры подобных случаев мы рассмотрим несколько позже).

         Аргумент №6: Имеет тип void* и поэтому такое странное у него оформление. Это смещение того места, где
         начинаются данные в буфере. Поскольку данные находятся в начале массива данных, то это значение равно 0.
         Позже мы рассмотрим этот параметр более подробно.*/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Обратите внимание, что данное действие разрешено, вызов glVertexAttribPointer() зарегистрировал VBO как
    привязанный вершинный буферный объект для вершинного атрибута, так что после этого мы можем спокойно
    выполнить отвязку*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*Вы можете отменить привязку VАО после этого, чтобы другие вызовы VАО случайно не изменили этот VAO (но
    подобное довольно редко случается).
    Модификация других VAO требует вызов glBindVertexArray(), поэтому мы обычно не снимаем привязку VAO
   (или VBO), когда это не требуется напрямую*/
    glBindVertexArray(0);

    // Раскомментируйте следующую строку для отрисовки полигонов в режиме каркаса
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Первыми двумя параметрами данной функции являются координаты верхнего левого угла окна просмотра. Третий и
    четвертый параметры — это координаты правого нижнего угла. В итоге мы получаем область для рендеринга шириной
    800 и высотой 600 пикселей, которая совпадает с нашим GLFW-окном.

    На самом деле, OpenGL использует данные из функции glViewport(), чтобы преобразовать обработанные им 2D-координаты
    в координаты на вашем экране. Например, обработанная точка с координатами (-0.5; 0.5) будет (после финального
    преобразования) отображена в точку с координатами (200; 450) на экране. Обратите внимание, что обработанные
    координаты в OpenGL находятся в диапазоне от -1 до 1, поэтому мы отображаем диапазон обработанных координат (-1, 1)
    на соответствующем диапазоне координат (0, 800) и (0, 600) на экране.*/
    // glViewport(0, 0, 800, 600);


    // glfwWindowShouldClose() проверяет, сообщали ли мы GLFW закрыть приложение
    while (!glfwWindowShouldClose(window))
    {

        // обработка ввода
        processInput(window);

        // Выполнение рендеринга
        /* glClearColor(red, green, blue, alpha) - указываем, какой(-ие) цвета будут очищены из буффера цветов */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        /*
        Вызывая функцию glClear(), мы хотим, чтобы она очистила один из буфер экрана. Используя в качестве
        аргументов так называемые «буферные биты», мы можем указать нашей функции glClear() какой именно буфер
        требуется очистить:

            GL_COLOR_BUFFER_BIT — очистка буфера цвета;

            GL_DEPTH_BUFFER_BIT — очистка буфера глубины;

            GL_STENCIL_BUFFER_BIT — очистка буфера трафарета.
        */
        glClear(GL_COLOR_BUFFER_BIT);

        /* ПРИМЕЧАНИЕ: Обратите внимание, что мы также задаем и цвет для очистки экрана с помощью функции glClearColor().
           Всякий раз, когда мы вызываем glClear() и, тем самым, очищаем цветовой буфер, весь цветовой буфер будет
           заполнен цветом, заданным при помощи функции glClearColor().В результате мы получим
           темно - зелено - голубоватый цвет. */

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /*glfwSwapBuffers() меняет местами цветовой буфер(большой 2D - буфер, содержащий значения цвета для каждого
          пикселя в окне GLFW), который используется для рендеринга во время данной итерации рендеринга, и выводит
          его на экран*/
        glfwSwapBuffers(window);

        /*glfwPollEvents() следит за тем, инициируются ли какие-либо события (например, ввод с клавиатуры или
            перемещение мышки), обновляет состояние окна и вызывает соответствующие функции (которые мы можем
                зарегистрировать с помощью callback-методов)*/
        glfwPollEvents();

    }

    // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);

    // glfw: завершение, освобождение всех ранее задействованных GLFW-ресурсов
    glfwTerminate();

    return 0;
}

/*
Однако в тот момент, когда пользователь изменяет размеры окна, должен быть скорректирован и размер области окна
просмотра. Для этого необходимо определить callback-функцию (или «функцию обратного вызова»), которая вызывается
при каждом изменении размера окна.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);

}

void processInput(GLFWwindow* window)
{

    // glfwGetKey(window, GLFW_KEY_ESCAPE) - отслеживает нажатие клавиши, указанной вторым агрументом
    // 1 аргумент - объект пользовательского ввода (в нашем случае этим объектом будет window, т.е. само окно)
    // 2 аргумент - клавиша, нажатие которой мы отслеживаем 
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // закрываем окно
        glfwSetWindowShouldClose(window, true);
    }

}