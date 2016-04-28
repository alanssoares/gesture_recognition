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

/*
 Método responsavél por carregar os
 gestos armazenados no arquivo de dados.
 O arquivo contém a seguinte estrutura:
 gesture nomegesto hands k
 X Y Z
 end
 gesture segundogesto k
 X Y Z
 end
 */
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
    
    cout<<"SamplesOneHand - "<<mGesturesOneHand.size()<<endl;
    cout<<"SamplesTwoHands - "<<mGesturesTwoHands.size()<<endl;
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