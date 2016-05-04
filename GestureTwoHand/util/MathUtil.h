//
//  MathUtil.h
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__MathUtil__
#define __GestureTracking__MathUtil__

#include <stdio.h>
#include <XnCppWrapper.h>
#include <vector>
#include <math.h>
#include "../util/ConstantsUtil.h"
#include "../geometry/Bezier.h"
#include "../geometry/BSpline.h"

using namespace std;

/*
 This class was created to support the development
 of Gesture Recognition Project based in methods that compare trajectories.
 */
class MathUtil {

public:
    
    /**
     Método responsável por calcular a amplitude/módulo escalar de um vetor.
     @param point da trajetória
     @return double
     */
    static double length(XnPoint3D point);
    
    /**
     Método responsável por calcular a distância entre dois pontos
     @param p1 ponto
     @param p2 ponto
     @return double distância entre os pontos
    **/
    static double getDistancePointToPoint(XnPoint3D p1, XnPoint3D p2);

    /**
     Método responsável por efetuar a subtração de dois vetores a e b.
     @param a ponto da trajetória
     @param b ponto da trajetória
     @return XnPoint3D resultado da subtração
     */
    static XnPoint3D subtract(XnPoint3D a, XnPoint3D b);
    
    /**
     Método responsável por verificar se os pontos p1 e p2 são iguais.
     @param p1 ponto
     @param p2 ponto
     @retunr bool indicando se são iguais ou não
    **/
    static bool pointsEqual(XnPoint3D p1, XnPoint3D p2);

    /**
     Método responsável por normalizar um ponto utilizando a magnitude deste.
     Normaliza um vetor dividindo cada componente pelo módulo do vetor.
     @param point da trajetória
     @return XnPoint3D normalizado
     */
    static XnPoint3D normalize(XnPoint3D point);
    
    /**
     Método responsável por calcular a distância de um ponto p a um
     segmento formado pelos pontos p1 e p2.
     @param p ponto usado para calcular a distância
     @param p1 ponto do segmento
     @param p2 ponto do segmento
     @return double indicando a distância do ponto ao segmento
    **/
    static double getDistancePointToSegment(XnPoint3D p, XnPoint3D p1, XnPoint3D p2);

    /**
     Método responsável por obter a soma das distâncias entre dois pontos
     @param positions da trajetória
     @return double com soma
    */
    static double getSumDistanceBetweenPoints(vector<XnPoint3D> positions);

    /**
     Método responsável por obter os valores mínimos de X, Y, Z 
     do array de posições.
     @param positions da trajetória
     @return XnPoint3D valores mínimos de X, Y, Z
     */
    static XnPoint3D minValueXYZ(vector<XnPoint3D> positions);
    
    /**
     Método responsável por obter os valores máximos de X, Y, Z
     do array de posições da trajetória.
     @param positions da trajetória
     @return XnPoint3D valores máximos de X, Y, Z
     */
    static XnPoint3D maxValueXYZ(vector<XnPoint3D> positions);
    
    /**
     Método responsável por normalizar a trajetória no intervalo de -1 a 1.
     @param positions da trajetória
     @return vector<XnPoint3D> normalizado
     */
    static vector<XnPoint3D> normalizeTrajectory(vector<XnPoint3D> positions);

    /**
     Método responsável por obter o ângulo entre dois pontos a e b.
     Calcula o ângulo formado entre dois vetores a e b, com
     uma reta saindo da origem e passando pelos pontos
     @param a posição da trajetória
     @param b posição da trajetória
     @return double representando o ângulo
     */
    static double getAngleBetween2Points(XnPoint3D a, XnPoint3D b);
    
    /**
     Método responsável por obter o valor máximo de um vetor
     @param values com valores
     @return double valor máximo encontrado
     */
    static double getMaxValue(std::vector<double> values);
    
    /**
     Método responsável por obter a soma das diferenças das distâncias
     da trajetória.
     @param positions da trajetória
     @return double soma das distâncias dos n pontos da trajetória
     */
    static bool isGestureDoing(vector<XnPoint3D> positions);
    
