//
//  FileUtil.h
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__FileUtil__
#define __GestureTracking__FileUtil__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <XnCppWrapper.h>
#include "../gesture/Gesture.h"
#include "../logger/Logger.h"

#define GESTURE "gesture"
#define NAME_FILE_DATA "Samples.txt"

class FileUtil {
    
private:
    
    std::vector<type_gesture> mGestures;
public:
    
    
    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    void loadGestures();
    bool isNewGesture(std::string str);
    
    
    std::vector<type_gesture> getGestures();
    XnPoint3D getPointFile(std::string str);
    
};
#endif /* defined(__GestureTracking__FileUtil__) */
