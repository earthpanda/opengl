#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

//������ɫ�� ʹ��string ���� ����
//gl_Position ���ֵ�����Ҹ� Ӧ�����ڲ�����ģ���������Ƭ����ɫ����Ч��bug
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

//�ж���ɫ�� ʹ��string ���� ����
const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 inFragColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = inFragColor;\n"
"}\n\0";


//��������һ����ɫƬ����ɫ��
//const char* colorFragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
//"}\n\0";


//ʹ��uniform����color
//ʹ��uniform�����������������Ļ� ������program�н��������ֵ������
//������Ҫע����� uniform����ɫ����Ψһ �������ظ�
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
		std::cout << "vertext shader compile faild \n" << infoLog << std::endl;

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



	//�������ζ�����ɫ�����ж���ɫ�����б���
	unsigned int colorVertexShader;
	colorVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(colorVertexShader, 1, &colorVertexShaderSource, NULL);
	glCompileShader(colorVertexShader);

	//��鶥����ɫ��������
	glGetShaderiv(colorVertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {

		glGetShaderInfoLog(colorVertexShader, 512, NULL, infoLog);
		std::cout << "vertext shader compile faild \n" << infoLog << std::endl;

	}


	//������һ��Ƭ����ɫ���Ķ���
	unsigned int colorFragmentShader;
	colorFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(colorFragmentShader, 1, &colorFragmentShaderSource, NULL);
	glCompileShader(colorFragmentShader);


	glGetShaderiv(colorFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(colorFragmentShader, 512, NULL, infoLog);
		std::cout << "color fragment compile failed \n " << infoLog << std::endl;
	}


	//��������һ��program ��������ɫ����Ƭ����ɫ������link
	int colorShaderProgram;
	colorShaderProgram = glCreateProgram();
	glAttachShader(colorShaderProgram, vertexShader);
	glAttachShader(colorShaderProgram, colorFragmentShader);
	glLinkProgram(colorShaderProgram);

	// ���program link error
	glGetProgramiv(colorShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(colorShaderProgram, 512, NULL, infoLog);
		std::cout << "color program link failed \n" << infoLog << std::endl;
	}



	//�Ѿ�link�ú� ��Ҫɾ�����õ���ɫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteShader(colorFragmentShader);
	//glDeleteShader(colorVertexShader);
	

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

	////������
	//float trangle[] = {
	//	//l
	//	0.0f,0.0f,0.0f,
	//	//r
	//	1.0f,0.0f,0.0f,
	//	//t
	//	1.0f,1.0f,0.0f

	//};


	//������
	float trangle[] = {
		//l-c
		0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,
		//r-t
		1.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,
		//t-t
		1.0f,1.0f,0.0f, 0.0f,0.0f,1.0f

	};


	/****************************************************ʹ��IBO ���о��εĻ���**************************************************************************/


	//����VAO �� VBO ����
	//����ʹ��VAO VBO��IBO ������ʹ��VAO��VBOʵ��
	unsigned int VAOS[2], VBOS[2], IBO;
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

	//index=0 �������óɹ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	//index=1 �������óɹ�
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


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
	while (!glfwWindowShouldClose(window)) {
		//���������¼�
		process_input(window);

		// render
	  // ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//����������
		glUseProgram(shaderProgram);
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
		glBindVertexArray(VAOS[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		//������ص�program
		//��program link ��ص�fragmentShader ���Ҹ�shader�а�����ص� ��ɫ������
		glUseProgram(colorShaderProgram);
		/*float timeValue = glfwGetTime();
		float greenColor = sin(timeValue) / 2 + 0.5f;
		int uniColorLocation = glGetUniformLocation(colorShaderProgram,"uniColor");
		glUniform4f(uniColorLocation, 0.0f, greenColor, 0.0f, 1.0f);*/

		
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
	glDeleteProgram(shaderProgram);
	glDeleteProgram(colorShaderProgram);



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
		glfwSetWindowShouldClose(window, true);
	}
}