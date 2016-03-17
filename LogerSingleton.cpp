//
//  LogerSingleton.cpp
//
//  Created by crh on 16-3-2.
//
//
#include <stdlib.h>
#include "LogerSingleton.h"

LogerSingleton* LogerSingleton::m_instance = nullptr;

LogerSingleton::LogerSingleton()
{
    this->size.width = 960;
    this->size.height = 640;
    this->strProjectPath = "";
}

LogerSingleton* LogerSingleton::getInstance()
{
    if(NULL == m_instance)
    {
        if(NULL == m_instance)
        {
            m_instance = new LogerSingleton();
        }
    }
    return m_instance;
}

void LogerSingleton::initMainData( char *argv[] )
{
    
    for (int i=0; i<100; i++)
    {
        if (argv[i] == nullptr)
        {
            return;
        }
        string strargv = argv[i];
        if (strargv == "-p")
        {
            if (argv[i+1] != nullptr)
            {
                m_instance->strProjectPath = argv[i+1];
            }
        }
        else if (strargv == "-s")
        {
            m_instance->setLogerSize(argv[i+1]);
        }
    }
}

void LogerSingleton::initMainData( string argv )
{
    string temp = "";
    string tag = "";
    for (int i=0; i<argv.length(); i++)
    {
        string t = "";
        t += argv[i];
        if (t == " " || (i + 1) == argv.length())
        {
            if (tag == "-p")
            {
                m_instance->strProjectPath = temp;
                temp = "";
                tag = "";
            }
            else if ( tag == "-s" )
            {
                m_instance->setLogerSize( const_cast<char*>( temp.c_str() ) );
                temp = "";
                tag = "";
            }
            else
            {
                tag = temp;
                temp = "";
            }
        }
        else
        {
            temp += argv[i];
        }
    }
}

void LogerSingleton::setLogerSize( char* argv )
{
    if (argv == nullptr)
    {
        m_instance->size.width = 960;
        m_instance->size.height = 640;
        return;
    }
    string str = argv;
    int sizes[2] = { 0, 0 };
    int tag = 0;
    for (int i = 0; i<str.length(); i++) {
        string ts = "";
        ts += argv[i];
        if (ts == "*")
        {
            tag ++;
            continue;
        }
        sizes[tag] = sizes[tag]*10 + (str[i]-48);
        std::cout<<(str[i]-48)<<std::endl;
    }
    m_instance->size.width = sizes[0];
    m_instance->size.height = sizes[1];
}


LogerSize LogerSingleton::getLogerSize()
{
    return m_instance->size;
}

string LogerSingleton::getProjectPath()
{
    return m_instance->strProjectPath;
}