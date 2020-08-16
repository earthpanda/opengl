#include "Texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//�ļ����� 1.0����ǰһ��͸�� ��һ��100%
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

	//��ʼ��glfw
	glfwInit();
	//opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��������
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "OPENGL WINDOW", NULL, NULL);

	//��鴰���Ƿ�ʱnull
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	//��window����Ϊ������
	glfwMakeContextCurrent(window);
	//��ӻص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//���glad�Ƿ��Ѿ���ȷ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to load glad " << std::endl;

		return;

	}

	//�ӿڴ�С һ��window���ڿ��Ժ��ж���ӿ� �Դﵽ������Ч��
	glViewport(0, 0, screenWidth, screenHeight);


	//�������ֺ�vs�й�ϵ �������Ĳ��Ǿ�����ļ��� ֻ���鹹���ļ��У�������ﻹ����ͬĿ¼��
	//FileSystem::getPath("resources/textures/awesomeface.png")
	const char* vertextPath = ".\\glsl\\cow\\cowVertex.vert";
	const char* fragmentPath = ".\\glsl\\cow\\cowFragment.frag";
	//
	Shader shader(vertextPath, fragmentPath);
	//Shader* shader = new Shader(vertextPath, fragmentPath);

	//����ͼƬ�������Ͻ�(0,0) opengl�������½�����(0,0) ���vertical���෴�ģ���� �����������ʹ֮ͼ������������
	stbi_set_flip_vertically_on_load(true);

	unsigned int texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//��ʼ�������� ����һ������ļ���
	int width, height, nrChannels;
	//stb_image.h������ͼ��Ŀ�ȡ��߶Ⱥ���ɫͨ���ĸ����������������
	unsigned char* data = stbi_load(".\\img\\cow_12_8.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//�ͷ���Դ
	stbi_image_free(data);

		/****************************************************ʹ��IBO ���о��εĻ��� ��ʱ��˳�� **************************************************************************/
	//�������� -1.0 - 1.0 ֮��
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

		//������ ��ת����
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

		//û����Ӧȫ��������
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


	/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/


	//��������������� �� ���㻺����� ��  IBO(������Ҫ���д���)
	//����VAO��VBO�Ĺ����Ǳ���� IBO�ǿ�ѡ��
	//���ʹ��IBO �����Ļ��Ʒ�ʽ��������
	unsigned int VAOS[2], VBOS[2], IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//1. �󶨶����������
	//����VAO��VBO���а�  ������Ҫ����VAO���а� Ȼ���VBO���а�
	glBindVertexArray(VAOS[0]);

	//2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	//ʹ��VBO��ʱ����Ҫ��������VBO��buffer�Ͱ�����
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);

	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	//ʹ��IBO���д����ͻ���
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//ʹ��IBO�������ݻ������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// 4. �趨��������ָ��
	//���ڶ������ݸ�ʽ���н���
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

	//���windowû�б��ر� �򲻹ܽ��л���
	while (!glfwWindowShouldClose(window)) {
		//���������¼�
		process_input(window);

		// render
	  // ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//�����߿�ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//�������ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




		//ʹ��IBO�����л��ƾ���
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




		////�۲����
		glm::mat4 viewMat = glm::mat4(1.0f);
		viewMat = glm::scale(viewMat, glm::vec3(0.1f, 1.0f, 1.0f));
		//viewMat = glm::rotate(viewMat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		viewMat = glm::translate(viewMat, glm::vec3(9.0f, 0.0f, 0.0f));

		////ͶӰ����
		//glm::mat4 projectionMat = glm::mat4(1.0f);
		//projectionMat = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		//shader.setUniformFloat("translateX", translate);
		//shader.setUniformFloat("radio", radio);
		shader.setUniformMat4("transform", trans);
		shader.setUniformMat4("view", viewMat);
		//shader.setUniformMat4("projection", projectionMat);



		//window��ʹ��˫����ļ�����������л���Ľ���
		glfwSwapBuffers(window);
		//���������Ѿ����ݵĴ����¼�
		glfwPollEvents();



	}


	// ������Դ���ͷ�

	/*glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);*/


	glDeleteVertexArrays(2, VAOS);
	glDeleteBuffers(2, VBOS);
	glDeleteBuffers(1, &IBO);
	glDeleteTextures(1, &texture0);
	shader.release();



	//����opengl������������
	glfwTerminate();
}

/*

opengl�Ļص����������ڻص����ڵı仯

*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/*

�Զ�������ڼ���window�¼��ĺ���
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
