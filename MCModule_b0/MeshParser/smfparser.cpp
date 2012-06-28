/***************************************************************************
                                smfparser.cpp
                             -------------------
    begin                : Sun Sep 19 2004
    author               : John Li in 2004
    email                : ysl@sfu.ca
 ***************************************************************************/

#include "smfparser.h"

SMFParser::SMFParser()
:m_inBuffer(NULL), m_indexBuffer(0), m_fileSize(0)
{
  m_bufferLine[0] = '\0';
  m_bufferWord[0] = '\0';
}

SMFParser::~SMFParser(){
}

// load SMF file
bool SMFParser::loadFile(char* filename){
  if (m_inBuffer != NULL)
    delete[] m_inBuffer;
  m_inBuffer = NULL;

  ifstream inFile;
  inFile.open(filename);
  inFile.precision(10);

  if (!inFile.is_open())
  {
    //cout << "SMFParser::loadFile() > Unable to open " << filename << ".\n";
    return false;
  }

  // get length of file:
  inFile.seekg (0, ios::end);
  m_fileSize = inFile.tellg();
  inFile.seekg (0, ios::beg);

  // use <stdio> if <ifstream> doesn't work
  // note: for some reason, libc++ v6 doesn't work
  if (m_fileSize == -1)
  {
    inFile.close();

    FILE *inStream = fopen(filename, "r");
    fseek(inStream, 0, SEEK_END);
    m_fileSize = ftell(inStream);
    rewind(inStream);

    m_inBuffer = new char[m_fileSize];
    fread(m_inBuffer, 1, m_fileSize, inStream);

    fclose(inStream);

    return true;
  }

  // allocate memory:
  m_inBuffer = new char[m_fileSize];

  // read data as a block:
  inFile.read (m_inBuffer, m_fileSize);

  inFile.close();

  return true;
}

// return vertices list and face list
bool SMFParser::parse(vector<vector<double> > &verticesList, vector<vector<int> > &faceList)
{
  while (m_indexBuffer < m_fileSize)
  {
    readWord();
    if (strncmp(m_bufferWord, "v", 1) == 0)
    {
      if (!parseVertices(verticesList))
        return false;
    }
    else if (strncmp(m_bufferWord, "f", 1) == 0)
    {
      if (!parseFaces(faceList))
        return false;
    }
    else// if (strncmp(m_bufferWord, "#", 1) == 0)
    {
      readLine();
    }
  }

  return true;
}

// read one line of the buffer
bool SMFParser::readLine(){
	m_bufferLine[0] = '\0';

  int i=0;
  do
		m_bufferLine[i++] = m_inBuffer[m_indexBuffer++];
	while(m_inBuffer[m_indexBuffer-1] != '\n' &&
		    i < MAX_LINE &&
				m_indexBuffer < m_fileSize);

	m_bufferLine[i-1] = '\0';

	return true;
}

// read the next word
bool SMFParser::readWord(){
  m_bufferWord[0] = '\0';
	int i=0;

	// Jump to next valid character
	while((m_inBuffer[m_indexBuffer] == '\n' ||
		     m_inBuffer[m_indexBuffer] == '\t' ||
		     m_inBuffer[m_indexBuffer] == '\r' ||
		     m_inBuffer[m_indexBuffer] == ' ') &&
				 m_indexBuffer < m_fileSize)
		m_indexBuffer++;

	// Check eof
	if(m_indexBuffer >= m_fileSize)
		return false;

	do
		m_bufferWord[i++] = m_inBuffer[m_indexBuffer++];
	while(m_inBuffer[m_indexBuffer-1] != '\n' &&
		    m_inBuffer[m_indexBuffer-1] != '\t' &&
		    m_inBuffer[m_indexBuffer-1] != '\r' &&
		    m_inBuffer[m_indexBuffer-1] != ' ' &&
//        m_inBuffer[m_indexBuffer-1] != ',' &&
//        m_inBuffer[m_indexBuffer-1] != '{' &&
//        m_inBuffer[m_indexBuffer-1] != '}' &&
//        m_inBuffer[m_indexBuffer-1] != '[' &&
//        m_inBuffer[m_indexBuffer-1] != ']' &&
        m_inBuffer[m_indexBuffer-1] != '#' &&
		    i < MAX_WORD &&
				m_indexBuffer < m_fileSize);

  // handle special "comment" case
  if (m_bufferWord[i-1] == '#' || m_bufferWord[i-1] == '\n')
  {
    if (i > 1)
    {
      m_indexBuffer--;
      m_bufferWord[i-1] = '\0';
    }
    return true;
  }

/*
  if (i == 1)   // word is a single symbol; either , { } [ ]
    m_bufferWord[i] = '\0';
  else
    m_bufferWord[i-1] = '\0';
*/
	return true;
}

// returns vertices list
bool SMFParser::parseVertices(vector<vector<double> > &verticesList){
  vector<double> vertex;

  for (int i = 0; i < 3; i++)
  {
    if (!readWord())
      return false;
    else
      vertex.push_back(atof(m_bufferWord));
  }

  verticesList.push_back(vertex);

  return true;
}
// returns face list
bool SMFParser::parseFaces(vector<vector<int> > &faceList){
  vector<int> polygon;

  for (int i = 0; i < 3; i++)
  {
    if (!readWord())
      return false;
    else
      polygon.push_back(atoi(m_bufferWord));
  }

  faceList.push_back(polygon);

  return true;
}
