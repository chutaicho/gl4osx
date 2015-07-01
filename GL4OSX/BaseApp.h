//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include "core.h"
#include "config.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <termios.h>

using namespace App;

class BaseApp
{
public:
	BaseApp();
	~BaseApp();

	void setup();
	void update();
	void draw();

    void mouseMoved(const float& px, const float& py);
    void mouseDragged(const float& px, const float& py){};
    void mousePressed(const float& px, const float& py, const int& button){};
    void mouseReleased(const float& px, const float& py, const int& button){};
    void scrolled(const float& xoffset, const float& yoffset){};
    void keyInput(const int& key);

	void timerHandler(core::EventObj& e);

private:
	void updateShader();

	//lo_address _reciever;
	core::Timer* _timer;
	
	core::Shader _shader1;
	core::Shader _shader2;
	core::Fbo _fbo;	

	GLuint _vao;
	GLuint _textures[2];

	float _color;
	float _sine;
	float _mouseX,_mouseY;

	GLuint _instanceVBO;
};