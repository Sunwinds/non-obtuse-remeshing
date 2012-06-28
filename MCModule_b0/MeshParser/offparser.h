/***************************************************************************
                                offparser.h
                             -------------------
    begin                : Sun Sep 19 2004
    author               : John Li in 2004
    email                : ysl@sfu.ca
 ***************************************************************************/

#ifndef OFFPARSER_H
#define OFFPARSER_H

#include <fstream>
#include <vector>

using namespace std;

/**
  *@author John Li,galat,2006/09
  */

class OFFParser {
public: 
	OFFParser();
  ~OFFParser();

  // load OFF file
  bool loadFile(char* filename);

  // return vertices list and face list
  bool parse(vector<vector<double> > &verticesList, vector<vector<int> > &faceList);

private:
    char* fn;
};
#endif
