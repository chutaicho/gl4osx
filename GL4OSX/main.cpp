//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#include "../lib/core/core.h"
#include "config.h"
#include "BaseApp.h"

using namespace core;
using namespace App;

// Default values
bool  Config::DEBUG_MODE       = true;
int   Config::STAGE_WIDTH      = 960;
int   Config::STAGE_HEIGHT     = 540;
float Config::TIMER_INTERVAL   = 1.0;
bool  Config::WINDOW_RESIZABLE = false;
bool  Config::FULLSCREEN_MODE  = false;
bool  Config::OSC_LISTENING    = false;
std::string Config::OSC_PORT   = "7777";
int Config::GL_VERSION_MAJOR   = 4;
int Config::GL_VERSION_MINOR   = 1;
int Config::MONITOR_ID         = 0;
std::string Config::APP_PATH   = "";

BaseApp* _appPtr;

#pragma mark -
#pragma mark GLFW Callback functions

static bool _mouseIn = false;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
};
static void cb_key(GLFWwindow* window, int key, int scancode, unsigned int codepoint, int action, int mods)
{
    //std::cout << key << ", "<< scancode << ", " << codepoint << ", " << action << std::endl;
    if(codepoint == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_TAB:
                if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            default:
                _appPtr->keyInput(key);
                break;
        } 
    }
};
static void cb_mouse(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    switch(action)
    {
        default:
        case GLFW_PRESS:
            _appPtr->mousePressed(xpos, ypos, 0);
            break;
        case GLFW_RELEASE:
            _appPtr->mouseReleased(xpos, ypos, 0);
            break;
    }    
};
static void cd_mousepos(GLFWwindow* window, double xpos, double ypos)
{
    int sw,sh;
    glfwGetWindowSize(window,&sw,&sh);

    if((int)xpos >= 0 && sw >= (int)xpos && (int)ypos >= 0 && sh >= (int)ypos)
    {
        int state = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1);
        
        switch(state)
        {
            default:
            case GLFW_PRESS:
                _appPtr->mouseDragged(xpos, ypos);
                break;
            case GLFW_RELEASE:
                _appPtr->mouseMoved(xpos, ypos);
                break;
        }
    }
};
static void cb_mousein(GLFWwindow* window, int state)
{
    _mouseIn = (state == GL_TRUE)? true : false;
};
static void cb_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    _appPtr->scrolled(xoffset, yoffset);
};
static void cb_resize(GLFWwindow* window, int width, int height)
{
    glfwGetWindowSize(window,&Config::STAGE_WIDTH,&Config::STAGE_HEIGHT);
};

static void cb_fbsize(GLFWwindow* window, int width, int height)
{
    
};

#pragma mark -
#pragma mark main

int main(int argc, const char* argv[])
{
    
    // Config
    //////////////////////////////////////////////////////////////////////////////////////////////////

    std::string appLocation = argv[0];
    int pos = (int)appLocation.rfind('/');
    Config::APP_PATH = appLocation.substr(0,pos+1);
    
    core::JsonLoader json;
    std::string config_file = Config::getFullPath(App::CONFIG_JSON);

    if(json.load(config_file))
    {
        if(Config::DEBUG_MODE) printf("loading file -> %s\n", config_file.c_str());

        JsonElement& value = json.getJSON();
        JsonElement& setting = value["config"];
        Config::DEBUG_MODE       = setting["debug"].asBool();
        Config::STAGE_WIDTH      = setting["width"].asInt();
        Config::STAGE_HEIGHT     = setting["height"].asInt();
        Config::TIMER_INTERVAL   = setting["timer_interval"].asFloat();
        Config::OSC_PORT         = setting["osc_port"].asString();
        Config::OSC_LISTENING    = setting["osc_listening"].asBool();
        Config::WINDOW_RESIZABLE = setting["window_resizable"].asBool();
        Config::FULLSCREEN_MODE  = setting["fullscreen"].asBool();
        Config::MONITOR_ID       = setting["monitor_id"].asInt();
    }

    // GLFW
    //////////////////////////////////////////////////////////////////////////////////////////////////

    if(!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, (Config::WINDOW_RESIZABLE)? GL_TRUE : GL_FALSE);

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    
    if(Config::FULLSCREEN_MODE)
    {
        window = glfwCreateWindow(Config::STAGE_WIDTH, Config::STAGE_HEIGHT, "", glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        window = glfwCreateWindow(Config::STAGE_WIDTH, Config::STAGE_HEIGHT, "", NULL, NULL);
    }

    if(!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    else 
    {
        if(Config::DEBUG_MODE) printf("GLFW version %s\n",glfwGetVersionString());
    }

    glfwMakeContextCurrent(window);
    
    // GLFW - callback settings  
    //////////////////////////////////////////////////////////////////////////////////////////////////

    glfwSetKeyCallback(window, cb_key);
    glfwSetMouseButtonCallback(window,cb_mouse);
    glfwSetCursorPosCallback(window,cd_mousepos);
    glfwSetCursorEnterCallback(window,cb_mousein);
    glfwSetScrollCallback(window,cb_scroll);
    glfwSetWindowSizeCallback(window,cb_resize);
    glfwSetFramebufferSizeCallback(window, cb_fbsize);
    glfwSetWindowPos(window,0,0); // stick on the upper left corner of the screen.
    glfwSwapInterval(1);

    // for High DPI : Todo
    int sw,sh;
    glfwGetFramebufferSize(window,&sw,&sh);
    //glViewport(0,0,sw,sh);
    if(Config::DEBUG_MODE) printf("GLFW window: %d, %d & frame buffer: %d, %d\n", Config::STAGE_WIDTH, Config::STAGE_HEIGHT, sw, sh);

    // GLEW
    //////////////////////////////////////////////////////////////////////////////////////////////////

    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();

    if (glewErr != GLEW_OK)
    {
        puts((const char*)glewGetErrorString(glewErr));
        exit(EXIT_FAILURE);
    }
    else
    {
        if(Config::DEBUG_MODE)printf("GLEW version %s\n",glewGetString(GLEW_VERSION));
    }
    
    std::string version = reinterpret_cast< char const * >(glGetString(GL_VERSION));
    if(Config::DEBUG_MODE)printf("OpenGL version supported by this platform (%s) \n", glGetString(GL_VERSION));

    // Application
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    core::randomSeed();
    
    _appPtr = new BaseApp();
    _appPtr->setup();

    // Main Loop
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    while (!glfwWindowShouldClose(window))
    {
        // write a information
        std::string message = "GL " + version + " -  FPS: " + floatToString(getFPS(), 2);
        glfwSetWindowTitle(window,message.c_str());

        _appPtr->update();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        _appPtr->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Exit
    //////////////////////////////////////////////////////////////////////////////////////////////////
    delete _appPtr;
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
};