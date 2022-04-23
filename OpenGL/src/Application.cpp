#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    
    float position[6] = { // メモリバッファ
    	-0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int buffer;
    glGenBuffers(1, &buffer); // バッファID生成
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // BINDはSELECTという意味
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

    // glBindBuffer(GL_ARRAY_BUFFER, 0); // 0:　無効なID

    glEnableVertexAttribArray(0); //  Enable　重要！！
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // 0: 最初, 2: 2D座標,

    /* ------------- シェーダ ------------- */ // 光や影ではない　           本質→プログラム



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawElements(GL_TRIANGLES, 3, GLfloat, NULL); // 55行目の代わりに(同じ)

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}