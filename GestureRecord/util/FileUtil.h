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
#include <dirent.h>
#include <XnCppWrapper.h>
#include "../util/ConstantsUtil.h"

/*
  Class created to read and write in files

 Exemple: How to remove the first line in a text file:
  awk '{$1=""; sub("",""); print}' gesture35.txt > output.txt
 
 */
class FileUtil {
    
private:

    FileUtil();
    ~FileUtil();

public:

    /*
     Break a string in substrings accordir with delimiter character
     @param std::string
     @param char
     @param std::vector<std::string>
     @return std::vector<std::string>
    */
    std::vector<std::string>& split(const std::string &s, char delim, std::vector<std::string> &elems);
    
    /*
     Clear the hands tracking
    */
    void clear();

    /*
     Create the dirs that will be used to save the all files
    */
    void createDirs();

    /*
     Set the gesture info like name and num of hands
     @param char*
     @param int
    */
    void setInfoGesture(char* name, int numHands);

    /*
     Verify if there is a new gesture from a key "gesture" word inside a string
     @param std::string
     @return bool
    */
    bool isNewGesture(std::string str);

    /*
     Extract the gestures from the vector containing the rows with all positions that
     was readed from file
     @param std::vector<std::string>
    */
    void extractGesture(std::vector<std::string> rows);
    
    /*
     Remove the hand according with the idHand
     @param int
    */
    void removeHand(int idHand);

    /*
     Add a new position in the hand according with the idHand
     @param int
     @param XnPoint3D
    */
    void addPosition(int idHand, XnPoint3D pos);

    /*
     Add a new hand detected with the initial position
     @param int
     @param XnPoint3D
    */
    void addHand(int idHand, XnPoint3D pos);
    
    /*
     Start the hand tracking
    */
    void startTrack();

    /*
     Stop the hand tracking to save
    */
    void stopTrack();

    /*
     Save the tracking with the tracking positions
    */
    void saveTrack();

    /*
     Create a new file and return the name of the file
     @return std::string
    */
    std::string createFileTrack();
    
    /*
     Get the one hand gestures
     @return std::vector<type_gesture>    
    */
    std::vector<type_gesture> getGesturesOneHand() { return mGesturesOneHand; };

    /*
     Get the two hands gestures
     @return std::vector<type_gesture>
    */
    std::vector<type_gesture> getGesturesTwoHands() { return mGesturesTwoHands; };

    /*
     Get the new gesture
     @return type_gesture
    */
    type_gesture getNewGesture() { return m_NewGesture; };

    /*
     Get a XnPoint3D from string
     @param std::string
     @return XnPoint3D
    */
    XnPoint3D getPointFile(std::string str);

    /*
     Read the num of the last file saved
     @param int
    */
    int readNumLastFile(int typeFile);

public:
    static FileUtil& getInstance();
    static FileUtil* m_Instance;
    
    bool                        m_StartingStorage;
    type_gesture                m_NewGesture;
    std::vector<type_gesture>   mGesturesOneHand;
    std::vector<type_gesture>   mGesturesTwoHands;
    std::string                 m_Root_dir;
};
#endif /* defined(__GestureTracking__FileUtil__) */
