#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);


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
	const char* vertextPath = ".\\vertex.vert";
	const char* fragmentPath = ".\\fragment.frag";
	Shader shader(vertextPath,fragmentPath);



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

	unsigned int indices[] = {
		//��һ��������
		0,1,3,
		//�ڶ���������
		1,2,3

	};

	//������
	float trangle[] = {
		//l
		-0.5f,0.5f,0.0f,
		//r
		0.0f,-0.5f,0.0f,
		//t
		0.5f,0.5f,0.0f

	};


	/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/


	//����VAO �� VBO ����
	//����ʹ��VAO VBO��IBO ������ʹ��VAO��VBOʵ��
	unsigned int VAOS[2], VBOS[2],IBO;
	glGenVertexArrays(2, VAOS);
	glGenBuffers(2, VBOS);
	glGenBuffers(1, &IBO);

	//�󶨾���
	glBindVertexArray(VAOS[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//��������
	glBindVertexArray(VAOS[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(trangle), trangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);


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

	

	//���windowû�б��ر� �򲻹ܽ��л���
	while (!glfwWindowShouldClose(window)){
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
		shader.useProgram();

		

		//ʹ��IBO�����л��ƾ���
		/*glBindVertexArray(VAOS[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/


		float timeValue = glfwGetTime();
		float translate = sin(timeValue) / 2 + 0.5f;
		shader.setUniformFloat("translateX", translate);
		//ʹ��VBO�����������εĻ���
		glBindVertexArray(VAOS[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);




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
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

/*

�Զ�������ڼ���window�¼��ĺ���
*/
void process_input(GLFWwindow* window) {
	if (glfwGetKey(window,GLFW_KEY_SPACE)==GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}