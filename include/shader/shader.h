#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
public:
	unsigned int ID;

	/* Constructor generates the shader */
	Shader(const char* vertexPath, const char* fragmentPath);

	/* Use/Activate the shader */
	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif


