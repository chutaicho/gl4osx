//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

// TODO - solve including std lib path.
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "type.h"

namespace core
{
#pragma mark -
#pragma mark util
    
    /*
    
    Reference:
    http://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand    

    */
    inline void randomSeed()
    {
//        struct timeval tv;
//        gettimeofday(&tv, 0);
//        long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
//        srand(n);
        srand((unsigned int)time(NULL));
    };
    inline int getRandom(const float& max)
    {
        return (int)(max * rand() / (RAND_MAX + 1.0f));
    };
    inline float getRandomf(const float& min, const float& max)
    {
        return min + (rand()*(max-min)/(1.0+RAND_MAX));
    };
    inline int getRandomEX(const int& max, const int& exception)
    {
        int res = getRandom(max);
        return (res == exception)? getRandomEX(max,exception) : res;
    };
    inline int fastabs(const int& a)
    {
        int mask = a >> 31;
        return (a ^ mask) - mask;
    };
    inline int fastmax(const int& a, const int& b)
    {
        int t = (a-b);
        return a - (t & (t >> 31));
    };
    inline int fastmin(const int& a, const int& b)
    {
        int t = (a-b);
        return b + (t & (t >> 31));
    };
    inline float fastmap(const float& value, const float& inputMin, const float& inputMax, const float& outputMin, const float& outputMax) 
    {
        return ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
    };
    inline float getRadian(const float& y2, const float& y1, const float& x2, const float& x1)
    {
        return atan2(y2-y1, x2-x1);
    };
    inline float getDegree(const float& y2, const float& y1, const float& x2, const float& x1)
    {
        return atan2(y2-y1, x2-x1) * (180.0/M_PI);
    };
    // ToDo: ceil to nearest multiple value of numDevices
    // inline int getNearestM(int value, int m)
    // {
    //     return (value + m - 1) / m * m;
    // };

#pragma mark -
#pragma mark string utilities
    
    template<class T>
    inline std::string toString(const T& val)
    {
        std::ostringstream iss;
        iss << val;
        return iss.str();
    };
    inline int stringToInt(const std::string& str)
    {
        int num;
        std::istringstream iss(str);
        iss >> num;
        return num;
    };
    inline std::string floatToString(const float& f, const int& digits)
    {
        std::ostringstream oss;
        oss << std::setprecision(digits) << std::setiosflags(std::ios::fixed) << f;
        return oss.str();
    };
    inline int getNumWords(const std::string& str)
    {
        int word_count(0);
        std::stringstream ss(str);
        std::string word;
        while(ss >> word) ++word_count;
        return word_count;
    };
    template<class T>
    inline std::string vecToString(const std::vector<T>& values)
    {
        std::stringstream out;
        int n = values.size();
        out << "[";
        if(n > 0)
        {
            for(int i = 0; i < n - 1; i++) out << values[i] << ",";
            out << values[n - 1];
        }
        out << "]";
        return out.str();
    };
    inline std::string getFileExtention(const std::string& str)
    {
        return str.substr(str.find_last_of("."));
    };

#pragma mark -
#pragma mark Color

    inline color<int> hexToRGB(const int& hex)
    {
        color<int> res;  
        res.r = (hex >> 16) & 0xff;
        res.g = (hex >> 8) & 0xff;
        res.b = (hex >> 0) & 0xff;
        return res;
    };

    // 0.0 - 1.0
    inline int rgbToHex(const float& r, const float& g, const float& b)
    {
        return 0;
    };
    // 0 - 255
    inline int rgbToHex(const int& r, const int& g, const int& b)
    {
        return 0;
    };
    inline int rgbToHex(const color<float>& rgb)
    {
        return 0;
    };
    inline int rgbToHex(const color<int>& rgb)
    {
        return 0;
    };

#pragma mark -
#pragma mark easing formula

    // ToDo 
    enum LFOType
    {
        LFO_CYCLE = 0x01
    };
    class LFO
    {
    public:
        LFO(LFOType type=LFO_CYCLE):_angle(0.f),_type(type){};
        