    /**
     Método responsável por calcular o centro geométrico
     da trajetória, designado como centróide. O calculo
     é realizado através da razão entre somatório dos pontos
     e o número de pontos da trajetória. A centróide fornece
     a direção e distância para realizar a translação do movimento
     afim de obter invariância de posição.
     @param positions da trajetória
     @return XnPoint3D centróide encontrada a partir da trajetória
     */
    static XnPoint3D calcCentroid(vector<XnPoint3D> positions);
    
    /**
     Método responsável por realizar o deslocamento da trajetória para a
     origem utilizando a centróide da trajetória.
     Método responsável por normalizar a trajetória para reduzir
     o impacto de gestos realizados por pessoas com aspectos físicos
     diferentes. A normalização é realizada através de uma translação
     da trajetória para a origem (0,0,0) usando a centróide da trajetória.
     @param positions da trajetória
     @return vector<XnPoint3D> deslocada para a origem
     */
    static vector<XnPoint3D> translateToOrigin(vector<XnPoint3D> positions);
    
    /*
     Método responsável por aplicar interpolação cúbica de Bézier sobre
     n pontos da trajetória
     @param positions of the trajectory
     @return std::vector<XnPoint3D> da nova trajetória interpolada
     */
    static std::vector<XnPoint3D> applyCubicBezier(std::vector<XnPoint3D> positions);
    
    /*
     Método responsável por aplicar uma interpolação cúbica usando B-Spline
     @param positions of the trajectory
     @return std::vector<XnPoint3D> da nova trajetória interpolada
    */
    static std::vector<XnPoint3D> applyCubicBSpline(std::vector<XnPoint3D> positions);
    
    /*
     Método responsável por normalizar a trajetória usando a média da primeira vizinhança, ou seja
     P(n - 1), P(n), P(n + 1).
     @param positions of the trajectory
     @param numTimes of the smooth will be apllied
     @return std::vector<XnPoint3D> da nova trajetória filtrada usando a média da primeira vizinhança
     */
    static std::vector<XnPoint3D> smoothMeanNeighboring(std::vector<XnPoint3D> positions, int numTimes = 1);

    /**
     Método responsável por reduzir a quantidade de pontos da trajetória e
     manter as características da curva
     @ref http://mourner.github.io/simplify-js/
     @param points da trajetória
     @param tolerance da distância entre os pontos
     @param highestQuality que indica se será aplicado um algoritmo de distância radial
    */
    static std::vector<XnPoint3D> simplify(std::vector<XnPoint3D> points, double tolerance, bool highestQuality);

    /**
     Método responsável por aplicar o método de Douglas Peucker para simplificar a trajetória
     @param points a serem simplificados
     @param sqTolerance da distância entre dois pontos
     @return std::vector<XnPoint3D> com trajetória simplificada
    **/
    static std::vector<XnPoint3D> simplifyDouglasPeucker(std::vector<XnPoint3D> points, double sqTolerance);

    /**
     Método responsável por simplificar a trajetória usando a distância radial
     @param points da trajetória
     @param sqTolerance da distância entre dois pontos
     @return std::vector<XnPoint3D> com trajetória simplificada
    **/
    static std::vector<XnPoint3D> simplifyRadialDist(std::vector<XnPoint3D> points, double sqTolerance);

    /**
     Método responsável por aplicar um passo da simplificação de Douglas Peucker
     @param points da trajetória
     @param first ponto da trajetória
     @param last ponto da trajetória
     @param sqTolerence entre dois pontos
     @param simplified trajetória simplificada
     @return std::vector<XnPoint3D> com trajetória simplificada
    **/
    static std::vector<XnPoint3D> simplifyDPStep(std::vector<XnPoint3D> points, int first, int last, double sqTolerance, std::vector<XnPoint3D> simplified);
};

#endif /* defined(__GestureTracking__MathUtil__) */
