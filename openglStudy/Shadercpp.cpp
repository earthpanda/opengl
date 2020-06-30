#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//顶点着色器 使用string 表述 定义
//gl_Position 这个值不能乱改 应该是内部定义的，否则会造成片段着色器无效的bug
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
//"out vec4 inFragColor;\n"
//"out vec3 inTrangleColor;\n"
//"void main()\n"
//"{\n"
//"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"inFragColor = vec4(0.0,1.0,1.0,0.5);\n"
//"inTrangleColor =aColor;\n"
//"}\0";



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 inTrangleColor;\n"
"out vec4 inFragColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"inFragColor = vec4(1.0,0.0,0.0,1.0);\n"
"inTrangleColor =aColor;\n"
"}\0";

//判断着色器 使用string 表述 定义
const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 inFragColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = inFragColor;\n"
"}\n\0";


//定义另外一个颜色片段着色器
//const char* colorFragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
//"}\n\0";


//使用uniform定义color
//使用uniform的意义是在于这样的话 可以在program中进行相关数值的设置
//但是需要注意的是 uniform在着色器中唯一 不能有重复
//const char* colorFragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"uniform vec4 uniColor;\n"
//"void main()\n"
//"{\n"
//"FragColor = uniColor;\n"
//"}\n\0";



const char* colorVertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec4 testColor;\n"
"out vec4 inFragColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"testColor = vec4(1.0,1.0,1.0,1.0);\n"
"inFragColor = vec4(aColor,1.0);\n"
"}\0";

const char* colorFragmentShaderSource = "#version 330 core\n"
"in vec3 inTrangleColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(inTrangleColor,1.0);\n"
"}\n\0";



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



	//对三角形顶点着色器和判断着色器进行编译
	unsigned int colorVertexShader;
	colorVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(colorVertexShader, 1, &colorVertexShaderSource, NULL);
	glCompileShader(colorVertexShader);

	//检查顶点着色器编译结果
	glGetShaderiv(colorVertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(colorVertexShader, 512, NULL, infoLog);
		std::cout << "vertext shader compile faild \n" << infoLog << std::endl;

	}


	//进行另一个片段着色器的定义
	unsigned int colorFragmentShader;
	colorFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(colorFragmentShader, 1, &colorFragmentShaderSource, NULL);
	glCompileShader(colorFragmentShader);


	glGetShaderiv(colorFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(colorFragmentShader, 512, NULL, infoLog);
		std::cout << "color fragment compile failed \n " << infoLog << std::endl;
	}


	//创建另外一个program 将顶点着色器和片段着色器进行link
	int colorShaderProgram;
	colorShaderProgram = glCreateProgram();
	glAttachShader(colorShaderProgram, vertexShader);
	glAttachShader(colorShaderProgram, colorFragmentShader);
	glLinkProgram(colorShaderProgram);

	// 检查program link error
	glGetProgramiv(colorShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(colorShaderProgram, 512, NULL, infoLog);
		std::cout << "color program link failed \n" << infoLog << std::endl;
	}



	//已经link好后 需要删除无用的着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteShader(colorFragmentShader);
	//glDeleteShader(colorVertexShader);
	

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
		-1.0f,1.0f,0.0f,
		//lb
		-1.0f,-0.0f,0.0f,
		//rb
		0.0f,0.0f,0.0f,
		//rt
		0.0f,1.0f,0.0f
	};

	unsigned int indices[] = {
		//第一个三角形
		0,1,3,
		//第二个三角形
		1,2,3

	};

	////三角形
	//float trangle[] = {
	//	//l
	//	0.0f,0.0f,0.0f,
	//	//r
	//	1.0f,0.0f,0.0f,
	//	//t
	//	1.0f,1.0f,0.0f

	//};


	//三角形
	float trangle[] = {
		//l-c
		0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
		//r-t
		1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		//t-t
		1.0f,1.0f,0.0f, 0.0f,0.0f,1.0f

	};


	/****************************************************使用IBO 进行矩形的绘制**************************************************************************/


	//定义VAO 和 VBO 数组
	//矩形使用VAO VBO和IBO 三角形使用VAO和VBO实现
	unsigned int VAOS[2], VBOS[2], IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//绑定矩形
	glBindVertexArray(VAOS[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//绑定三角形
	glBindVertexArray(VAOS[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);

	//index=0 属性设置成功
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//index=1 属性设置成功
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	/***********************************************************不使用数组进行关于三角形的绘制***************************************************************************/

		//创建顶点数组对象 和 顶点缓冲对象 和  IBO(根据需要进行创建)
		//其中VAO和VBO的过程是必须的 IBO是可选的
		//如果使用IBO 则最后的绘制方式有所区别
		//unsigned int VAO, VBO,IBO;
		//glGenVertexArrays(1, &VAO);
		//glGenBuffers(1, &VBO);
		//glGenBuffers(1, &IBO);
		//
		//// 1. 绑定顶点数组对象
		////对于VAO和VBO进行绑定  首先需要对于VAO进行绑定 然后对VBO进行绑定
		//glBindVertexArray(VAO);

		//// 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
		////使用VBO的时候需要创建关于VBO的buffer和绑定数据
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		////对于缓冲进行数据填充
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
		////使用IBO进行创建和绘制
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		////使用IBO进行数据缓冲填充
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//// 4. 设定顶点属性指针
		////对于顶点数据格式进行解释
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
		//glEnableVertexAttribArray(0);

		/**********************************************************************************************************************************************/



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
		//这里决定画多少个顶点
		//如果指定3 则会绘制前三个顶点
		//如果使用了ibo 则不用这样的绘制方式
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//绘制线框图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//绘制填充图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//使用IBO来进行绘制矩形
		glBindVertexArray(VAOS[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//激活相关的program
		//该program link 相关的fragmentShader ，且该shader中绑定了相关的 着色器语言
		glUseProgram(colorShaderProgram);
		/*float timeValue = glfwGetTime();
		float greenColor = sin(timeValue) / 2 + 0.5f;
		int uniColorLocation = glGetUniformLocation(colorShaderProgram,"uniColor");
		glUniform4f(uniColorLocation, 0.0f, greenColor, 0.0f, 1.0f);*/

		
		//使用VBO来进行三角形的绘制
		glBindVertexArray(VAOS[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);




		//window是使用双缓冲的技术，因而进行缓冲的交替
		glfwSwapBuffers(window);
		//立即处理已经传递的窗口事件
		glfwPollEvents();
	}


	// 进行资源的释放

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/


	glDeleteVertexArrays(2, VAOS);
	glDeleteBuffers(2, VBOS);
	glDeleteBuffers(1, &IBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(colorShaderProgram);



	//对于opengl程序来个结束
	glfwTerminate();

	return 0;
}



void opengl_init() {



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