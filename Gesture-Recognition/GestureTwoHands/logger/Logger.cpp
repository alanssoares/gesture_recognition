//
//  Logger.cpp
//  GestureTracking
//
//  Created by Alan Santos on 28/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "Logger.h"
#include <time.h>

const string Logger::m_sFileName = "Log.txt";

Logger* Logger:: m_pThis = NULL;

ofstream Logger::m_Logfile;

Logger::Logger(){
    
}

Logger*
Logger::getLogger(){
    if(m_pThis == NULL){
        m_pThis = new Logger();
        m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app );
    }
    return m_pThis;
}

void
Logger::Log( const char * format, ... )
{
    char sMessage[256];
    va_list args;
    va_start (args, format);
    vsprintf (sMessage,format, args);
    m_Logfile <<"\n"<<getLocalTime()<<":\t";
    m_Logfile << sMessage;
    va_end (args);
}

void Logger::Log( const string& sMessage )
{
    m_Logfile <<"\n"<<getLocalTime()<<":\t";
    m_Logfile << sMessage;
}

Logger& Logger::operator<<(const string& sMessage )
{
    m_Logfile <<"\n"<<getLocalTime()<<":\t";
    m_Logfile << sMessage;
    return *this;
}

string
Logger::getLocalTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    return asctime(timeinfo);
}