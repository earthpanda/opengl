#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//顶点着色器 使用string 表述 定义
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//判断着色器 使用string 表述 定义
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";



int main() {

	//初始化glfw
	glfwInit();
	//opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL WINDOW", NULL, NULL);

	//检查窗口是否时null
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//将window设置为上下文
	glfwMakeContextCurrent(window);
	//添加回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//检查glad是否已经正确加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to load glad " << std::endl;

		return -1;

	}

	//视口大小 一个window窗口可以含有多个视口 以达到分屏的效果
	glViewport(0, 0, 800, 600);


	//对顶点着色器和判断着色器进行编译
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//检查顶点着色器编译结果
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertext shader compile faild \n" << infoLog << std::endl;

	}


	//对于片段着色器进行编译
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//检查片段着色器编译结果
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragment shader compile failed \n " << infoLog << std::endl;

	}


	//创建program将 顶点着色器和片段着色器进行link
	int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 检查program link error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "program link failed \n" << infoLog << std::endl;
	}

	//已经link好后 需要删除无用的着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/*****************************************************三角形顶点数组************************************************************/
	//float vertices[] = {
	//	//left
	//	-0.5f,-0.5f,0.0f,
	//	//right
	//	0.5f,-0.5f,0.0f,
	//	//top
	//	0.0f,0.5f,0.0f
	//};
	/******************************************************************************************************************************/


	/*****************************************************绘制矩形数组************************************************************/
	/*float vertices[] = {

		-0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		 0.5f,0.5f,0.0f,

		0.5,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		};*/

		/****************************************************使用IBO 进行矩形的绘制**************************************************************************/
	float vertices[] = {
		//lt
		-0.5f,0.5f,0.0f,
		//lb
		-0.5f,-0.5f,0.0f,
		//rb
		0.5f,-0.5f,0.0f,
		//rt
		0.5f,0.5f,0.0f
	};

	unsigned int indices[] = {
		//第一个三角形
		0,1,3,
		//第二个三角形
		1,2,3

	};



	/****************************************************使用IBO 进行矩形的绘制**************************************************************************/



	//创建顶点数组对象 和 顶点缓冲对象 和  IBO(根据需要进行创建)
	//其中VAO和VBO的过程是必须的 IBO是可选的
	//如果使用IBO 则最后的绘制方式有所区别
	unsigned int VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	// 1. 绑定顶点数组对象
	//对于VAO和VBO进行绑定  首先需要对于VAO进行绑定 然后对VBO进行绑定
	glBindVertexArray(VAO);

	// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	//使用VBO的时候需要创建关于VBO的buffer和绑定数据
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//对于缓冲进行数据填充
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	//使用IBO进行创建和绘制
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//使用IBO进行数据缓冲填充
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// 4. 设定顶点属性指针
	//对于顶点数据格式进行解释
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);





	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);



	//如果window没有被关闭 则不管进行绘制
	while (!glfwWindowShouldClose(window)) {
		//监听窗口事件
		process_input(window);

		// render
	  // ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//绘制三角形
		glUseProgram(shaderProgram);
		// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glBindVertexArray(VAO);
		//这里决定画多少个顶点
		//如果指定3 则会绘制前三个顶点
		//如果使用了ibo 则不用这样的绘制方式
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//绘制线框图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//绘制填充图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//使用IBO来进行绘制
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




		//window是使用双缓冲的技术，因而进行缓冲的交替
		glfwSwapBuffers(window);
		//立即处理已经传递的窗口事件
		glfwPollEvents();
	}


	// 进行资源的释放

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);



	//对于opengl程序来个结束
	glfwTerminate();

	return 0;
}

/*

opengl的回调函数，用于回调窗口的变化

*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*

自定义的用于监听window事件的函数
*/
void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}