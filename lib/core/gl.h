//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include "type.h"
#include "logic.h"
#include "stream.h"

#include <SOIL.h>

namespace core
{
 
#pragma mark -
#pragma mark GL Util

	inline void clearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	};
	inline void createTexture(const std::string& file, GLint format=GL_RGB)
	{
		int img_width, img_height;
    	unsigned char* image = SOIL_load_image(file.c_str(),&img_width,&img_height,NULL,((format== GL_RGB)?SOIL_LOAD_RGB:SOIL_LOAD_RGBA));

    	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA, img_width, img_height); // ???
    	glTexImage2D(GL_TEXTURE_2D, 0, format, img_width, img_height, 0, format, GL_UNSIGNED_BYTE, image);
    	SOIL_free_image_data(image);

    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	};
	
	/*
	
	Reference:
	http://antongerdelan.net/opengl/vertexbuffers.html
	
	*/

	inline void enableCullFace(GLenum mode=GL_BACK)
	{
    	glFrontFace(GL_CW); // GL_CCW for counter clock-wise  
	    glEnable(GL_CULL_FACE);
    	glCullFace(mode);
	};
	inline void disableCullFace()
	{
		glDisable(GL_CULL_FACE); // cull face
	};

#pragma mark -
#pragma mark shader
	
	class Shader
	{
	public:
		Shader(){};
		
		~Shader()
		{
			glDeleteProgram(_program);
		};

		bool load(const std::string& vert, const std::string& frag);
		void begin();
		void end();

		void setUniform1f(const std::string& name, float v1);
		void setUniform2f(const std::string& name, float v1, float v2);
		void setUniform3f(const std::string& name, float v1, float v2, float v3);
		void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);

		void setUniform1i(const std::string& name, int v1);
		void setUniform2i(const std::string& name, int v1, int v2);
		void setUniform3i(const std::string& name, int v1, int v2, int v3);
		void setUniform4i(const std::string& name, int v1, int v2, int v3, int v4);

		// void setUniform2f(const std::string& name, const vec2<float>& v);
		// void setUniform3f(const std::string& name, const vec3<float>& v);
		// void setUniform4f(const std::string& name, const vec4<float>& v);

		// void setUniform2i(const std::string& name, const vec2<int>& v);
		// void setUniform3i(const std::string& name, const vec3<int>& v);
		// void setUniform4i(const std::string& name, const vec4<int>& v);

		// void setUniform4i(const std::string& name, const color<int>& v);
		// void setUniform4f(const std::string& name, const color<float>& v);

		void setUniform1iv(const std::string& name, int* v, int count = 1);
		void setUniform2iv(const std::string& name, int* v, int count = 1);
		void setUniform3iv(const std::string& name, int* v, int count = 1);
		void setUniform4iv(const std::string& name, int* v, int count = 1);
		
		void setUniform1fv(const std::string& name, float* v, int count = 1);
		void setUniform2fv(const std::string& name, float* v, int count = 1);
		void setUniform3fv(const std::string& name, float* v, int count = 1);
		void setUniform4fv(const std::string& name, float* v, int count = 1);

		//void setUniformMatrix4f(const std::string& name, const glm::mat4& m);
		const GLuint& getProgram() const { return _program; };

	private:
		bool checkCompileStatus(const GLuint& program);
		bool checkProgramLinkStatus(const GLuint& program);

		GLuint _program;
	};

#pragma mark -
#pragma mark FBO

	/*
	
	Referene:
	OpenGL 4.0 Cook Book, p143

	Allocation example:
	Fbo.allocate(GL_TEXTURE2, 512, 512);
	
	*/

	class Fbo
	{
	public:
		Fbo(){};
		~Fbo()
		{
			glDeleteFramebuffers(1,&_fbo);
		};
		void allocate(const GLenum& texunit, const int& w, const int& h, const bool& useDepth=true);
		void begin();
		void end();
	
	private:
		GLuint _fbo;
	};
}