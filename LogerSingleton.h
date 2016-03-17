//
//  LogerSingleton.h
//
//  Created by crh on 16-3-2.
//
//

#ifndef __test10js__LogerSingleton__
#define __test10js__LogerSingleton__

#include <stdio.h>
#include <iostream>
#include <functional>
using namespace std;

// size
struct LogerSize
{
    int width;
    int height;
};


class LogerSingleton
{
private:
    LogerSingleton();
    
    
    static LogerSingleton* m_instance;
    string strProjectPath;
    LogerSize size;
    
    void setLogerSize( char* argv );
    
public:
    static LogerSingleton* getInstance();
    void initMainData( char *argv[] );
    void initMainData( string argv );
    LogerSize getLogerSize();
    string getProjectPath();
    
};

#endif
