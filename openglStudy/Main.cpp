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

//�ļ����� 1.0����ǰһ��͸�� ��һ��100%
float radio = 1.0f;


int main() {

	//��ʼ��glfw
	glfwInit();
	//opengl version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//opengl core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL WINDOW", NULL, NULL);

	//��鴰���Ƿ�ʱnull
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//��window����Ϊ������
	glfwMakeContextCurrent(window);
	//��ӻص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//���glad�Ƿ��Ѿ���ȷ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to load glad " << std::endl;

		return -1;

	}

	//�ӿڴ�С һ��window���ڿ��Ժ��ж���ӿ� �Դﵽ������Ч��
	glViewport(0, 0, 800, 600);


	/*const char* vertextPath = ".\\vertex.vert";
	const char* fragmentPath = ".\\fragment.frag";*/

	//�������ֺ�vs�й�ϵ �������Ĳ��Ǿ�����ļ��� ֻ���鹹���ļ��У�������ﻹ����ͬĿ¼��
	//FileSystem::getPath("resources/textures/awesomeface.png")
	const char* vertextPath = ".\\vertex.vert";
	const char* fragmentPath = ".\\fragment.frag";
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
	unsigned char* data = stbi_load(".\\container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//�ͷ���Դ
	stbi_image_free(data);


	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
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
		//.png ��RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	//�ͷ���Դ
	stbi_image_free(data);

	/*********************************************************glm ������ش���***********************************************************************/
	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	//vec = trans * vec;
	////���210
	//std::cout << vec.x <<vec.y<< vec.z << std::endl;
	





	/************************************************************************************************************************************************/


	/*****************************************************�����ζ�������************************************************************/
	//float vertices[] = {
	//	//left
	//	-0.5f,-0.5f,0.0f,
	//	//right
	//	0.5f,-0.5f,0.0f,
	//	//top
	//	0.0f,0.5f,0.0f
	//};
	/******************************************************************************************************************************/


	/*****************************************************���ƾ�������************************************************************/
	/*float vertices[] = {

		-0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		 0.5f,0.5f,0.0f,

		0.5,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		};*/

		/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/
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
	//	//��һ��������
	//	0,1,3,
	//	//�ڶ���������
	//	1,2,3

	//};

	////������
	//float trangle[] = {
	//	//l color
	//	-0.5f,0.5f,0.0f,1.0f,0.0f,0.0f,0.45f,0.55f,
	//	//b color
	//	0.0f,-0.5f,0.0f,0.0f,1.0f,0.0f,0.5f,0.45f,
	//	//r color
	//	0.5f,0.5f,0.0f,0.0f,0.0f,1.0f,0.55f,0.55f

	//}

		//������
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


	/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/


	//����VAO �� VBO ����
	//����ʹ��VAO VBO��IBO ������ʹ��VAO��VBOʵ��
	unsigned int VAOS[2], VBOS[2], IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//�󶨾���
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


	//��������
	//glBindVertexArray(VAOS[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	//glEnableVertexAttribArray(2);



	/***********************************************************��ʹ��������й��������εĻ���***************************************************************************/

		//��������������� �� ���㻺����� ��  IBO(������Ҫ���д���)
		//����VAO��VBO�Ĺ����Ǳ���� IBO�ǿ�ѡ��
		//���ʹ��IBO �����Ļ��Ʒ�ʽ��������
		//unsigned int VAO, VBO,IBO;
		//glGenVertexArrays(1, &VAO);
		//glGenBuffers(1, &VBO);
		//glGenBuffers(1, &IBO);
		//
		//// 1. �󶨶����������
		////����VAO��VBO���а�  ������Ҫ����VAO���а� Ȼ���VBO���а�
		//glBindVertexArray(VAO);

		//// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
		////ʹ��VBO��ʱ����Ҫ��������VBO��buffer�Ͱ�����
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		////���ڻ�������������
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
		////ʹ��IBO���д����ͻ���
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		////ʹ��IBO�������ݻ������
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//// 4. �趨��������ָ��
		////���ڶ������ݸ�ʽ���н���
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

	//���windowû�б��ر� �򲻹ܽ��л���
	while (!glfwWindowShouldClose(window)) {
		//���������¼�
		process_input(window);

		// render
	  // ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//����������
		//������������ٸ�����
		//���ָ��3 ������ǰ��������
		//���ʹ����ibo ���������Ļ��Ʒ�ʽ
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//�����߿�ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//�������ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




		//ʹ��IBO�����л��ƾ���
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
		//ʹ��VBO�����������εĻ���
		glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
		//����һ��������
		glDrawArrays(GL_TRIANGLES, 0, 3);


		// second transformation
	   // ---------------------
		trans = glm::mat4(1.0f); // reset it to identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		shader.setUniformMat4("transform", trans);
		//���Ƶڶ���������
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);



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

	return 0;
}



void opengl_init() {



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