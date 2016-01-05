//
//  Gesture.h
//  GestureTracking
//
//  Created by Alan Santos on 14/02/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__Gesture__
#define __GestureTracking__Gesture__

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <XnCppWrapper.h>
#include "../logger/Logger.h"
#include "../geometry/DTW.h"
#include "../util/MathUtil.h"
#include "../util/ConstantsUtil.h"
#include "../util/FileUtil.h"

using namespace std;

class Gesture{
    
private:
    
    /*
     O construtor inicializa as mãos com um valor negativo indicando que não existe nenhuma
     mão sendo rastreada.
     */
    Gesture();
    ~Gesture();
    
public:
    
    /*
     Método responsável por limpar os vetores
     de posições das mãos que estavam sendo rastreadas.
     */
    void    clearHands();
    
    /*
     O método removeHand é chamado no momento em que a mão que estava sendo rastreada sai
     do campo de visão e deixa de ser rastreada. A mão removida é identificada através de
     um identificador da mão, o id.
     */
    void    removeHand(const int idHand);
    
    /*
     O método addHand recebe como parametro o id da mão detectada e sua respectiva posição
     no momento da detecção. A partir desse momento, a mão será rastreada para capturar a
     trajetória do movimento.
     */
    void    addHand(const int idHand, XnPoint3D position);
    
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
    void    update(const int idHand, XnPoint3D position);
    
    /*
     O método que recebe como parametro o id da mão e a posição que será adicionada
     a sua trajetória. A trajetória será utilizada para reconhecer o movimento.
     */
    void    updatePosition(const int idHand, XnPoint3D position);
    
    /*
     Método utilizado para verificar se o usuário está realizando um gesto ou se está parado.
     Eu posso ter duas mãos fazendo um movimento.
     Se uma mão está parada e a outra está realizando um movimento, então o estado é DOING.
     Se as duas mãos estão em movimento, então o estado é DOING.
     */
    void    updateState();
    
    /*
     Método updateRecognition() é utilizado para reconhecimento do gesto.
     Para isso no momento em que a mão pára, o método gesturePerformed verifica
     se algum gesto foi realizado. Em caso positivo, o gesto será classificado.
     */
    void    updateRecognition();
    
    /**
     O método verifica se algum gesto foi realizado.
     A verificação consiste em saber se o estado anterior era DOING e se a quantidade
     de posições é maior que a mínima definida.
     @return status
     **/
    bool    isGesturePerformed();
    
    /*
     Obter a trajetória do gesto, normalizar utilizando
     a centróide. Carregar todos os gestos, normalizá-los
     e comparar o realizado com os n carregados.
     */
    void    recognizeDTW();
    
    /*
     Método responsável por setar os gestos
     carregados do arquivo de dados.
     @param gestures carregados de arquivo
     */
    void    setGesturesFromFile(std::vector<type_gesture> gestures);
    
    int     m_StateGesture;
    int     m_StateGesturePrev;
    float   m_Diff;
    
    map<int, type_hand> m_Hands;
    map<int,type_hand>::iterator it;
    
    DTW*     mDTW;
    FileUtil mFileUtil;
    
    std::vector<type_gesture> mGesturesFromFile;
    
public:
    static Gesture& getInstance();
    static Gesture*	m_Instance;
    
    int     m_TimeGestureStop;

};

#endif /* defined(__GestureTracking__GestureRecognize__) */
