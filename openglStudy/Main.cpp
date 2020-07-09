#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//文件比例 1.0代表前一张透明 后一张100%
float radio = 1.0f;


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


	/*const char* vertextPath = ".\\vertex.vert";
	const char* fragmentPath = ".\\fragment.frag";*/

	//这里很奇怪和vs有关系 它创建的不是具体的文件夹 只是虚构的文件夹，因而这里还是在同目录下
	//FileSystem::getPath("resources/textures/awesomeface.png")
	const char* vertextPath = ".\\vertex.vert";
	const char* fragmentPath = ".\\fragment.frag";
	//
	Shader shader(vertextPath, fragmentPath);
	//Shader* shader = new Shader(vertextPath, fragmentPath);

	//由于图片坐标左上角(0,0) opengl纹理左下角坐标(0,0) 因而vertical是相反的，因而 设置这个参数使之图像看起来是正的
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//开始处理纹理 进行一个纹理的加载
	int width, height, nrChannels;
	//stb_image.h将会用图像的宽度、高度和颜色通道的个数填充这三个变量
	unsigned char* data = stbi_load(".\\container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//释放资源
	stbi_image_free(data);


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // set texture filtering to nearest neighbor to clearly see the texels/pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	data = stbi_load(".\\awesomeface.png", &width, &height, &nrChannels, 0);

	if (data) {
		//.png 是RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//释放资源
	stbi_image_free(data);

	/*********************************************************glm 矩阵相关处理***********************************************************************/
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//vec = trans * vec;
	////输出210
	//std::cout << vec.x <<vec.y<< vec.z << std::endl;
	





	/************************************************************************************************************************************************/


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

	//unsigned int indices[] = {
	//	//第一个三角形
	//	0,1,3,
	//	//第二个三角形
	//	1,2,3

	//};

	////三角形
	//float trangle[] = {
	//	//l color
	//	-0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.45f,0.55f,
	//	//b color
	//	0.0f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.5f,0.45f,
	//	//r color
	//	0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,0.55f,0.55f

	//}

		//三角形
	float trangle[] = {
		//l color
		-0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f,
		//b color
		0.0f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.5f,0.0f,
		//r color
		0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f

	};

	unsigned int indices[] = {
		0,1,2
	};


	/****************************************************使用IBO 进行矩形的绘制**************************************************************************/


	//定义VAO 和 VBO 数组
	//矩形使用VAO VBO和IBO 三角形使用VAO和VBO实现
	unsigned int VAOS[2], VBOS[2], IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//绑定矩形
	//glBindVertexArray(VAOS[0]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	//glEnableVertexAttribArray(0);

	//
	glBindVertexArray(VAOS[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);


	//绑定三角形
	//glBindVertexArray(VAOS[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(2);



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


	shader.useProgram();
	shader.setUniformInt("texture0", 0);
	shader.setUniformInt("texture1", 1);

	//如果window没有被关闭 则不管进行绘制
	while (!glfwWindowShouldClose(window)) {
		//监听窗口事件
		process_input(window);

		// render
	  // ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//绘制三角形
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
		/*glBindVertexArray(VAOS[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		
		float timeValue = glfwGetTime();
		std::cout << timeValue << std::endl;
		float translate = sin(timeValue) / 2 + 0.5f;
		glm::mat4 trans = glm::mat4(1.0f);

		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		trans = glm::rotate(trans, timeValue, glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	
		shader.setUniformFloat("translateX", translate);
		shader.setUniformFloat("radio", radio);
		shader.setUniformMat4("transform", trans);

		glBindVertexArray(VAOS[1]);
		//使用VBO来进行三角形的绘制
		glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
		//绘制一个三角形
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// second transformation
	   // ---------------------
		trans = glm::mat4(1.0f); // reset it to identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		shader.setUniformMat4("transform", trans);
		//绘制第二个三角形
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);



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
	glDeleteTextures(1, &texture0);
	shader.release();



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

		std::cout << "press down GLFW_KEY_SPACE " << std::endl;
		glfwSetWindowShouldClose(window, true);

	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		std::cout << "press down " << std::endl;
		radio = radio - 0.01;
		if (radio <= 0.0f) {
			radio = 0.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		std::cout << "press up " << std::endl;
		radio = radio + 0.01;
		if (radio >= 1.0f) {
			radio = 0.0f;
		}
	}



}