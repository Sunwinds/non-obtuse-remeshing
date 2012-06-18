/***************************************************************************
                                objparser.cpp
                             -------------------
    begin                : Sun Sep 19 2004
    author               : John Li in 2004
    email                : ysl@sfu.ca
 ***************************************************************************/

#include "objparser.h"

OBJParser::OBJParser()
:fn(NULL)
{
}

OBJParser::~OBJParser(){
}

// load OBJ file
bool OBJParser::loadFile(char* filename){
    this->fn = filename;
  return true;
}

// return vertices list and face list
bool OBJParser::parse(vector<vector<double> > &verticesList, vector<vector<int> > &faceList)
{
    if(!this->fn)
        return false;
    std::fstream stream(this->fn, std::ios::in);
    char temp[1024];
    char type;
    std::vector<double> vertex(3);
    std::vector<int> face(3);
    while(!stream.eof())
    {
        stream >> type;
        if(type == 'v')
        {
            stream >> vertex[0] >> vertex[1] >> vertex[2];
            verticesList.push_back(vertex);
        }
        else if(type == 'f')
        {
            stream >> face[0] >> face[1] >> face[2]; // be the same as SMF
            faceList.push_back(face);
        }
        else
        {
            stream.getline(temp, '\n');
        }
        type = '#';
    }
    stream.close();
    return true;
}
