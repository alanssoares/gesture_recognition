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
    type_gesture newGesture;
    unsigned long last = 0;
    
    file.open(NAME_FILE_DATA);
    
    if(file.is_open()){
        
        while (getline(file, line)){
            
            if(line.empty()){
                continue;
            }else if(isNewGesture(line)){
                LOGGER->Log("Novo gesto encontrado no arquivo.");
                std::vector<std::string> nameGesture;
                split(line,' ', nameGesture);
                newGesture.name = nameGesture[1];
                mGestures.push_back(newGesture);
            }else{
                point = getPointFile(line);
                last = mGestures.size() - 1;
                mGestures[last].positions.push_back(point);
            }
        }
        
        LOGGER->Log("Fim de leitura do arquivo: ", NAME_FILE_DATA);
    }else{
        LOGGER->Log("Não foi possível abrir o arquivo: ", NAME_FILE_DATA);
    }
}

/*
 Método responsável por retornar
 os gestos carregados do arquivo.
 */
std::vector<type_gesture>
FileUtil::getGestures(){
    return mGestures;
}