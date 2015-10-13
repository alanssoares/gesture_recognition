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
    vector<float> diff;
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            diff.push_back(getSumDiff(it->second.positions));
        }
    }
    
    m_Diff = getMaxValue(diff);
    m_StateGesturePrev = m_StateGesture;
    m_StateGesture = m_Diff > MIN_DIFF_LENGTH ? GESTURE_DOING : GESTURE_STOPED;
    
    LOGGER->Log("State updated");
}

/*
 Somatório das diferenças de distância euclidiana entre as n posições anteriores
 */
float
Gesture::getSumDiff(vector<XnPoint3D> positions)
{
    float total = 0.0;
    size_t n = positions.size();
    size_t len = n - MAX_HAND_CONTROL_POINTS;
    
    XnPoint3D ant = positions[n-1];
    XnPoint3D result;
    for (size_t i = len; i < n; i++){
        result = subtract(ant, positions[i]);
        total += length(result);
        ant = positions[i];
    }
    
    return total;
}

float
Gesture::getMaxValue(vector<float> values)
{
    float max = 0.0;
    for (float value : values)
    {
        if (max < value) {
            max = value;
        }
    }
    
    return max;
}

/*
 TODO: calcula a amplitude/módulo escalar de um vetor.
 */
float
Gesture::length(XnPoint3D point)
{
    return sqrt(pow(point.X, 2) + pow(point.Y, 2) + pow(point.Z, 2));
}

/*
 TODO: realiza a subtração entre dois vetores a e b.
 */
XnPoint3D
Gesture::subtract(XnPoint3D a, XnPoint3D b)
{
    a.X = a.X - b.X;
    a.Y = a.Y - b.Y;
    a.Z = a.Z - b.Z;
    
    return a;
}

/*
 TODO: Normaliza um vetor dividindo cada componente pelo 
 módulo do vetor.
 */
XnPoint3D
Gesture::normalize(XnPoint3D point)
{
    float len = length(point);
    point.X = point.X / len;
    point.Y = point.Y / len;
    point.Z = point.Z / len;
    
    return point;
}

/*
 TODO: Imprime a trajetória que foi realizada pela mão. Cada 
 ponto é normalizado utilizando o módulo de cada vetor.
 */
void
Gesture::printPoints()
{
    /**
    static int numGesture = 1;
    int i = 1;
    ofstream file;
    
    if(numGesture % 2 == 0) {
        file.open("Results/gesture"+to_string(numGesture)+".txt");
        
        for ( hand : m_Hands){
            if (!hand.positions.empty()) {
                for (XnPoint3D point : hand.positions) {
                    //point = normalize(point);
                    file << i++ <<" "<<point.X <<" "<<point.Y <<" "<<point.Z <<endl;
                }
            }
        }
        
        file.close();
    }
    
    numGesture++;
     */
}
/*
 TODO: Calcula o ângulo formado entre dois vetores a e b, com
 uma reta saindo da origem e passando pelos pontos
 */
float
Gesture::getAngleBetween2Points(XnPoint3D a, XnPoint3D b){
    float mU, mV, mUV, uv, angle;
    
    //Produto vetorial
    uv = a.X * b.X + a.Y * b.Y;
    
    //Módulo dos vetores
    mU = sqrt(pow(a.X, 2) + pow(a.Y, 2));
    mV = sqrt(pow(b.X, 2) + pow(b.Y, 2));
    
    //Produto vetorial dos módulos
    mUV = mU * mV;
    
    angle = cos(uv/mUV);
    
    angle = acos(angle);
    
    printf("angle %f\n", angle);
    
    return angle;
}

/*
 TODO: Calcula o somatorio dos angulos formados pelo conjunto de pontos S = {p1, p2, p3,...,pn}
 */
float
Gesture::getSumAngles(){
    
    float sum = 0.0;
    /*
    for (int i = 1; i < m_Hands[0].positions.size(); i++) {
        sum += getAngleBetween2Points(m_Hands[0].positions[i], m_Hands[0].positions[i-1]);
    }
    
    printf("Sum = %f\n", sum);
    */
    return sum;
}

/*
 Método responsável por calcular o centro geométrico
 da trajetória, designado como centróide. O calculo
 é realizado através da razão entre somatório dos pontos
 e o número de pontos da trajetória. A centróide fornece
 a direção e distância para realizar a translação do movimento
 afim de obter invariância de posição.
 */
XnPoint3D
Gesture::calcCentroid(vector<XnPoint3D> positions)
{
    XnPoint3D centroid;
    centroid.X = centroid.Y = centroid.Z = 0;
    for(XnPoint3D point : positions)
    {
        centroid.X += point.X;
        centroid.Y += point.Y;
        centroid.Z += point.Z;
    }
    
    centroid.X = centroid.X / positions.size();
    centroid.Y = centroid.Y / positions.size();
    centroid.Z = centroid.Z / positions.size();
    
    LOGGER->Log("Centroid X = ", centroid.X, " Y = ", centroid.Y, " Z = ", centroid.Z);
    return centroid;
}

/*
 Método responsável por normalizar a trajetória para reduzir
 o impacto de gestos realizados por pessoas com aspectos físicos
 diferentes. A normalização é realizada através de uma translação
 da trajetória para a origem (0,0,0) usando a centróide da trajetória.
 */
vector<XnPoint3D>
Gesture::translateToOrigin(vector<XnPoint3D> positions){

    XnPoint3D centroid = calcCentroid(positions);
    
    for(XnPoint3D point : positions)
    {
        point.X -= centroid.X;
        point.Y -= centroid.Y;
        point.Z -= centroid.Z;
    }
    
    return positions;
}

/*
 Obter a trajetória do gesto, normalizar utilizando
 a centróide. Carregar todos os gestos, normalizá-los
 e comparar o realizado com os n carregados.
 */
void
Gesture::recognizeDTW(){
    LOGGER->Log("Init DTW");
    
    std::vector<XnPoint3D> trajectoryHand;
    std::vector<XnPoint3D> trajectoryComp;
    double distance = 0.0;
    double bestDistance = 999999999;
    type_gesture gestureRecognized;
    
    for (it = m_Hands.begin(); it != m_Hands.end(); ++it){
        
        //Translate the hand trajectory to origin
        trajectoryHand = translateToOrigin(it->second.positions);
        //Total number of points
        unsigned long numberPoints = trajectoryHand.size();
        
        for (int i = 0; i < mGesturesFromFile.size(); i++) {
            trajectoryComp = mGesturesFromFile[i].positions;
            trajectoryComp = translateToOrigin(trajectoryComp);
            
            //Initialize the dynamic time warping
            DTW dtw(numberPoints, 0.3);
            
            //Calc dtw distance dtw between two trajectories
            distance = dtw.fastDynamic(trajectoryHand, trajectoryComp);
            
            if(distance < bestDistance){
                bestDistance = distance;
                gestureRecognized = mGesturesFromFile[i];
            }
        }
    }
    
    LOGGER->Log("Best distance: " + std::to_string(bestDistance));
    if(bestDistance > MIN_DISTANCE_TRESHOLD){
        LOGGER->Log("Gesture not recognized");
    }else{
        LOGGER->Log("Gesture recognized: " + gestureRecognized.name);
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
        calcCentroid(it->second.positions);
        it->second.positions.clear();
    }
}