//
//  FileUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "FileUtil.h"

//---------------------------------------------------------------------------
// Statics
//---------------------------------------------------------------------------
FileUtil* FileUtil::m_Instance = NULL;

FileUtil::FileUtil() {
    m_StartingStorage = false;
    m_NewGesture.handOne.id_hand = NOT_HAND;
    m_NewGesture.handTwo.id_hand = NOT_HAND;
}

FileUtil::~FileUtil(){}

FileUtil&
FileUtil::getInstance() {
    if(m_Instance == NULL) {
        return *(m_Instance = new FileUtil());
    }
    return *m_Instance;
}

void
FileUtil::addHand(int idHand, XnPoint3D pos){
    if(m_NewGesture.handOne.id_hand == NOT_HAND){
        m_NewGesture.handOne.id_hand = idHand;
    } else if(m_NewGesture.handTwo.id_hand == NOT_HAND){
        m_NewGesture.handTwo.id_hand = idHand;
    }
}

void
FileUtil::removeHand(int idHand){
    if(m_NewGesture.handOne.id_hand == idHand){
        m_NewGesture.handOne.id_hand = NOT_HAND;
        m_NewGesture.handOne.positions.clear();
    } else if(m_NewGesture.handTwo.id_hand == idHand){
        m_NewGesture.handTwo.id_hand = NOT_HAND;
        m_NewGesture.handTwo.positions.clear();
    }
}

void
FileUtil::addPosition(int idHand, XnPoint3D pos){
    if(m_StartingStorage){
        if(m_NewGesture.handOne.id_hand == idHand){
            m_NewGesture.handOne.positions.push_back(pos);
        } else if(m_NewGesture.handTwo.id_hand == idHand){
            m_NewGesture.handTwo.positions.push_back(pos);
        }
    }
}

void
FileUtil::setInfoGesture(std::string name, int numHands){
    m_NewGesture.numHands = numHands;
    m_NewGesture.name = name;
    createDirs();
}

void
FileUtil::startTrack(){
    m_StartingStorage = true;
    clear();
    PRINT("Tracking");
}

void
FileUtil::stopTrack(){
    m_StartingStorage = false;
    if(m_NewGesture.numHands == 1){
        mGesturesOneHand.push_back(m_NewGesture);
    } else if(m_NewGesture.numHands == 2){
        mGesturesTwoHands.push_back(m_NewGesture);
    }
    clear();
    PRINT("Finished");
}

std::string
FileUtil::createFileTrack(){
    int i = readNumLastFile(FILE_TRACK);
    std::string root_dir = "../samples/gesture_" + m_NewGesture.name + "/track/gesture_";
    std::string nameFile = root_dir + m_NewGesture.name + "_track_" + std::to_string(i + 1) + ".txt";
    std::ofstream fileCreate(nameFile);
    fileCreate.close();
    return nameFile;
}

void
FileUtil::saveTrack(){
    type_gesture gesture;
    std::string nameFile;
    std::fstream fileOut;

    stopTrack();

    if(m_NewGesture.numHands == 1) {
        gesture = mGesturesOneHand.back();
        nameFile = createFileTrack();
        fileOut.open(nameFile, ios::in | ios::out | ios::ate);
        if(fileOut.is_open()){
            fileOut<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
            }
        }
    } else if(m_NewGesture.numHands == 2){
        gesture = mGesturesTwoHands.back();
        nameFile = createFileTrack();
        fileOut.open(nameFile, ios::in | ios::out | ios::ate);
        if(fileOut.is_open()){
            fileOut<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
            }
            fileOut<<std::endl;
            for(int j = 0; j < gesture.handTwo.positions.size(); j++){
                fileOut<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<std::endl;
            }
        }
    }
    
    fileOut<<"end"<<std::endl;
    fileOut.close();

    PRINT("Saved");
}

std::vector<std::string>&
FileUtil::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

bool
FileUtil::isNewGesture(const std::string str){
    size_t found;
    found = str.find(GESTURE);
    if (found != std::string::npos) return true;
    return false;
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
FileUtil::extractGesture(std::vector<std::string> rows){

    if(rows.empty()) return;

    bool isOneHand = false;
    std::vector<std::string> tokens;
    type_gesture newGesture;
    size_t n = rows.size();
    int i = 0;
    split(rows[0],' ', tokens);
    newGesture.name = tokens[1];
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
FileUtil::createDirs(){
    m_Root_dir = "../samples/gesture_" + std::string(m_NewGesture.name);
    system(("mkdir -p " + m_Root_dir + "/track").c_str());
    system(("mkdir " + m_Root_dir + "/image").c_str());
    system(("mkdir " + m_Root_dir + "/depth").c_str());
}

void
FileUtil::clear(){
    m_NewGesture.handOne.positions.clear();
    m_NewGesture.handTwo.positions.clear();
}

int
FileUtil::readNumLastFile(int typeFile){
    int numReg = 0, aux;
    struct dirent *lsdir;
    std::vector<std::string> tokens;
    std::string nameFile, ext = ".oni";
    std::size_t found;
    DIR *dir = NULL;
    
    if(FILE_TRACK == typeFile){
        ext = ".txt";
        dir = opendir((m_Root_dir + "/track").c_str());
    } else if(FILE_DEPTH == typeFile){
        dir = opendir((m_Root_dir + "/depth").c_str());
    } else if(FILE_IMAGE == typeFile){
        dir = opendir((m_Root_dir + "/image").c_str());
    }

    while((lsdir = readdir(dir)) != NULL ){
        split(lsdir->d_name, '_', tokens);
        nameFile = tokens.back();
        found = nameFile.find(ext);
        if(found != std::string::npos){
            tokens.clear();
            split(nameFile, '.', tokens);
            aux = atoi(tokens[0].c_str());
            numReg = aux > numReg ? aux : numReg;
        }
    }

    closedir(dir);
    return numReg;
}