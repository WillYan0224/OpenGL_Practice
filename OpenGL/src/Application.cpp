#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source) // OPENGLにSHADERをコンパイルさせ
{
    unsigned int id = glCreateShader(GL_VERTEX_SHADER);
    const char* src = source.c_str(); // スコープ注意！！ Rubbish memory        string→char原因： ↓ char*が求められ
    glShaderSource(id, 1, &src, nullptr); // 
    glCompileShader(id);

    // TODO: error handling
    
    return id;
}


static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) // Avoid leaking (Static)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); 


    /*                          */        
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

	
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 640, "OpenGL Practice", NULL, NULL);
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
	// glBindBuffer(GL_ARRAY_BUFFER, 0); // 0:　無効なID

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);

 
    glEnableVertexAttribArray(0); //  Enable　重要！！
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // バッファlayout作成＋Binding -> index 0に
    /* ------------- シェーダ ------------- */ // 光や影ではない　           本質→プログラム

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); // スクリーン作成

        glDrawArrays(GL_TRIANGLES, 0, 3); // Bindされたバッファを表示
        // glDrawElements(GL_TRIANGLES, 3, GLfloat, NULL); // 55行目の代わりに(同じ)

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}