        float out(const float& range, const float& speed)
        {
            float res = 0.f;
            res = sin(_angle) * range;
            _angle += speed;
            return res;
        };

    protected:
        float _angle;
        LFOType _type;
    };

	//! Easing equation for a simple linear tweening with no easing.
	inline float easeNone(float t)
	{
		return t;
	};
	inline float easeInSine(float t )
	{
		return -1.0 * cos(t*(M_PI*.5)) + 1.0;
	};
	inline float easeOutSine(float t)
	{
		return sin(t*M_PI*.5);
	};
	inline float easeInOutSine(float t)
	{
		return -0.5 * (cos(M_PI*t) - 1.0);
	};
	inline float easeInCubic(float t)
	{
		return t*t*t;
	};
	inline float easeOutCubic(float t)
	{
		t -= 1;
		return t*t*t + 1;
	};
	inline float easeInOutCubic(float t)
	{
		t *= 2;
		if( t < 1 )
			return 0.5f * t*t*t;
		t -= 2;
		return 0.5f*(t*t*t + 2);
	};
	inline float easeInQuint(float t)
	{
		return t*t*t*t*t;
	};
	inline float easeOutQuint(float t)
	{
		t -= 1;
		return t*t*t*t*t + 1;
	};
	inline float easeInOutQuint(float t)
	{
		t *= 2;
		if( t < 1 ) return 0.5f*t*t*t*t*t;
		else {
			t -= 2;
			return 0.5f*(t*t*t*t*t + 2);
		}
	};
	inline float easeInQuad(float t)
	{
		return t*t;
	};
	inline float easeOutQuad(float t)
	{
		return -t * ( t - 2 );
	};
	inline float easeInOutQuad(float t)
	{
		t *= 2;
		if( t < 1 ) return 0.5 * t * t;
		
		t -= 1;
		return -0.5 * ((t)*(t-2) - 1);
	};
	inline float easeInQuart(float t)
	{
		return t*t*t*t;
	};
	inline float easeOutQuart(float t)
	{
		t -= 1;
		return -(t*t*t*t - 1);
	};
	inline float easeInOutQuart(float t)
	{
		t *= 2;
		if( t < 1 ) return 0.5*t*t*t*t;
		else {
			t -= 2;
			return -0.5 * (t*t*t*t - 2);
		}
	};
	inline float easeInBack(float t, float s=1.70158)
	{
		return t * t * ((s+1)*t - s);
	};
	inline float easeOutBack(float t, float s=1.70158)
	{
		t -= 1;
		return (t*t*((s+1)*t + s) + 1);
	};
	inline float easeInOutBack(float t,float s=1.70158)
	{
		t *= 2;
		if( t < 1 ) {
			s *= 1.525;
			return 0.5*(t*t*((s+1)*t - s));
		}
		else {
			t -= 2;
			s *= 1.525;
			return 0.5*(t*t*((s+1)*t+ s) + 2);
		}
	};

#pragma mark -
#pragma mark Event

    class EventObj
    {
    public:
        EventObj():param(0){};
        ~EventObj(){};
        int param;
    };
    
    // ToDo
    template<class T>
    class CustomEvent : public EventObj
    {
    public:
        CustomEvent(){};
        ~CustomEvent(){};
        T value;
    };

    class ListenerInterface
    {
    public:
        virtual void send(core::EventObj& e) = 0;
    };

    template <class T>
    class ListenerObject : public ListenerInterface
    {
    public:
        ListenerObject(T* obj, void (T::*cbf)(core::EventObj&))
        {
            _obj = obj;
            _cbf = cbf;
        };
        virtual void send(core::EventObj& e)
        {
            (_obj->*_cbf)(e);
        };

    private:
        T* _obj;
        void (T::*_cbf)(core::EventObj&);
    };

    class EventManager
    {
        static EventManager* instance;

