#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//������ɫ�� ʹ��string ���� ����
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//�ж���ɫ�� ʹ��string ���� ����
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";



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
	//���ӻص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//���glad�Ƿ��Ѿ���ȷ����
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {

		std::cout << "Failed to load glad " << std::endl;
		
		return -1;

	}

	//�ӿڴ�С һ��window���ڿ��Ժ��ж���ӿ� �Դﵽ������Ч��
	glViewport(0, 0, 800, 600);


	//�Զ�����ɫ�����ж���ɫ�����б���
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//��鶥����ɫ��������
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertext shader compile faild \n" << infoLog <<std::endl;
	
	}


	//����Ƭ����ɫ�����б���
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//���Ƭ����ɫ��������
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
	
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragment shader compile failed \n " << infoLog << std::endl;

	}


	//����program�� ������ɫ����Ƭ����ɫ������link
	int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// ���program link error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "program link failed \n" << infoLog << std::endl;
	}

	//�Ѿ�link�ú� ��Ҫɾ�����õ���ɫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
		-0.1f,-0.1f,0.0f,
		//rb
		0.1f,-0.1f,0.0f,
		//rt
		0.1f,0.1f,0.0f
	};

	unsigned int indices[] = {
		//��һ��������
		0,1,3,
		//�ڶ���������
		1,2,3

	};

	//float trangle[] = {




	//};


	/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/


	//����VAO �� VBO ����
	unsigned int VAOS[2], VBOS[2];

	//��������������� �� ���㻺����� ��  IBO(������Ҫ���д���)
	//����VAO��VBO�Ĺ����Ǳ���� IBO�ǿ�ѡ��
	//���ʹ��IBO �����Ļ��Ʒ�ʽ��������
	unsigned int VAO, VBO,IBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
	
	// 1. �󶨶����������
	//����VAO��VBO���а�  ������Ҫ����VAO���а� Ȼ���VBO���а�
	glBindVertexArray(VAO);

	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	//ʹ��VBO��ʱ����Ҫ��������VBO��buffer�Ͱ�����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//���ڻ�������������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	//ʹ��IBO���д����ͻ���
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//ʹ��IBO�������ݻ������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. �趨��������ָ��
	//���ڶ������ݸ�ʽ���н���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);





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
		glUseProgram(shaderProgram);
		// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glBindVertexArray(VAO); 
		//������������ٸ�����
		//���ָ��3 ������ǰ��������
		//���ʹ����ibo ���������Ļ��Ʒ�ʽ
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//�����߿�ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//�������ͼ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		//ʹ��IBO�����л���
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




		//window��ʹ��˫����ļ�����������л���Ľ���
		glfwSwapBuffers(window);
		//���������Ѿ����ݵĴ����¼�
		glfwPollEvents();
	}


	// ������Դ���ͷ�

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteProgram(shaderProgram);



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