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

    /**
     Break a string in substrings accordir with delimiter character
     @param std::string
     @param char
     @param std::vector<std::string>
     @return std::vector<std::string>
    */
    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    /**
     Load the gestures from file
    */
    void loadGestures();

    /**
     Extrac the gestures that was readed from the file and that was stored in a vector
     of strings
     @param std::vector<std::string>
    */
    void extractGesture(std::vector<std::string> rows);

    /**
     Return the XnPoint3D from a std::string with the x,y,z positions
     @param std::string
     @return XnPoint3D
    */
    XnPoint3D getPointFile(std::string str);

    /**
     Return the template gestures of one hand
     @return std::vector<type_gesture>
    */
    std::vector<type_gesture> getGesturesOneHand() { return mGesturesOneHand; };

    /**
     Return the template gestures of two hands
     @return std::vector<type_gesture>
    */
    std::vector<type_gesture> getGesturesTwoHands() { return mGesturesTwoHands; };
};
#endif /* defined(__GestureTracking__FileUtil__) */