    private:
        typedef std::map<core::EventObj*, ListenerInterface*> Keymap;
        EventManager(){};
        ~EventManager(){};

    public:
        static EventManager* getInstance()
        {
            if(instance == NULL)
            {
                instance = new EventManager();
            }
            return instance;
        };
        static void deleteInstance()
        {
            if(instance != NULL)
            {
                delete instance;
            }
        };

        template <class T>
        void addListener(core::EventObj& event, T* obj, void (T::*cbf)(core::EventObj&))
        {
            ListenerInterface* lo = new ListenerObject<T>(obj, cbf);
            _map.insert(std::pair<core::EventObj*, ListenerInterface*>(&event,lo));
        };

        template <class T>
        void removeListener(core::EventObj& event, T* obj, void (T::*cbf)(core::EventObj&))
        {
            Keymap::iterator it = _map.find(&event);

            if (it != _map.end()) 
            {
                _map.erase(it);
            }
            else std::cout << "removeListener @ EventManager: Object not found." << std::endl;
        };

        void dispatch(core::EventObj& event)
        {
            Keymap::iterator it = _map.find(&event);

            if (it != _map.end()) 
            {
                ListenerInterface* target = it->second;
                target->send(event);
            }
            else std::cout << "notifyEvent @ EventManager: Event not found." << std::endl;
        }
    private:
        Keymap _map;
    };
};

typedef core::EventObj eEventObj; // this might be useful.

template <class T>
void AddEventManager(core::EventObj& event, T* obj, void (T::*cbf)(core::EventObj&))
{
    core::EventManager::getInstance()->addListener(event, obj, cbf);
};
template <class T>
void RemoveEventManager(core::EventObj& event, T* obj, void (T::*cbf)(core::EventObj&))
{
    core::EventManager::getInstance()->removeListener(event, obj, cbf);
};
void DispatchEvent(core::EventObj& event);

namespace core
{
    inline float getFPS()
    {
        static int frames = 0;
        static float prev = 0;

        frames++;

        float now = glfwGetTime();
        if(now - prev > 1.0 || frames == 0)
        {
            frames = 0;
            prev += 1.0;
        }
        
        return (frames/(now - prev));
    };
    inline int getHours()
    {
        time_t  rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        return timeinfo->tm_hour;
    };
    inline int getMinutes()
    {
        time_t  rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        return timeinfo->tm_min;
    };
    inline int getSeconds()
    {
        time_t  rawtime;
        struct tm* timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        return timeinfo->tm_sec;
    };
    inline std::string getLocalTime()
    {
        time_t  rawtime;
        std::string str(std::ctime(&rawtime));
        return str;
    };
    inline std::string getTimeStamp()
    {
        time_t  rawtime;
        struct tm* info;
        time(&rawtime);
        info = localtime(&rawtime);
        std::string str = toString(info->tm_hour)+":"+toString(info->tm_min)+":"+toString(info->tm_sec);
        return str;
    };

#pragma mark -
#pragma mark Timer

    static const float TIMER_INTERVAL_ONE = 1.0;
    static const float TIMER_INTERVAL_MIN = 0.1;

    class Timer
    {
    public:
        Timer():id(0),_interval(TIMER_INTERVAL_ONE),_startTime(0.f),_isStarted(false){};
        ~Timer(){};
        
        void update()
        {
            if(_isStarted)
            {
                float lap = glfwGetTime() - _startTime;

                if(lap > _interval)
                {
                    updateEvent.param = id;
                    DispatchEvent(updateEvent);        
                    start();
                }
            }
        };
        void setInterval(float interval)
        {
            _interval = (interval < TIMER_INTERVAL_MIN)? TIMER_INTERVAL_MIN : interval;
        };
        void start()
        {
            _startTime = glfwGetTime();
            _isStarted = true;
        };
        void stop()
        {
            _isStarted = false;
        };
        
        core::EventObj updateEvent;

        int id;
    private:
        float _interval, _startTime;
        bool  _isStarted;
    };
};