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

    FileUtil();
    ~FileUtil();

    bool m_StartingStorage;
    type_gesture m_NewGesture;
    std::vector<type_gesture> mGesturesOneHand;
    std::vector<type_gesture> mGesturesTwoHands;

public:

    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    void setInfoGesture(char* name, int numHands);
    bool isNewGesture(std::string str);
    void extractGesture(std::vector<std::string> rows);

    void startStorage();
    void stopStorage();
    
    void removeLast();
    
    void removeHand(int idHand);
    void addPosition(int idHand, XnPoint3D pos);
    void addHand(int idHand, XnPoint3D pos);

    void saveTrack();
    std::string createFileTrack(int i);
    
    std::vector<type_gesture> getGesturesOneHand() { return mGesturesOneHand; };
    std::vector<type_gesture> getGesturesTwoHands() { return mGesturesTwoHands; };
    type_gesture getNewGesture() { return m_NewGesture; };
    XnPoint3D getPointFile(std::string str);

public:
    static FileUtil& getInstance();
    static FileUtil* m_Instance;
    
};
#endif /* defined(__GestureTracking__FileUtil__) */
