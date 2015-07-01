//
//  Created by Takashi Aoki
//  http://takashiaoki.com
//
//  Released under the MIT license
//  http://opensource.org/licenses/mit-license.php
//

#include "stream.h"

namespace core
{

#pragma mark -
#pragma mark Directory

    Directory::~Directory()
    {
        clear();
    };
    void Directory::read(const std::string& path)
    {
        DIR* dir;
        struct dirent *ent;
        if ((dir = opendir(path.c_str())) != NULL) 
        {
            while((ent = readdir(dir)) != NULL) 
            {
                std::string f(ent->d_name);
                if(f.find('.') != 0)_list.push_back(path+"/"+f); // Ignore hidden files.
            }
            closedir(dir);
        }
        else std::cout << "Error @ Directory: could not open " << path << std::endl; /* could not open directory */
    };
    void Directory::log()
    {
        for(std::vector<std::string>::iterator it = _list.begin(); it != _list.end();it++) std::cout << *(it) << std::endl;
    };
    void Directory::clear()
    {
        _it = 0;
        _list.clear();
    };

#pragma mark -
#pragma mark Buffer

	bool Buffer::get(const std::string& file, bool binary)
	{
		bool res = false;
		//ios_b ase::openmode mode = binary ? ifstream::binary : ios_base::in;
		std::ifstream ifs(file.c_str());    
		//std::string line;

		if(!ifs.fail())
		{
			// streambuf_iterator: read everything at once, include line breaks, from the file.
			std::string str((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());
			_data = str;

			// Todo: ignore whitespace. ex) ifs.ignore(1000,' ');
			//while(std::getline(ifs, line))_data += line;
			res = true;
		}
		else std::cout << "buffer loading error" << std::endl;

		ifs.close();
		return res;
	};
	void Buffer::clear()
	{
		_data.clear();
	};

#pragma mark -
#pragma mark Json

	bool JsonLoader::load(const std::string& file)
	{
		Json::Reader read;
        bool res = false;

		if(file.find("http://")==0 || file.find("https://")==0)
		{
			std::cout << "Error: JsonLoader is only support loading local files." << std::endl; 
		}
		else
		{
			if(get(file))
			{
				if(!read.parse(_data, _value)) std::cout << "Error @ JsonLoader: " << read.getFormatedErrorMessages() << std::endl;
                else res = true;
			}
		}
        return res;
	};

#pragma mark -
#pragma mark CSV

	bool Csv::load(const std::string& file)
     {
         bool res = false;

         if(get(file))
         {
             parse(_data);
             res = true;
         }

         return res;
     };
     void Csv::save(std::string filename)
     {
         std::ofstream ofs;
         ofs.open(filename.c_str());

         for(std::vector<std::string>::iterator it = _value.begin(); it != _value.end();it++)
         {
             std::string& target = *(it);
             ofs << target << ",";
         }

         ofs << std::endl;
         ofs.close();
     };
     void Csv::clear()
     {
         _value.clear();
     };
     void Csv::add(std::string val)
     {
         _value.push_back(val);
     }; 
     const std::string& Csv::getStringFromIndex(int index) const 
     { 
         if(index > (int)_value.size()-1)
         {
             std::cout << "CSVLoader request failed: the index is not found. return index 0." << std::endl;
             return _value[index];
         }
         else return _value[index];
     };
     void Csv::parse(std::string source)
     {
         std::stringstream  lineStream(source);
         std::string        cell;
         while(std::getline(lineStream,cell,','))
         {
             _value.push_back(cell);
         }
     };
};