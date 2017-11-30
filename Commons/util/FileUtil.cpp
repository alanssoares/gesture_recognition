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
FileUtil::saveAll(std::string name){
    std::string nameFile = "../" + name + ".txt";
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
FileUtil::saveFeauresToToolkit(std::vector<type_gesture> gestures, std::string nameFile, bool series) {
  std::fstream fileOut;
  type_gesture sample;
  XnPoint3D pl, pr;
  bool msrc = false, grufba = false, msr_action_3d = false, utkinect = true;
  size_t n = gestures.size(), nPos;
  long numDimensions = 3;

  fileOut.open(nameFile.c_str(), ios::out | ios::ate);

  if(series) {
    fileOut << "GRT_LABELLED_TIME_SERIES_CLASSIFICATION_DATA_FILE_V1.0" << std::endl;
  } else {
    fileOut << "GRT_LABELLED_CLASSIFICATION_DATA_FILE_V1.0" << std::endl;
  }

  if (msrc) {
    fileOut << "DatasetName: msrc_12" << std::endl;
    fileOut << "InfoText: This dataset contains 12 gestures, totalizing 594 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 594" << std::endl;
    fileOut << "NumberOfClasses: 12" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "1 30 NOT_SET" << std::endl;
    fileOut << "10 48 NOT_SET" << std::endl;
    fileOut << "11 49 NOT_SET" << std::endl;
    fileOut << "12 50 NOT_SET" << std::endl;
    fileOut << "1 20 NOT_SET" << std::endl;
    fileOut << "2 50 NOT_SET" << std::endl;
    fileOut << "3 50 NOT_SET" << std::endl;
    fileOut << "4 50 NOT_SET" << std::endl;
    fileOut << "5 48 NOT_SET" << std::endl;
    fileOut << "6 49 NOT_SET" << std::endl;
    fileOut << "7 50 NOT_SET" << std::endl;
    fileOut << "8 50 NOT_SET" << std::endl;
    fileOut << "9 50 NOT_SET" << std::endl;
  } else if (grufba) {
    fileOut << "DatasetName: grufba" << std::endl;
    fileOut << "InfoText: This dataset contains 7 gestures, totalizing 1099 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 1099" << std::endl;
    fileOut << "NumberOfClasses: 7" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "1 182 NOT_SET" << std::endl;
    fileOut << "2 63 NOT_SET" << std::endl;
    fileOut << "3 220 NOT_SET" << std::endl;
    fileOut << "4 135 NOT_SET" << std::endl;
    fileOut << "5 194 NOT_SET" << std::endl;
    fileOut << "6 147 NOT_SET" << std::endl;
    fileOut << "7 158 NOT_SET" << std::endl;
  } else if (msr_action_3d) {
    fileOut << "DatasetName: msr_action_3d" << std::endl;
    fileOut << "InfoText: This dataset contains 19 gestures, totalizing 519 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 519" << std::endl;
    fileOut << "NumberOfClasses: 20" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "12 27 NOT_SET" << std::endl;
    fileOut << "13 27 NOT_SET" << std::endl;
    fileOut << "14 27 NOT_SET" << std::endl;
    fileOut << "15 26 NOT_SET" << std::endl;
    fileOut << "16 26 NOT_SET" << std::endl;
    fileOut << "17 26 NOT_SET" << std::endl;
    fileOut << "18 28 NOT_SET" << std::endl;
    fileOut << "19 30 NOT_SET" << std::endl;
    fileOut << "20 30 NOT_SET" << std::endl;
    fileOut << "21 30 NOT_SET" << std::endl;
    fileOut << "22 30 NOT_SET" << std::endl;
    fileOut << "23 30 NOT_SET" << std::endl;
    fileOut << "24 30 NOT_SET" << std::endl;
    fileOut << "25 30 NOT_SET" << std::endl;
    fileOut << "26 20 NOT_SET" << std::endl;
    fileOut << "27 30 NOT_SET" << std::endl;
    fileOut << "28 30 NOT_SET" << std::endl;
    fileOut << "29 30 NOT_SET" << std::endl;
    fileOut << "30 30 NOT_SET" << std::endl;
    fileOut << "31 30 NOT_SET" << std::endl;
  } else if (utkinect) {
    fileOut << "DatasetName: utkinect" << std::endl;
    fileOut << "InfoText: This dataset contains 10 gestures performed by 10 subjects twice, totalizing 200 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 200" << std::endl;
    fileOut << "NumberOfClasses: 10" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "30 20 NOT_SET" << std::endl;
    fileOut << "31 20 NOT_SET" << std::endl;
    fileOut << "32 20 NOT_SET" << std::endl;
    fileOut << "33 20 NOT_SET" << std::endl;
    fileOut << "34 20 NOT_SET" << std::endl;
    fileOut << "35 20 NOT_SET" << std::endl;
    fileOut << "36 20 NOT_SET" << std::endl;
    fileOut << "37 20 NOT_SET" << std::endl;
    fileOut << "38 20 NOT_SET" << std::endl;
    fileOut << "39 20 NOT_SET" << std::endl;
  }

  fileOut << "UseExternalRanges: 0 " << std::endl;

  if(series) {
    fileOut << "LabelledTimeSeriesTrainingData: " << std::endl;
    for (size_t i = 0; i < n; i++) {
      sample = gestures[i];
      if(series) {
        fileOut << "************TIME_SERIES************" << std::endl;
        fileOut << "ClassID: " << changeNameGesture(sample.name) << std::endl;
        fileOut << "TimeSeriesLength: " << sample.handOne.positions.size() * 2 << std::endl;
        fileOut << "TimeSeriesData: " << std::endl;
      }
      for (size_t j = 0; j < sample.handOne.positions.size(); j++) {
        pl = sample.handOne.positions[j];
        fileOut << pl.X << " " << pl.Y << " " << pl.Z << std::endl;
      }
      for (size_t j = 0; j < sample.handTwo.positions.size(); j++) {
        pr = sample.handTwo.positions[j];
        fileOut << pr.X << " " << pr.Y << " " << pr.Z << std::endl;
      }
    }
  } else {
    fileOut << "LabelledTrainingData: " << std::endl;
    for (size_t i = 0; i < n; i++) {
      sample = gestures[i];
      for (size_t j = 0; j < sample.handOne.positions.size(); j++) {
        pl = sample.handOne.positions[j];
        fileOut << sample.name[1] << " " << pl.X << " " << pl.Y << " " << pl.Z << std::endl;
      }
      for (size_t j = 0; j < sample.handTwo.positions.size(); j++) {
        pr = sample.handTwo.positions[j];
        fileOut << sample.name[1] << " "<< pr.X << " " << pr.Y << " " << pr.Z << std::endl;
      }
    }
  }
}

void
FileUtil::saveDescriptorFeauresToolkit(std::vector<type_gesture> gestures, std::string nameFile) {
  std::fstream fileOut;
  FeatureExtractor featureExtractor;
  FeatureDescriptor descriptor;
  size_t n = gestures.size();
  bool msrc = false, grufba = false, msr_action_3d = true, utkinect = false;
  fileOut.open(nameFile.c_str(), ios::out | ios::ate);
  long numDimensions = 1;

  fileOut << "GRT_LABELLED_CLASSIFICATION_DATA_FILE_V1.0" << std::endl;

  if (msrc) {
    fileOut << "DatasetName: msrc_12" << std::endl;
    fileOut << "InfoText: This dataset contains 12 gestures, totalizing 594 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 594" << std::endl;
    fileOut << "NumberOfClasses: 12" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "1 30 NOT_SET" << std::endl;
    fileOut << "10 48 NOT_SET" << std::endl;
    fileOut << "11 49 NOT_SET" << std::endl;
    fileOut << "12 50 NOT_SET" << std::endl;
    fileOut << "1 20 NOT_SET" << std::endl;
    fileOut << "2 50 NOT_SET" << std::endl;
    fileOut << "3 50 NOT_SET" << std::endl;
    fileOut << "4 50 NOT_SET" << std::endl;
    fileOut << "5 48 NOT_SET" << std::endl;
    fileOut << "6 49 NOT_SET" << std::endl;
    fileOut << "7 50 NOT_SET" << std::endl;
    fileOut << "8 50 NOT_SET" << std::endl;
    fileOut << "9 50 NOT_SET" << std::endl;
  } else if (grufba) {
    fileOut << "DatasetName: grufba" << std::endl;
    fileOut << "InfoText: This dataset contains 7 gestures, totalizing 1099 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 1099" << std::endl;
    fileOut << "NumberOfClasses: 7" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "1 182 NOT_SET" << std::endl;
    fileOut << "2 63 NOT_SET" << std::endl;
    fileOut << "3 220 NOT_SET" << std::endl;
    fileOut << "4 135 NOT_SET" << std::endl;
    fileOut << "5 194 NOT_SET" << std::endl;
    fileOut << "6 147 NOT_SET" << std::endl;
    fileOut << "7 158 NOT_SET" << std::endl;
  } else if (msr_action_3d) {
    fileOut << "DatasetName: msr_action_3d" << std::endl;
    fileOut << "InfoText: This dataset contains 19 gestures, totalizing 519 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 519" << std::endl;
    fileOut << "NumberOfClasses: 19" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "12 27 NOT_SET" << std::endl;
    fileOut << "13 27 NOT_SET" << std::endl;
    fileOut << "14 27 NOT_SET" << std::endl;
    fileOut << "15 26 NOT_SET" << std::endl;
    fileOut << "16 26 NOT_SET" << std::endl;
    fileOut << "17 26 NOT_SET" << std::endl;
    fileOut << "18 27 NOT_SET" << std::endl;
    fileOut << "19 30 NOT_SET" << std::endl;
    fileOut << "20 30 NOT_SET" << std::endl;
    fileOut << "21 30 NOT_SET" << std::endl;
    fileOut << "22 30 NOT_SET" << std::endl;
    fileOut << "23 30 NOT_SET" << std::endl;
    fileOut << "24 20 NOT_SET" << std::endl;
    // fileOut << "25 30 NOT_SET" << std::endl;
    fileOut << "26 20 NOT_SET" << std::endl;
    fileOut << "27 30 NOT_SET" << std::endl;
    fileOut << "28 30 NOT_SET" << std::endl;
    fileOut << "29 30 NOT_SET" << std::endl;
    fileOut << "30 30 NOT_SET" << std::endl;
    fileOut << "31 24 NOT_SET" << std::endl;
  } else if (utkinect) {
    fileOut << "DatasetName: utkinect" << std::endl;
    fileOut << "InfoText: This dataset contains 10 gestures performed by 10 subjects twice, totalizing 200 executions." << std::endl;
    fileOut << "NumDimensions: " << numDimensions << std::endl;
    fileOut << "TotalNumTrainingExamples: 200" << std::endl;
    fileOut << "NumberOfClasses: 10" << std::endl;
    fileOut << "ClassIDsAndCounters:" << std::endl;
    fileOut << "30 20 NOT_SET" << std::endl;
    fileOut << "31 20 NOT_SET" << std::endl;
    fileOut << "32 20 NOT_SET" << std::endl;
    fileOut << "33 20 NOT_SET" << std::endl;
    fileOut << "34 20 NOT_SET" << std::endl;
    fileOut << "35 20 NOT_SET" << std::endl;
    fileOut << "36 20 NOT_SET" << std::endl;
    fileOut << "37 20 NOT_SET" << std::endl;
    fileOut << "38 20 NOT_SET" << std::endl;
    fileOut << "39 20 NOT_SET" << std::endl;
  }

  fileOut << "UseExternalRanges: 0 " << std::endl;
  fileOut << "LabelledTrainingData: " << std::endl;

  std::vector<FeatureDescriptor> descriptors = featureExtractor.extractDescriptors(gestures);

  for (size_t i = 0; i < descriptors.size(); i++) {
    fileOut << changeNameGesture(descriptors[i].name) << " ";
    // fileOut << descriptors[i].centroidDerivative << " ";
    // fileOut << descriptors[i].centroidLength << " ";

    // size_t n1 = descriptors[i].lc.size();
    // size_t n2 = descriptors[i].lsc.size();
    // size_t n3 = descriptors[i].velocity.size();
    //
    // std::cout << "LC = " << n1 << '\n';
    // std::cout << "LSC = " << n2 << '\n';
    // std::cout << "VEL = " << n3 << '\n';

    size_t n1 = descriptors[i].lc.size();
    for (size_t j = 0; j < n1; j++) {
      fileOut << descriptors[i].lc[j];
      if (j + 1 < n1) {
        fileOut << " ";
      }
    }

    fileOut << " ";

    size_t n2 = descriptors[i].lsc.size();
    for (size_t j = 0; j < n2; j++) {
      fileOut << descriptors[i].lsc[j];
      if (j + 1 < n2) {
        fileOut << " ";
      }
    }

    fileOut << " ";

    size_t n3 = descriptors[i].velocity.size();
    for (size_t j = 0; j < n3; j++) {
      fileOut << descriptors[i].velocity[j];
      if (j + 1 < n3) {
        fileOut << " ";
      }
    }

    // fileOut << descriptors[i].lc;
    // fileOut << descriptors[i].lsc;
    // fileOut << descriptors[i].sumVariance;
    // fileOut << descriptors[i].sumCurvature;
    // fileOut << descriptors[i].centroidDerivative;
    // fileOut << descriptors[i].centroidLength;
    fileOut << std::endl;
  }
}

std::string
FileUtil::changeNameGesture(std::string name) {

  if (name == "g1") {
    return "1";
  } else if (name == "g2") {
    return "2";
  } else if (name == "g2A") {
    return "2";
  } else if (name == "g3A") {
    return "3";
  } else if (name == "g3") {
    return "3";
  } else if (name == "g4") {
    return "4";
  } else if (name == "g5") {
    return "5";
  } else if (name == "g6A") {
    return "6";
  } else if (name == "g6") {
    return "6";
  } else if (name == "g7") {
    return "7";
  } else if (name == "g9") {
    return "9";
  } else if (name == "g11") {
    return "11";
  }

  // MSC 12
  if (name == "10A") {
    return "10";
  } else if (name == "11A") {
    return "11";
  } else if (name == "12A") {
    return "12";
  } else if (name == "1A") {
    return "1";
  } else if (name == "2A") {
    return "2";
  } else if (name == "3A") {
    return "3";
  } else if (name == "4A") {
    return "4";
  } else if (name == "5A") {
    return "5";
  } else if (name == "6A") {
    return "6";
  } else if (name == "7A") {
    return "7";
  } else if (name == "8A") {
    return "8";
  } else if (name == "9A") {
    return "9";
  }

  // MSR Action 3D
  if (name == "a01") {
    return "12";
  } else if (name == "a02") {
    return "13";
  } else if (name == "a03") {
    return "14";
  } else if (name == "a04") {
    return "15";
  } else if (name == "a05") {
    return "16";
  } else if (name == "a06") {
    return "17";
  } else if (name == "a07") {
    return "18";
  } else if (name == "a08") {
    return "19";
  } else if (name == "a09") {
    return "20";
  } else if (name == "a10") {
    return "21";
  } else if (name == "a11") {
    return "22";
  } else if (name == "a12") {
    return "23";
  } else if (name == "a13") {
    return "24";
  } else if (name == "a14") {
    return "25";
  } else if (name == "a15") {
    return "26";
  } else if (name == "a16") {
    return "27";
  } else if (name == "a17") {
    return "28";
  } else if (name == "a18") {
    return "29";
  } else if (name == "a19") {
    return "30";
  } else if (name == "a20") {
    return "31";
  }

  return name;
}
