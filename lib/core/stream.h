//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>

#include <json.h>

typedef Json::Value JsonElement;

namespace core
{

#pragma mark -
#pragma mark Directory

    /*
    
    Reference: 
    
    https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/readdir.3.html
    
    Usage:

    core::Directory dir;
    dir.read(IMAGE_PATH);    
    while(dir.hasNext())
    {
        std::string& path = dir.getNext();
    } 

    */

    class Directory
    {
    public:
        Directory():_it(0){};
        ~Directory();

        void read(const std::string& path);        
        void log();
        void clear();

        int size(){ return (int)_list.size(); };
        const std::string& get(const int& index){ return _list[index]; };

        bool hasNext()
        {
            bool res = (_it < size())? true : false;
            return res;
        };
        const std::string& getNext()
        {
            std::string& next = _list[_it++];
            return next;
        };

    private:
        int _it;
        std::vector<std::string> _list;
    };

#pragma mark -
#pragma mark Buffer

    class Buffer
    {
    public:
        Buffer():_data(""){};
        ~Buffer(){};

        bool get(const std::string& file, bool binary=false);
        const std::string& getRawString() const { return _data; };
        
        void clear();

        virtual bool load(const std::string& file){ return 0; };
    protected:
        std::string _data;
    };

#pragma mark -
#pragma mark JSON

    /*

    Usage:

    core::JSONLoader json;
    json.load(ofToDataPath("test.json"));   
    JsonElement& value = json.getJSON();
    std::string str = value["firstName"].asString();

    Json Test:
    http://www.jsontest.com/
    
    */

    class JsonLoader : public Buffer
    {
    public:
        JsonLoader(){};
        ~JsonLoader(){};

        bool load(const std::string& file);
        JsonElement& getJSON(){ return _value; };

    protected:
        Json::Value _value;
    };

#pragma mark -
#pragma mark CSV

	 /*

	 Usage:

	 core::Csv csv;
	 csv.load(ofToDataPath("test.csv"));
     
	 */

	 class Csv : public Buffer
	 {
	 public:
		Csv(){};
		~Csv(){};
	        
	    bool load(const std::string& file);
	    void save(std::string filename);
	    void clear();
	    void add(std::string val);
	    const std::string& getStringFromIndex(int index) const;

	    int getSize() const { return (int)_value.size(); };

	protected:
	    void parse(std::string source);
	    std::vector<std::string> _value;
	};
};