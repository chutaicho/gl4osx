//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#include "BaseApp.h"

static const int num = 50000;
glm::vec3 translations[num];
float _rotation = 0;

#pragma mark -
#pragma mark lo callback

//int lo_recieve(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data)
//{
//    std::cout << "osc recieved." << std::endl;
//    return 0;
//};

#pragma mark -
#pragma mark constructor & destructor

BaseApp::BaseApp()
{
    
}
BaseApp::~BaseApp()
{
    glDeleteTextures(2, _textures);
    glDeleteVertexArrays(1, &_vao);
}

#pragma mark -
#pragma mark public

void BaseApp::setup()
{
//    const char *device = "/dev/ttyS0";
//    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
//    if(fd == -1) {
//        printf( "failed to open port\n" );
//    }
    
    if(App::Config::OSC_LISTENING)
    {
//        _reciever = lo_server_thread_new(Config::OSC_PORT.c_str(), NULL);
//        lo_server_thread_add_method(_reciever, "/param", "", lo_recieve, this);
//        lo_server_thread_start(_reciever);        
    }

    _timer = new core::Timer();
    _timer->setInterval(Config::TIMER_INTERVAL);
    AddEventManager(_timer->updateEvent, this, &BaseApp::timerHandler);
    //_timer->start();

    _shader1.load(Config::getFullPath(App::SHADER_BASE_VERT), Config::getFullPath(App::SHADER_BASE_FRAG));
    _shader2.load(Config::getFullPath(App::SHADER_BASE_VERT), Config::getFullPath(SHADER_PATH + "buffer.fs"));

    int i = 0;
    while (i < num)
    {
        glm::vec3 translation;
        translation.x = core::getRandomf(-1.0, 1.0) * 20;
        translation.y = core::getRandomf(-1.0, 1.0) * 20;
        translation.z = core::getRandomf(-1.0, 1.0) * 20;
        translations[i] = translation;
        i++;
    }

    //std::cout << "--------------------> " << pID << std::endl;
    
    /*

    0 - 2
    | / |
    1 - 3

    */

    float verts[] = 
    {
//        -1.0,  1.0,  0.0,
//        -1.0, -1.0,  0.0,
//         1.0,  1.0,  0.0,
//         1.0, -1.0,  0.0,

//        -.05,  .05,  0.0,
//        -.05, -.05,  0.0,
//         .05,  .05,  0.0,
//         .05, -.05,  0.0,
        
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };
    float colors[] = 
    {
//        1.0f, 0.0f, 0.0f,
//        0.0f, 1.0f, 0.0f,
//        0.0f, 0.0f, 1.0f,
//        0.5f, 0.5f, 0.5f
        
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f
    };
    float texcoords[] = 
    {
        0.0, 0.0, // Top-left
        0.0, 1.0, // Bottom-left
        1.0, 0.0, // Top-right
        1.0, 1.0  // Bottom-right
    };
    unsigned int index[] = 
    {
//        0,1,2,
//        1,3,2,
        0, 1, 2, 2, 3, 0,
        3, 2, 6, 6, 7, 3,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 5, 6, 6, 2, 1
    };
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint p_vbo;
    GLuint c_vbo;
    GLuint t_vbo;
    GLuint i_vbo;

//------------------
    glGenBuffers(1, &p_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts) * sizeof(float), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(0);

//------------------
    glGenBuffers(1, &c_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors) * sizeof(float), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(1);

//------------------
    glGenBuffers(1, &t_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords) * sizeof(float), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    glEnableVertexAttribArray(2);

//------------------  
    glGenBuffers(1, &i_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index) * sizeof(unsigned int), index, GL_STATIC_DRAW);

    
//------------------

    glGenBuffers(1, &_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * num, &translations[0], GL_DYNAMIC_DRAW); // Todo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);   
    glVertexAttribDivisor(3, 1);

    glBindVertexArray(0);
    
//------------------

    glGenTextures(2, _textures);

//------------------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textures[0]);
    std::string f1 = App::getDataPath("images/brick_bump.jpg");
    core::createTexture(f1,GL_RGB);

    glBindTexture(GL_TEXTURE_2D, 0);

//------------------

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _textures[1]);
    std::string f2 = App::getDataPath("images/cat2.png");
    core::createTexture(f2,GL_RGBA);

    glBindTexture(GL_TEXTURE_2D, 0);

//------------------

    _fbo.allocate(GL_TEXTURE2, 512, 512);

