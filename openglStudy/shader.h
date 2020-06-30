#ifndef  SHADER_H
#define  SHADER_H


#include <glad/glad.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


class Shader
{
public:
	//����id
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

			//�����ļ�io���Ķ�д ���Ҹ�ֵ�����string ��
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//�ر�io��
			vShaderFile.close();
			fShaderFile.close();
			//����ȡ������ תΪ string
			vertexSourceCode = vShaderStream.str();
			fragmentSourceCode = fShaderStream.str();

		}
		catch (const std::exception&)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}





	};

private:

};

#endif // !
