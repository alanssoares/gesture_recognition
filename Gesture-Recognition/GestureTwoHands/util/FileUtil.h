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
#include "../util/ConstantsUtil.h"
#include "../logger/Logger.h"

/*
  Class created to read and write in files

 Exemple: How to remove the first line in a text file:
  awk '{$1=""; sub("",""); print}' gesture35.txt > output.txt
 
 */
class FileUtil {
    
private:
    std::vector<type_gesture> mGesturesOneHand;
    std::vector<type_gesture> mGesturesTwoHands;
public:
    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    void loadGestures();
    void extractGesture(std::vector<std::string> rows);
    XnPoint3D getPointFile(std::string str);
    std::vector<type_gesture> getGesturesOneHand() { return mGesturesOneHand; };
    std::vector<type_gesture> getGesturesTwoHands() { return mGesturesTwoHands; };
};
#endif /* defined(__GestureTracking__FileUtil__) */
