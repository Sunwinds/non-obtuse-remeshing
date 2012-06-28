/***************************************************************************
                                offparser.cpp
                             -------------------
    begin                : Sun Sep 19 2004
    author               : John Li in 2004
    email                : ysl@sfu.ca
 ***************************************************************************/

#include "offparser.h"

OFFParser::OFFParser()
:fn(NULL)
{
}

OFFParser::~OFFParser(){
}

// load OFF file
bool OFFParser::loadFile(char* filename){
    this->fn = filename;
  return true;
}

// return vertices list and face list
bool OFFParser::parse(vector<vector<double> > &verticesList, vector<vector<int> > &faceList)
{
    if(!this->fn)
        return false;
    std::fstream stream(this->fn, std::ios::in);
    int vCount = 0;
    int fCount = 0;
    int eCount = 0;
    char temp[1024];
    stream.getline(temp, '\n'); // OFF
    stream >> vCount >> fCount >> eCount;
    std::vector<double> vertex(3);
    for(int vIdx = 0; vIdx < vCount; ++vIdx)
    {
        stream >> vertex[0] >> vertex[1] >> vertex[2];
        verticesList.push_back(vertex);
    }
    std::vector<int> face(3);
    int count;
    for(int fIdx = 0; fIdx < fCount; ++fIdx)
    {
        stream >> count;
        if(count == 3)
        {
            stream >> face[0] >> face[1] >> face[2]; // be the same as SMF
            ++face[0];
            ++face[0];
            ++face[0];
        }
        else
        {
            stream.getline(temp, '\n');
        }
        faceList.push_back(face);
    }
    stream.close();
    return true;
}
