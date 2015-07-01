//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include <iostream>

namespace App
{
	static const std::string DATA_PATH   = "data/";
    static const std::string IMAGE_PATH  = DATA_PATH + "images";
    static const std::string CONFIG_JSON = DATA_PATH + "config.json";

    static const std::string SHADER_PATH = "shader/";
    
    static const std::string SHADER_BASE_VERT = SHADER_PATH + "base.vs";
    static const std::string SHADER_BASE_FRAG = SHADER_PATH + "base.fs";
    static const std::string SHADER_BASE_GEO  = SHADER_PATH + "base.gs";

    inline std::string getDataPath(const std::string& file)
    {
        return DATA_PATH + file;
    };
    inline std::string getShaderPath(const std::string& file)
    {
        return SHADER_PATH + file;
    };

    class Config
    {
    public:
        static int STAGE_WIDTH;
        static int STAGE_HEIGHT;

        static float TIMER_INTERVAL;
        static std::string OSC_PORT;
        static bool OSC_LISTENING;
        static bool WINDOW_RESIZABLE;
        static bool FULLSCREEN_MODE;
        static bool DEBUG_MODE;
        static int GL_VERSION_MAJOR;
        static int GL_VERSION_MINOR;
        static int MONITOR_ID;
        
        //
        static std::string APP_PATH;
        static std::string getFullPath(const std::string& path)
        {
            return APP_PATH + path;
        };
    };
}