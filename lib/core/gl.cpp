//
//  Created by Takashi Aoki on 26/12/2014
//  (c) takashiaoki.com
//
#include "gl.h"

namespace core
{
	
#pragma mark -
#pragma mark public

	bool Shader::load(const std::string& vert, const std::string& frag)
	{
		core::Buffer loader;
		loader.get(vert);

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* v_src = loader.getRawString().c_str();
		const GLchar* v_array[] = {v_src};

		glShaderSource(vs, 1, v_array, NULL);
		glCompileShader(vs);
		checkCompileStatus(vs);

		loader.get(frag);

		const GLchar* f_src = loader.getRawString().c_str();
		const GLchar* f_array[] = {f_src};

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, f_array, NULL);
		glCompileShader(fs);
		
		bool c_status = (checkCompileStatus(fs))? true : false;

		_program = glCreateProgram();
		glAttachShader(_program, vs);
		glAttachShader(_program, fs);
		glLinkProgram(_program);

		bool p_status = (checkProgramLinkStatus(_program))? true : false;
		return (p_status && c_status)? true : false;
	}
	void Shader::begin()
	{
		glUseProgram(_program);
	}
	void Shader::end()
	{
		glUseProgram(0);
	}

#pragma mark -
#pragma mark in / out

	void Shader::setUniform1f(const std::string& name, float v1)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform1f(loc,v1);
	}
	void Shader::setUniform2f(const std::string& name, float v1, float v2)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform2f(loc,v1,v2);
	}
	void Shader::setUniform3f(const std::string& name, float v1, float v2, float v3)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform3f(loc,v1,v2,v3);
	}
	void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform4f(loc,v1,v2,v3,v4);
	}
	void Shader::setUniform1i(const std::string& name, int v1)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform1i(loc,v1);
	}
	void Shader::setUniform2i(const std::string& name, int v1, int v2)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform2i(loc,v1,v2);
	}
	void Shader::setUniform3i(const std::string& name, int v1, int v2, int v3)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform3i(loc,v1,v2,v3);
	}
	void Shader::setUniform4i(const std::string& name, int v1, int v2, int v3, int v4)
	{
		GLuint loc = glGetUniformLocation(_program,name.c_str());
    	glUniform4i(loc,v1,v2,v3,v4);
	}
	
	// void Shader::setUniform2f(const std::string& name, const vec2<float>& v)
	// {

	// }
	// void Shader::setUniform3f(const std::string& name, const vec3<float>& v)
	// {

	// }
	// void Shader::setUniform4f(const std::string& name, const vec4<float>& v)
	// {

	// }
	// void Shader::setUniform2i(const std::string& name, const vec2<int>& v)
	// {

	// }
	// void Shader::setUniform3i(const std::string& name, const vec3<int>& v)
	// {

	// }
	// void Shader::setUniform4i(const std::string& name, const vec4<int>& v)
	// {

	// }
	// void Shader::setUniform4f(const std::string& name, const color<float>& v)
	// {
		
	// }


#pragma mark -
#pragma mark private

	bool Shader::checkCompileStatus(const GLuint& program)
	{
		bool res = false;

		GLint status;
		glGetShaderiv(program, GL_COMPILE_STATUS, &status);

		if(GL_FALSE == status)
		{
			fprintf(stderr, "shader compilation failed!\n");

			GLint logLen;
			glGetShaderiv(program, GL_INFO_LOG_LENGTH, &logLen);
			if( logLen > 0 )
			{
				char* log = (char *)malloc(logLen);

				GLsizei written;
				glGetShaderInfoLog(program, logLen, &written, log);
				fprintf(stderr, "Shader log:\n%s", log);
				free(log); 
			}
		}
		else res = true;
		return res;
	}
	bool Shader::checkProgramLinkStatus(const GLuint& program)
	{
		bool res = false;

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if(GL_FALSE == status)
		{
			fprintf(stderr, "Failed to link shader program!\n");

			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			if( logLen > 0 )
			{
				char* log = (char *)malloc(logLen);

				GLsizei written;
				glGetProgramInfoLog(program, logLen, &written, log);
				fprintf(stderr, "Program log:\n%s", log);
				free(log); 
			}
		}
		else res = true;
		return res;
	}

 
#pragma mark -
#pragma mark FBO

	void Fbo::allocate(const GLenum& texunit, const int& w, const int& h, const bool& useDepth)
	{
		glGenFramebuffers(1, &_fbo);
	    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	    // Create the texture object
	    GLuint renderTex;
	    glGenTextures(1, &renderTex);
	    glActiveTexture(texunit);
	    glBindTexture(GL_TEXTURE_2D, renderTex);
	    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, w, h);

	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	    // Bind the texture to the FBO
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

	    if(useDepth)
	    {
		    // Create the depth buffer
		    GLuint depthBuf;
		    glGenRenderbuffers(1, &depthBuf);
		    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
		    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

		    // Bind the depth buffer to the FBO
		    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	    }

	    // Set the targets for the fragment output variables // ??????
	    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
	    glDrawBuffers(1, drawBuffers);

	    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	    if(result != GL_FRAMEBUFFER_COMPLETE) std::cout << "Framebuffer error: " << result << std::endl;

	    // Unbind the framebuffer, and revert to default framebuffer
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};

	void Fbo::begin()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	};
	void Fbo::end()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	};
}