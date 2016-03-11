//
//  FileUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 11/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "FileUtil.h"

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

/*
 Método responsavél por carregar os
 gestos armazenados no arquivo de dados.
 O arquivo contém a seguinte estrutura:
 
 gesture nomegesto
 X Y Z
 ...
 
 gesture segundogesto
 X Y Z
 ...
 
 */
void
FileUtil::loadGestures(){
    std::ifstream file;
    std::string line;
    XnPoint3D point;
    std::vector<std::string> gestureTokens;
    type_gesture newGesture;
    unsigned long last = 0;
    bool isOneHand = false;
    file.open(NAME_FILE_DATA);
    
    if(file.is_open()){
        while (std::getline(file, line)){
            if(line.empty()){
                continue;
            } else if(isNewGesture(line)){
                LOGGER->Log("New gesture found\n");
                //Split the line with gesture name
                split(line,' ', gestureTokens);
                //Setter gesture name
                newGesture.name = gestureTokens[1];
                point = getPointFile(line);
                newGesture.positions.push_back(point);
                //Verify if is one or two hands and add to the vector
                isOneHand = gestureTokens[3].compare("1") == 0? true : false;
                if(isOneHand) {
                    mGesturesOneHand.push_back(newGesture);
                    last = mGesturesOneHand.size() - 1;
                } else {
                    mGesturesTwoHands.push_back(newGesture);
                    last = mGesturesTwoHands.size() - 1;
                }
            } else {
                point = getPointFile(line);
                if(isOneHand){
                    mGesturesOneHand[last].positions.push_back(point);
                } else {

                }
            }
        }
        
        LOGGER->Log("The file was successfully read:: " + std::string(NAME_FILE_DATA) + "\n");
    }else{
        LOGGER->Log("The file can't be open:: " + std::string(NAME_FILE_DATA) + "\n");
    }
}

void
FileUtil::saveGesture(const type_gesture gesture, const std::string fileName){
    std::fstream file;
    
    file.open(fileName, ios::in | ios::out | ios::ate);
    
    if(file.is_open()){
        file << "gesture "<< gesture.name << std::endl;
        for(int i = 0; i < gesture.positions.size(); i++){
            file << gesture.positions[i].X << " " << gesture.positions[i].Y << " " << gesture.positions[i].Z << std::endl;
        }
        file << std::endl;
        file.close();
        LOGGER->Log("The gesture was successfully saved\n");
    }else{
        LOGGER->Log("The file can't be open::" + fileName + "\n");
    }
}
void
FileUtil::printTrajectory(std::vector<XnPoint3D> trajectory){
    for (int i = 0; i < trajectory.size(); i++) {
        cout<<trajectory[i].X <<" "<<trajectory[i].Y<<" "<<trajectory[i].Z<<std::endl;
    }
    cout<<std::endl;
}
