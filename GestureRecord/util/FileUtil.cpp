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
FileUtil::setInfoGesture(char* name, int numHands){
    m_NewGesture.numHands = numHands;
    m_NewGesture.name = std::string(name);
}

void
FileUtil::startStorage(){
    m_StartingStorage = true;
    m_NewGesture.handOne.positions.clear();
    m_NewGesture.handTwo.positions.clear();
}

void
FileUtil::stopStorage(){
    m_StartingStorage = false;
    //add the gesture to vector
}

void
FileUtil::saveStorage(){
    std::fstream file;
    std::string nameFile = m_NewGesture.name + ".txt";
    file.open(nameFile, ios::in | ios::out | ios::ate);
    if(file.is_open()){
        file<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
        if(m_NewGesture.numHands == 1) {
            for(int i = 0; i < m_NewGesture.handOne.positions.size(); i++){
                file<<m_NewGesture.handOne.positions[i].X<<" "<<m_NewGesture.handOne.positions[i].Y<<" "<<m_NewGesture.handOne.positions[i].Z<<std::endl;
            }
        } else if(m_NewGesture.numHands == 2){
            for(int i = 0; i < m_NewGesture.handOne.positions.size(); i++){
                file<<m_NewGesture.handOne.positions[i].X<<" "<<m_NewGesture.handOne.positions[i].Y<<" "<<m_NewGesture.handOne.positions[i].Z<<std::endl;
            }
            file<<std::endl;
            for(int i = 0; i < m_NewGesture.handTwo.positions.size(); i++){
                file<<m_NewGesture.handTwo.positions[i].X<<" "<<m_NewGesture.handTwo.positions[i].Y<<" "<<m_NewGesture.handTwo.positions[i].Z<<std::endl;
            }
        }
        file<<"end"<<std::endl;
    }
    file.close();
}

/*
 Método responsável por quebrar uma string
 em substrings de acordo com um caracter delimitador
 */
std::vector<std::string>&
FileUtil::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) elems.push_back(item);
    }
    return elems;
}

/*
 Método responsáve por verificar se na
 string existe a palavra chave "gesture".
 Caso exista, um novo gesto foi encontrado.
 */
bool
FileUtil::isNewGesture(const std::string str){
    size_t found;
    found = str.find(GESTURE);
    
    if (found != std::string::npos) {
        return true;
    }
    
    return false;
}

/*
 Método responsável por obter um ponto x,y,z
 de uma string
 */
XnPoint3D
FileUtil::getPointFile(const std::string str){
    std::vector<std::string> coordinates;
    XnPoint3D newPoint;
    split(str, ' ', coordinates);
    newPoint.X = std::stod(coordinates[0]);
    newPoint.Y = std::stod(coordinates[1]);
    newPoint.Z = std::stod(coordinates[2]);
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