//------------------
    //We'll specify that we want to use this program that we've attached the shaders to.
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    //glDepthFunc(GL_LESS);

    //core::enableCullFace(GL_FRONT); // don't draw back face.

    _color = 1.f;
    _sine = 0.f;
}
void BaseApp::update()
{
	_timer->update();

    static core::LFO cycle;
    _sine = cycle.out(1.0, 0.01);

    //translations[10].y = core::fastmap(_mouseY, 0, (float)App::Config::STAGE_HEIGHT, -1.0, 1.0);
    //std::cout << translations[10].z << std::endl;

    _rotation += 0.01f;//core::fastmap(_mouseY, 0, (float)App::Config::STAGE_HEIGHT, -90.0, 90.0);
}
void BaseApp::draw()
{
	// background color
    static const GLfloat color[] = { 0.2, 0.2, 0.2, 1.0f };
    glClearBufferfv(GL_COLOR, 0, color);  

    //_fbo.begin();
    //glViewport(0, 0, 512, 512);
    core::clearBuffer();

    _shader1.begin();
    {
        float sw = (float)App::Config::STAGE_WIDTH;
        float sh = (float)App::Config::STAGE_HEIGHT;

        float t = (float)glfwGetTime();
        float mx = core::fastmap(_mouseX, 0.0, sw, -1.0, 1.0);
        float my = core::fastmap(_mouseY, 0.0, sh, 1.0, -1.0);

        //std::cout << mx << ", " << my << std::endl;
        //std::cout << sw << ", " << sh << std::endl;

//--------------------

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        float cameraZ = core::fastmap(_mouseY, 0, App::Config::STAGE_HEIGHT, -60.f, 60.f);
        //model = glm::rotate(model, _rotation, glm::vec3(0.0f, 0.0f, 0.0f));

        model = glm::rotate(model, _rotation, glm::vec3(0.0f, 1.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(0.01,0.01,0.01));
        
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));
        projection = glm::perspective(45.0f, sw/sh, 0.1f, 100.0f);
        
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(_shader1.getProgram(),"model");
        GLint viewLoc = glGetUniformLocation(_shader1.getProgram(), "view");
        GLint projLoc = glGetUniformLocation(_shader1.getProgram(), "projection");
        //GLint rotLoc  = glGetUniformLocation(_shader1.getProgram(), "rotation");
        
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc,1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        
        //glm::mat4 rotation;
        //rotation = glm::rotate(model, _rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        
        //glUniformMatrix4fv(rotLoc,  1, GL_FALSE, glm::value_ptr(rotation));
        
//--------------------

        _shader1.setUniform1f("g_time",t);
        _shader1.setUniform1f("g_sin",_sine);
        _shader1.setUniform2f("mouse",mx,my);
        _shader1.setUniform2f("resolution",sw,sh);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textures[0]);
        _shader1.setUniform1i("tex1", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _textures[1]);
        _shader1.setUniform1i("tex2", 1);

        glBindVertexArray(_vao);

        int i = 0;
        while (i < num)
        {
            //translations[i].x = core::getRandomf(-1.0, 1.0);
            //translations[i].y = core::getRandomf(-1.0, 1.0);
            //translations[i].z = core::getRandomf(-1.0, 1.0);
            i++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * num, &translations[0], GL_DYNAMIC_DRAW); // Todo
        
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, num);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    _shader1.end();

    //_fbo.end();
//-----------------

    // _shader2.begin();
    // _shader2.setUniform1i("tex1", 2);

    // core::clearBuffer();
    // glBindVertexArray(_vao);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // _shader2.end();
}

#pragma mark -
#pragma mark event

void BaseApp::timerHandler(core::EventObj& e)
{
    //_color = core::getRandom(100) * 0.01;
}

#pragma mark -
#pragma mark private

void BaseApp::updateShader()
{
    if(_shader1.load(App::SHADER_BASE_VERT, App::SHADER_BASE_FRAG))
    {
        std::cout << core::getTimeStamp() << ": Completed to load shader program." << std::endl;
    }
}

#pragma mark -
#pragma mark key & mouse

void BaseApp::mouseMoved(const float& px, const float& py)
{
    _mouseX = px; _mouseY = py;
}
void BaseApp::keyInput(const int& key)
{
    switch(key)
    {
        case GLFW_KEY_SPACE:
            updateShader();
            break;
        case GLFW_KEY_UP:
            break;
        case GLFW_KEY_DOWN:
            break;
        case GLFW_KEY_LEFT:
            break;
        case GLFW_KEY_RIGHT:
            break;
        case GLFW_KEY_1:
            break;
        case GLFW_KEY_2:
            break;
        case GLFW_KEY_3:
            break;
        default:
            break;
    }
}