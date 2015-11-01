//
//  GestureRecognize.cpp
//  GestureTracking
//
//  Created by Alan Santos on 14/02/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "Gesture.h"


//---------------------------------------------------------------------------
// Statics
//---------------------------------------------------------------------------
Gesture* Gesture::m_Instance = NULL;

/*
 O construtor inicializa as mãos com um valor negativo indicando que não existe nenhuma
 mão sendo rastreada.
 */
Gesture::Gesture()
{
    m_StateGesture = GESTURE_STOPED;
}

Gesture::~Gesture(){}

Gesture&
Gesture::getInstance()
{
    if(m_Instance == NULL)
    {
        LOGGER->Log("Instance Gesture Created");
        return *(m_Instance = new Gesture());
    }
    
    return *m_Instance;
}

/*
 O método addPosition recebe como parametro o id da mão e a posição que será adicionada
 a sua trajetória. A trajetória será utilizada para reconhecer o movimento.
 */
void
Gesture::updatePosition(const int idHand, XnPoint3D position)
{
    m_Hands.at(idHand).positions.push_back(position);
}

/*
 O método addHand recebe como parametro o id da mão detectada e sua respectiva posição
 no momento da detecção. A partir desse momento, a mão será rastreada para capturar a 
 trajetória do movimento.
 */
void
Gesture::addHand(int idHand, XnPoint3D position)
{
    type_hand newHand;
    
    newHand.id_hand = idHand;
    newHand.positions.push_back(position);
    
    m_Hands.insert(pair<int, type_hand>(idHand, newHand));
    
    LOGGER->Log("Hand add");
}

/*
 O método removeHand é chamado no momento em que a mão que estava sendo rastreada sai
 do campo de visão e deixa de ser rastreada. A mão removida é identificada através de
 um identificador da mão, o id.
 */
void
Gesture::removeHand(int idHand)
{
    m_Hands.erase(idHand);
    
    LOGGER->Log("Hand removed");
}

/*
 O método update() é chamado todos os ciclos para atualizar todas as informações relacionadas
 ao estado do sistema de reconhecimento de gestos.
 
 DOING:
 Caso o usuário esteja realizando um gesto, a mão deve ser rastreada através do armazenamento
 das posições/quadros/frames anteriores para posteriormente classificar o gesto realizado.
 
 STOPED:
 Verificar se algum gesto foi realizado através da verificação da quantidade
 de posições que foram armazenadas. Caso um gesto tenha sido realizado, é necessário
 verificar qual o gesto foi realizado e classifica-lo.
 */
void
Gesture::update(const int idHand, XnPoint3D position)
{
    updatePosition(idHand, position);
    
    updateState();
    
    updateRecognition();
}

/*
 Método updateRecognition() é utilizado para reconhecimento do gesto.
 Para isso no momento em que a mão pára, o método gesturePerformed verifica
 se algum gesto foi realizado. Em caso positivo, o gesto será classificado.
 */
void
Gesture::updateRecognition()
{
    if (isGesturePerformed())
    {
        recognizeDTW();
        
        clearHands();
    }
}

/**
 O método gesturePerformed() verifica se algum gesto foi realizado.
 A verificação consiste em saber se o estado anterior era DOING e se a quantidade
 de posições é maior que a mínima definida.
 @return status
 **/
bool
Gesture::isGesturePerformed()
{
    if (m_StateGesturePrev == GESTURE_DOING && m_StateGesture == GESTURE_STOPED)
    {
        for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
            if (it->second.positions.size() >= MIN_CONTROL_POINTS) {
                LOGGER->Log("Gesture performed");
                return true;
            }
        }
    }
    
    return false;
}

/*
 Método utilizado para verificar se o usuário está realizando um gesto ou se está parado.
 Eu posso ter duas mãos fazendo um movimento.
 Se uma mão está parada e a outra está realizando um movimento, então o estado é DOING.
 Se as duas mãos estão em movimento, então o estado é DOING.
 */
void
Gesture::updateState()
{
    vector<double> diff;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            diff.push_back(MathUtil::getSumDiff(it->second.positions));
        }
    }
    
    m_Diff = MathUtil::getMaxValue(diff);
    m_StateGesturePrev = m_StateGesture;
    m_StateGesture = m_Diff > MIN_DIFF_LENGTH ? GESTURE_DOING : GESTURE_STOPED;
    
    //LOGGER->Log("State updated");
}

/*
 Obter a trajetória do gesto, normalizar utilizando
 a centróide. Carregar todos os gestos, normalizá-los
 e comparar o realizado com os n carregados.
 */
void
Gesture::recognizeDTW(){
    LOGGER->Log("Init DTW");
    
    DTW2 dtw;
    std::vector<XnPoint3D> trajectoryHand;
    std::vector<XnPoint3D> trajectoryComp;
    double distance = 0.0;
    double bestDistance = 999999999;
    type_gesture gestureRecognized;
    
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        
        //Translate the hand trajectory to origin
        trajectoryHand = MathUtil::translateToOrigin(it->second.positions);
        
        for (int i = 0; i < mGesturesFromFile.size(); i++) {
            trajectoryComp = MathUtil::translateToOrigin(mGesturesFromFile[i].positions);

            //Initialize the dynamic time warping
            dtw.init();
            
            //Set sequences that will be computed
            dtw.setSequences(trajectoryHand, trajectoryComp);
            
            //Calc dtw distance between two trajectories
            dtw.compute();
            
            //Get the best cost distance computed by dtw
            distance = dtw.getDistance();
            
            //Verify if the computed distance is lower that previous best
            if(distance < bestDistance){
                bestDistance = distance;
                gestureRecognized = mGesturesFromFile[i];
            }
        }
    }
    
    if(bestDistance > MIN_DISTANCE_TRESHOLD){
        LOGGER->Log("Gesture not recognized");
        cout<<"Gesture not recognized : "<<bestDistance<<endl;
    }else{
        LOGGER->Log("Gesture recognized: " + gestureRecognized.name);
        cout<< "Gesture: "<<gestureRecognized.name<<" Distance: "<<bestDistance<<endl;
    }
    
    LOGGER->Log("End DTW");
}

/*
 Método responsável por setar os gestos
 carregados do arquivo de dados.
 */
void
Gesture::setGesturesFromFile(std::vector<type_gesture> gestures){
    mGesturesFromFile = gestures;
}

/*
 Método responsável por limpar os vetores
 de posições das mãos que estavam sendo rastreadas.
 */
void
Gesture::clearHands(){
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        it->second.positions.clear();
    }
}