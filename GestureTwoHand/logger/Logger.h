//
//  Logger.h
//  GestureTracking
//
//  Created by Alan Santos on 28/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__Logger__
#define __GestureTracking__Logger__

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>

#define LOGGER Logger::getLogger()

using namespace std;

/**
 
 This class was developed to debug the code
 using a logger that print msgs in the file.
 
 Link - https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
 
 **/
class Logger{
    
public:
    /**
     Get the current local time
     @return string as time
     */
    string getLocalTime();
    /**
     *   Logs a message
     *   @param sMessage message to be logged.
     */
    void Log(const std::string& sMessage);
    /**
     *   Variable Length Logger function
     *   @param format string for the message to be logged.
     */
    void Log( const char * format, ... );
    /**
     *   << overloaded function to Logs a message
     *   @param sMessage message to be logged.
     */
    Logger& operator<<(const string& sMessage );
    /**
     *   Funtion to create the instance of logger class.
     *   @return singleton object of Clogger class..
     */
    static Logger* getLogger();
private:
    /**
     *    Default constructor for the Logger class.
     */
    Logger();
    /**
     *   copy constructor for the Logger class.
     */
    Logger( const Logger&){};             // copy constructor is private
    /**
     *   assignment operator for the Logger class.
     */
    Logger& operator=(const Logger& ){ return *this;};  // assignment operator is private
    /**
     *   Log file name.
     **/
    static const std::string m_sFileName;
    /**
     *   Singleton logger class object pointer.
     **/
    static Logger* m_pThis;
    /**
     *   Log file stream object.
     **/
    static ofstream m_Logfile;
};
#endif /* defined(__GestureTracking__Logger__) */
