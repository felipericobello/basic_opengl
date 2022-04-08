#include "Shader.h"


Shader::Shader(const std::string& filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_ShaderID = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(m_ShaderID);
}
Shader::~Shader()
{
    Unbind();
}

void Shader::Bind() const
{
    glUseProgram(m_ShaderID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)/*Definition of a shader variable (uniform) as 4 dimentions float vector*/
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& proj)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &proj[0][0]);
}


int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_ShaderID, name.c_str());
    if (location == -1)
    {
        std::cout << "[GL ERROR]: uniform " << name << " doesn`t exist\n";
    }
    m_UniformLocationCache.insert({ name, location });
    return location;
}

Shader::ShaderProgramSource Shader::ParseShader(const std::string& filepath) /*ParseShader`s decode .shader archives as shader type enum on string stream array*/
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            if (type != ShaderType::NONE)
            {
                ss[(int)type] << line << '\n';
            }
        }
    }
    return { ss[0].str(), ss[1].str() };
}

GLuint Shader::CompileShader(GLuint type, const std::string& source) /*Shader compilation, used inside function CreateShader and could be static*/
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    /* Error handling */
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << '\n';
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) /*Definition of vertex and fragment shader inside a program (GPU code)*/
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}