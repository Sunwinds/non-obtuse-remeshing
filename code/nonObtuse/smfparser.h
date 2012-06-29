/***************************************************************************
                                smfparser.h
                             -------------------
    begin                : Sun Sep 19 2004
    author               : John Li in 2004
    email                : ysl@sfu.ca
 ***************************************************************************/

#ifndef SMFPARSER_H
#define SMFPARSER_H

#include <fstream>
#include <vector>

using namespace std;

/**
  *@author John Li,galat,2006/09
  */

#define MAX_LINE 10000
#define MAX_WORD 1000

class SMFParser {
public: 
	SMFParser();
  ~SMFParser();

  // load SMF file
  bool loadFile(char* filename);

  // return vertices list and face list
  bool parse(vector<vector<double> > &verticesList, vector<vector<int> > &faceList);

private:
  // private methods

  // read one line of the buffer
  bool readLine();
  // read the next word
  bool readWord();
  // returns vertices list
  bool parseVertices(vector<vector<double> > &verticesList);
  /** returns face list */
  bool parseFaces(vector<vector<int> > &faceList);



  // Private attributes

  // input SMF file
  char* m_inBuffer;
  // current line
  char m_bufferLine[MAX_LINE];
  // index of current location
  int m_indexBuffer;
  // length of file
  int m_fileSize;
  // current word
  char m_bufferWord[MAX_WORD];
};
#endif
