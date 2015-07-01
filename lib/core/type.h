//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

namespace core
{
#pragma mark -
#pragma mark types
    
    template<class T>
    class vec2
    {
    public:
        vec2(T _x=0.f,T _y=0.f):x(_x),y(_y){};
        void set(T _x, T _y)
        {
            x = _x; y = _y;
        };
        T x,y;
    };
    template<class T>
    class vec3
    {
    public:
        vec3(T _x=0.f,T _y=0.f,T _z=0.f):x(_x),y(_y),z(_z){};
        void set(T _x, T _y, T _z)
        {
            x = _x; y = _y; z = _z;
        };
        T x,y,z;
    };
    template<class T>
    class vec4
    {
    public:
        vec4(T _x=0.f,T _y=0.f,T _z=0.f,T _w=0.f):x(_x),y(_y),z(_z),w(_w){};
        void set(T _x, T _y, T _z, T _w)
        {
            x = _x; y = _y; z = _z; w = _w;
        };
        T x,y,z,w;
    };
    template<class T>
    class rect
    {
    public:
        rect():x(0.f),y(0.f),w(0.f),h(0.f){};
        void set(T _x, T _y, T _w, T _h)
        {
            x = _x; y = _y; w = _w; h = _h;
        };
        T x,y,w,h;
    };
    template<class T>
    class color
    {
    public:
        color(T _r=0.f,T _g=0.f,T _b=0.f,T _a=0.f):r(_r),g(_g),b(_b),a(_a){};
        void set(T _r, T _g, T _b, T _a)
        {
            r = _r; g = _g; b = _b; a = _a;
        };
        T r,g,b,a;
    };
    
#pragma mark -
#pragma mark class templates

    class view
    {
    public:
        view():x(0.f),y(0.f),z(0.f),w(0.f),h(0.f){};
        ~view(){};
        
        virtual void setup()  = 0;
        virtual void update() = 0;
        virtual void draw()   = 0;
        
        float x, y, z, w, h;
    protected:
    };
};