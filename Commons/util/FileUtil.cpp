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
    clearPositionsGesture();
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
    clearPositionsGesture();
    PRINT("Finished");
}

std::string
FileUtil::createFileTrack(){
    int i = readNumLastFile(FILE_TRACK);
    std::string root_dir = "../samples/gesture_" + m_NewGesture.name + "/track/gesture_";
    std::string nameFile = root_dir + m_NewGesture.name + "_track_" + MathUtil::intToString(i + 1) + ".txt";
    std::ofstream fileCreate(nameFile.c_str());
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
        fileOut.open(nameFile.c_str(), ios::in | ios::out | ios::ate);
        if(fileOut.is_open()){
            fileOut<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<"0.0 0.0 0.0 "<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
            }
        }
    } else if(m_NewGesture.numHands == 2){
        gesture = mGesturesTwoHands.back();
        nameFile = createFileTrack();
        fileOut.open(nameFile.c_str(), ios::in | ios::out | ios::ate);
        if(fileOut.is_open()){
            fileOut<<"gesture "<<m_NewGesture.name<<" hands "<<m_NewGesture.numHands<<std::endl;
            for(int j = 0; j < gesture.handOne.positions.size(); j++){
                fileOut<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<" ";
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<std::endl;
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

XnPoint3D
FileUtil::getPointFile(const std::string str, const int i){
    std::vector<std::string> coordinates;
    XnPoint3D newPoint;
    split(str, ' ', coordinates);
    newPoint.X = atof(coordinates[i].c_str());
    newPoint.Y = atof(coordinates[i + 1].c_str());
    newPoint.Z = atof(coordinates[i + 2].c_str());
    return newPoint;
}

void
FileUtil::loadGestures(std::string nameFile){
    int start_s = clock();

    std::ifstream file;
    std::string row;
    std::vector<std::string> rows;

    mGesturesOneHand.clear();
    mGesturesTwoHands.clear();

    file.open(nameFile.c_str());

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

    TIME_METHOD_EXEC("loadGestures", start_s, clock());
    PRINT("Total Templates - " << mGesturesTwoHands.size() + mGesturesOneHand.size());
}

void
FileUtil::extractGesture(std::vector<std::string> rows){

    if(rows.empty()) return;

    std::vector<std::string> tokens;
    type_gesture newGesture;
    split(rows[0],' ', tokens);
    newGesture.name = tokens[1];
    newGesture.numHands = atoi(tokens[3].c_str());

    for (int i = 1; i < rows.size(); i++) {
        newGesture.handOne.positions.push_back(getPointFile(rows[i]));
        newGesture.handTwo.positions.push_back(getPointFile(rows[i], 3));
    }

    if(newGesture.handOne.positions.size() > 0 &&
        newGesture.handTwo.positions.size() > 0) {
        if(newGesture.numHands == 1){
            mGesturesOneHand.push_back(newGesture);
        } else {
            mGesturesTwoHands.push_back(newGesture);
        }
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
FileUtil::clearPositionsGesture(){
    m_NewGesture.handOne.positions.clear();
    m_NewGesture.handTwo.positions.clear();
}

void
FileUtil::clearHandGestures(){
    mGesturesOneHand.clear();
    mGesturesTwoHands.clear();
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

void
FileUtil::saveAll(){
    std::string nameFile = "../NewSamples.txt";
    std::fstream fileOut;
    size_t n1 = mGesturesOneHand.size(), n2 = mGesturesTwoHands.size(), np1, np2, t;

    fileOut.open(nameFile.c_str(), ios::out | ios::ate);
    if(fileOut.is_open()){

        for (int i = 0; i < n1; i++){
            type_gesture gesture = mGesturesOneHand[i];
            np1 = gesture.handOne.positions.size();
            np2 = gesture.handTwo.positions.size();
            t = np1 > np2? np1 : np2;
            fileOut<<"gesture "<<gesture.name<<" hands 1"<<std::endl;
            for(int j = 0; j < t; j++){
              if(np1 > j){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<" ";
              } else {
                fileOut<<"0.0 0.0 0.0 ";
              }
              if(np2 > j){
                fileOut<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<std::endl;
              } else {
                fileOut<<"0.0 0.0 0.0"<<std::endl;
              }
            }
            fileOut<<"end"<<std::endl;
        }

        for (int i = 0; i < n2; i++){
            type_gesture gesture = mGesturesTwoHands[i];
            np1 = gesture.handOne.positions.size();
            np2 = gesture.handTwo.positions.size();
            t = np1 > np2? np1 : np2;
            fileOut<<"gesture "<<gesture.name<<" hands 2"<<std::endl;
            for(int j = 0; j < t; j++){
              if(np1 > j){
                fileOut<<gesture.handOne.positions[j].X<<" "<<gesture.handOne.positions[j].Y<<" "<<gesture.handOne.positions[j].Z<<" ";
              } else {
                fileOut<<"0.0 0.0 0.0 ";
              }
              if(np2 > j){
                fileOut<<gesture.handTwo.positions[j].X<<" "<<gesture.handTwo.positions[j].Y<<" "<<gesture.handTwo.positions[j].Z<<std::endl;
              } else {
                fileOut<<"0.0 0.0 0.0"<<std::endl;
              }
            }
            fileOut<<"end"<<std::endl;
        }
    }

    fileOut.close();
}

void
FileUtil::createFile(std::string nameFile){
    std::ofstream fileCreate(nameFile.c_str());
    fileCreate.close();
}

void
FileUtil::saveFeatureGestures(std::vector<type_gesture> gestures, std::string nameFile) {
  std::fstream fileOut;
  type_gesture sample;
  XnPoint3D pl, pr;
  fileOut.open(nameFile.c_str(), ios::out | ios::ate);
  size_t n = gestures.size(), nPos;
  // std::cout << "NSº = " << n << std::endl;
  for (size_t i = 0; i < n; i++) {
    sample = gestures[i];
    nPos = sample.handOne.positions.size();
    // std::cout << "NPº = " << nPos << std::endl;
    for (size_t j = 0; j < nPos; j++) {
      pl = sample.handOne.positions[j];
      pr = sample.handTwo.positions[j];
      fileOut << pl.X << " " << pl.Y << " " << pl.Z << " " << pr.X << " " << pr.Y << " " << pr.Z << " ";
    }
    fileOut << std::endl;
    // std::cout << "-------" << std::endl;
  }
}

void
FileUtil::saveFeauresToToolkit(std::vector<type_gesture> gestures, std::string nameFile) {
  std::fstream fileOut;
  type_gesture sample;
  XnPoint3D pl, pr;
  size_t n = gestures.size(), nPos;

  fileOut.open(nameFile.c_str(), ios::out | ios::ate);

  fileOut << "GRT_LABELLED_TIME_SERIES_CLASSIFICATION_DATA_FILE_V1.0" << std::endl;
  fileOut << "DatasetName: UFBAGRDataset" << std::endl;
  fileOut << "InfoText: This dataset contains 7 gestures (g1,g2,g3,g4,g5,g6,g7), totalizing 1099 executions." << std::endl;
  fileOut << "NumDimensions: 3" << std::endl;
  fileOut << "TotalNumTrainingExamples: 700" << std::endl;
  fileOut << "NumberOfClasses: 7" << std::endl;
  fileOut << "ClassIDsAndCounters:" << std::endl;
  fileOut << "g1 " << std::endl;
  fileOut << "g2 " << std::endl;
  fileOut << "g3 " << std::endl;
  fileOut << "g4 " << std::endl;
  fileOut << "g5 " << std::endl;
  fileOut << "g6 " << std::endl;
  fileOut << "g7 " << std::endl;
  fileOut << "UseExternalRanges: 0 " << std::endl;
  fileOut << "LabelledTimeSeriesTrainingData: " << std::endl;

  for (size_t i = 0; i < n; i++) {
    sample = gestures[i];
    fileOut << "************TIME_SERIES************" << std::endl;
    fileOut << "ClassID: " << sample.name << std::endl;
    fileOut << "TimeSeriesLength: " << sample.handOne.positions.size() << std::endl;
    fileOut << "TimeSeriesData: " << std::endl;
    for (size_t j = 0; j < sample.handOne.positions.size(); j++) {
      pl = sample.handOne.positions[j];
      fileOut << pl.X << " " << pl.Y << " " << pl.Z << std::endl;
    }
    for (size_t j = 0; j < sample.handTwo.positions.size(); j++) {
      pr = sample.handTwo.positions[j];
      fileOut << pl.X << " " << pl.Y << " " << pl.Z << " " << pr.X << " " << pr.Y << " " << pr.Z << " ";
    }
  }
}
