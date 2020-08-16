#include "Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//文件比例 1.0代表前一张透明 后一张100%
float radio = 1.0f;
int screenWidth = 1920;
int screenHeight = 1080;


float picWidth = 1200.0f;
float picHeight = 800.0f;


float picScreenWidth = picWidth / screenWidth;
float picScreenHeight = picHeight / screenHeight;

float screenPicWidth = screenWidth / picWidth;
float screenPicHeight = screenHeight / picHeight;


void Texture::run() {

	//初始化glfw
	glfwInit();
	//opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OPENGL WINDOW", NULL, NULL);

	//检查窗口是否时null
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	//将window设置为上下文
	glfwMakeContextCurrent(window);
	//添加回调函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//检查glad是否已经正确加载
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to load glad " << std::endl;

		return;

	}

	//视口大小 一个window窗口可以含有多个视口 以达到分屏的效果
	glViewport(0, 0, screenWidth, screenHeight);


	//这里很奇怪和vs有关系 它创建的不是具体的文件夹 只是虚构的文件夹，因而这里还是在同目录下
	//FileSystem::getPath("resources/textures/awesomeface.png")
	const char* vertextPath = ".\\glsl\\cow\\cowVertex.vert";
	const char* fragmentPath = ".\\glsl\\cow\\cowFragment.frag";
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
	unsigned char* data = stbi_load(".\\img\\cow_12_8.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//释放资源
	stbi_image_free(data);

		/****************************************************使用IBO 进行矩形的绘制 逆时针顺序 **************************************************************************/
	//窗口坐标 -1.0 - 1.0 之间
	float vertices[] = {
		//lt
		-1.0f,1.0f,0.0f,
		//lb
		-1.0f,-1.0f,0.0f,
		//rb
		1.0f,-1.0f,0.0f,
		//rt
		1.0f,1.0f,0.0f
	};

		//三角形 旋转测试
	float trangle[] = {
		//lt color
		-1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,
		//lb color
		-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
		//rb color
		1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
		//rt color
		1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f
	};

		//没有适应全屏的数据
	//	float trangle[] = {
	//	//lt color
	//	-1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,
	//	//lb color
	//	-1.0f,-0.48f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
	//	//rb color
	//	0.33f,-0.48f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
	//	//rt color
	//	0.33f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f
	//};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};


	/****************************************************使用IBO 进行矩形的绘制**************************************************************************/


	//创建顶点数组对象 和 顶点缓冲对象 和  IBO(根据需要进行创建)
	//其中VAO和VBO的过程是必须的 IBO是可选的
	//如果使用IBO 则最后的绘制方式有所区别
	unsigned int VAOS[2], VBOS[2], IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//1. 绑定顶点数组对象
	//对于VAO和VBO进行绑定  首先需要对于VAO进行绑定 然后对VBO进行绑定
	glBindVertexArray(VAOS[0]);

	//2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
	//使用VBO的时候需要创建关于VBO的buffer和绑定数据
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);

	// 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	//使用IBO进行创建和绘制
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//使用IBO进行数据缓冲填充
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 4. 设定顶点属性指针
	//对于顶点数据格式进行解释
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);



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


		//绘制线框图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//绘制填充图
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




		//使用IBO来进行绘制矩形
		glBindVertexArray(VAOS[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);


		//float timeValue = glfwGetTime();
		//std::cout << timeValue << std::endl;
		//float translate = sin(timeValue) / 2 + 0.5f;
		glm::mat4 trans = glm::mat4(1.0f);

		
		//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	/*	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 0.0f));*/
		//trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));
		//trans = glm::scale(trans, glm::vec3(screenPicWidth,screenPicHeight,1.0f));
		//trans = glm::scale(trans, glm::vec3(0.25f, 0.25f, 0.25f));
		//trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));




		////观察矩阵
		glm::mat4 viewMat = glm::mat4(1.0f);
		viewMat = glm::scale(viewMat, glm::vec3(0.1f, 1.0f, 1.0f));
		//viewMat = glm::rotate(viewMat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		viewMat = glm::translate(viewMat, glm::vec3(9.0f, 0.0f, 0.0f));

		////投影矩阵
		//glm::mat4 projectionMat = glm::mat4(1.0f);
		//projectionMat = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		//shader.setUniformFloat("translateX", translate);
		//shader.setUniformFloat("radio", radio);
		shader.setUniformMat4("transform", trans);
		shader.setUniformMat4("view", viewMat);
		//shader.setUniformMat4("projection", projectionMat);



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
