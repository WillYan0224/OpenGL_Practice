#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


static unsigned int CompileShader(unsigned int type, const std::string& source) // OPENGLにSHADERをコンパイルさせ
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // スコープ注意！！ Rubbish memory        source -> src:  string -> char
    glShaderSource(id, 1, &src, nullptr); // 
    glCompileShader(id);

    /* Error Handling */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char)); // static dynamic allocation
    	// OR IN heap: char* <var> = new [] / Unique pointer

    	glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed on compiling " <<
            (type == GL_FRAGMENT_SHADER ? "Fragment!" : "Vertex!") << " shader!\n" ;
        std::cout << message << std::endl;
        glDeleteShader(id);

    	return 0;
    }

	return id; // IDを戻す 
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) // Avoid leaking (Static)
{
    unsigned int program = glCreateProgram();  // (unsigned int) Program 作成 (for idバッファ)
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); 
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); 


    /* ------- vs&fsをリンクする -------  */        // Validation Operation
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
	/* ------- vs&fs → program -------  */
	/* ------- clear shader ------- */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
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
    
    float positions[6] = { // メモリバッファ
    	-0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };


    std::cout << glGetString(GL_VERSION) << std::endl;

    /* ------------- Vertex Buffer 作成 ------------- */
    unsigned int buffer;
    glGenBuffers(1, &buffer); // バッファID生成
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // BINDはSELECTという意味
	// glBindBuffer(GL_ARRAY_BUFFER, 0); // 0:　無効なID

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

 
    glEnableVertexAttribArray(0); //  Enable　重要！！
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // バッファlayout作成＋Binding -> index 0に

    /* ------------- シェーダ ------------- */ // 光や影ではない　           本質→プログラム
    std::string vertexShader = // VS 3回コール
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 positions;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = positions;\n"
        "}\n";

    std::string fragmentShader = // FS　ピクセルごとにコール
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    /* ↑↑↑↑↑↑↑↑↑↑↑↑ 問題：stringになってる! ↑↑↑↑↑↑↑↑↑↑↑↑ */
      

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    glUseProgram(shader);

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

    glDeleteProgram(shader); // clean up shader

    glfwTerminate();
    return 0;
}