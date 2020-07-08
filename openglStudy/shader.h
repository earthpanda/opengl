#ifndef  SHADER_H
#define  SHADER_H


#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader
{
public:
	//程序id
	unsigned int programId;


	Shader(const char* vertexPath, const char* fragmentPath) {

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexSourceCode;
		std::string fragmentSourceCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{

			//进行文件io流的读写 并且赋值到相关string 中
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//关闭io流
			vShaderFile.close();
			fShaderFile.close();
			//将读取的内容 转为 string
			vertexSourceCode = vShaderStream.str();
			fragmentSourceCode = fShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		//将str 转成char*的样式
		const char* vShaderChars = vertexSourceCode.c_str();
		const char* fShaderChars = fragmentSourceCode.c_str();

		unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vShaderChars, NULL);
		glCompileShader(vShader);
		checkCompileShader(vShader, "VERTEX");

		unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fShaderChars, NULL);
		glCompileShader(fShader);
		checkCompileShader(fShader, "FRAGMENT");

		programId = glCreateProgram();
		glAttachShader(programId, vShader);
		glAttachShader(programId, fShader);
		glLinkProgram(programId);
		checkCompileShader(programId, "PROGRAM");


		//delete shader
		glDeleteShader(vShader);
		glDeleteShader(fShader);

	};

	void useProgram() {
		glUseProgram(programId);
	}

	void release() {
		glDeleteProgram(programId);
	}

	int getProgramId() {
		return programId;
	}

	void setUniformBool(std::string name,bool value ) {
		glUniform1i(glGetUniformLocation(programId,name.c_str()),(int)value);
	}

	void setUniformInt(std::string name, int value) {
		glUniform1i(glGetUniformLocation(programId, name.c_str()),value);
	}


	void setUniformFloat(std::string name, float value) {
		glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
	}

	void setUniformMat4(std::string name,glm::mat4 mat4) {
		glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
	}



private:

	void checkCompileShader(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024];
		//检查shader的编译	
		if (type!="PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}


		//检查program的编译
		}else{

			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}


	}


};

#endif // !
