//
//  FileUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "FileUtil.h"

std::vector<std::string>&
FileUtil::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

XnPoint3D
FileUtil::getPointFile(const std::string str){
    std::vector<std::string> coordinates;
    XnPoint3D newPoint;
    split(str, ' ', coordinates);
    newPoint.X = atof(coordinates[0].c_str());
    newPoint.Y = atof(coordinates[1].c_str());
    newPoint.Z = atof(coordinates[2].c_str());
    return newPoint;
}

void
FileUtil::loadGestures(){
    std::ifstream file;
    std::string row;
    std::vector<std::string> rows;
    
    file.open(NAME_FILE_DATA);
    
    if(file.is_open()){
        while (std::getline(file, row)){
            if(row.compare("end") == 0){
                extractGesture(rows);
                rows.clear();
            } else {
                rows.push_back(row);
            }
        }
    }
    
    PRINT("SamplesOneHand - "<<mGesturesOneHand.size());
    PRINT("SamplesTwoHands - "<<mGesturesTwoHands.size());
}

void
FileUtil::extractGesture(std::vector<std::string> rows){

    if(rows.empty()) return;

    bool isOneHand = false;
    std::vector<std::string> tokens;
    type_gesture newGesture;
    size_t n = rows.size();
    int i = 0;
    split(rows[0],' ', tokens);
    newGesture.name = tokens[1];
    newGesture.numHands = atoi(tokens[3].c_str());
    isOneHand = tokens[3].compare("1") == 0? true : false;

    if(isOneHand){
        for (i = 1; i < n; i++) {
            newGesture.handOne.positions.push_back(getPointFile(rows[i]));
        }
        mGesturesOneHand.push_back(newGesture);
    } else {
        i = 1;
        //Get points of the hand one
        while(!rows[i].empty()){
            newGesture.handOne.positions.push_back(getPointFile(rows[i++]));
        }
        i++;
        //Get points of the hand two
        while(i < n && !rows[i].empty()){
            newGesture.handTwo.positions.push_back(getPointFile(rows[i++]));
        }
        mGesturesTwoHands.push_back(newGesture);
    }
}

void
FileUtil::saveAll(){
    type_gesture gesture;
    std::string nameFile = "../NewSamples.txt";
    std::fstream fileOut;
    size_t n = 0;

    fileOut.open(nameFile.c_str(), ios::out | ios::ate);
    if(fileOut.is_open()){
        
        n = mGesturesOneHand.size();
        for (int i = 0; i < n; i++){
            gesture = mGesturesOneHand[i];
            fileOut<<"gesture "<<gesture.name<<" hands "<<gesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
            }
            fileOut<<"end"<<std::endl;
        }

        n = mGesturesTwoHands.size();
        for (int i = 0; i < n; i++){
            gesture = mGesturesTwoHands[i];
            fileOut<<"gesture "<<gesture.name<<" hands "<<gesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
            }
            fileOut<<std::endl;
            for(int j = 0; j < gesture.handTwo.positions.size(); j++){
                fileOut<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<std::endl;
            }
            fileOut<<"end"<<std::endl;
        }
    }

    fileOut.close();
}