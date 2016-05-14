//
//  FileUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "FileUtil.h"
#include "MathUtil.h"
//---------------------------------------------------------------------------
// Statics
//---------------------------------------------------------------------------
FileUtil* FileUtil::m_Instance = NULL;

FileUtil::FileUtil() {
    m_StartingStorage = false;
    m_NewGesture.handOne.id_hand = -1;
    m_NewGesture.handTwo.id_hand = -1;
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
    if(m_NewGesture.handOne.id_hand == -1){
        m_NewGesture.handOne.id_hand = idHand;
    } else if(m_NewGesture.handTwo.id_hand == -1){
        m_NewGesture.handTwo.id_hand = idHand;
    }
}

void
FileUtil::removeHand(int idHand){
    if(m_NewGesture.handOne.id_hand == idHand){
        m_NewGesture.handOne.id_hand = -1;
        m_NewGesture.handOne.positions.clear();
    } else if(m_NewGesture.handTwo.id_hand == idHand){
        m_NewGesture.handTwo.id_hand = -1;
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
FileUtil::setInfoGesture(char* name, int numHands){
    m_NewGesture.numHands = numHands;
    m_NewGesture.name = std::string(name);
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
FileUtil::createFileTrack(int i){
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
    std::fstream file;

    stopTrack();

    if(m_NewGesture.numHands == 1) {
        for (int i = mGesturesOneHand.size() - 1; i < mGesturesOneHand.size(); i++){
            gesture = mGesturesOneHand[i];
            //TODO: trocar o i passado na função createFileTrack pelo número do último arquivo salvo 
            //para evitar que os arquivos atuais sejam apagados.
            file.open(createFileTrack(i), ios::in | ios::out | ios::ate);
            if(file.is_open()){
                file<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
                for(int j = 0; j < gesture.handOne.positions.size(); j++){
                    file<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
                }
                file<<"end"<<std::endl;
                file.close();
            }
        }
    } else if(m_NewGesture.numHands == 2){
        for (int i = mGesturesTwoHands.size() - 1; i < mGesturesTwoHands.size(); i++){
            gesture = mGesturesTwoHands[i];
            //TODO: trocar o i passado na função createFileTrack pelo número do último arquivo salvo 
            //para evitar que os arquivos atuais sejam apagados.
            file.open(createFileTrack(i), ios::in | ios::out | ios::ate);
            if(file.is_open()){
                file<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
                for(int j = 0; j < gesture.handOne.positions.size(); j++){
                    file<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
                }
                file<<std::endl;
                for(int j = 0; j < gesture.handTwo.positions.size(); j++){
                    file<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<std::endl;
                }
                file<<"end"<<std::endl;
                file.close();
            }
        }
    }
    PRINT("Saved");
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
    if (found != std::string::npos) return true;
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
    std::string root_dir = "../samples/gesture_" + std::string(m_NewGesture.name);
    system(("mkdir -p " + root_dir + "/track").c_str());
    system(("mkdir " + root_dir + "/image").c_str());
    system(("mkdir " + root_dir + "/depth").c_str());
}

void
FileUtil::clear(){
    m_NewGesture.handOne.positions.clear();
    m_NewGesture.handTwo.positions.clear();
}