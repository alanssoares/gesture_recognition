//
//  MathUtil.cpp
//  GestureTracking
//
//  Created by Alan Santos on 22/10/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "MathUtil.h"

/**
 Calcula a amplitude/módulo escalar de um vetor.
 */
double
MathUtil::length(XnPoint3D point){
    return sqrt(pow(point.X, 2) + pow(point.Y, 2) + pow(point.Z, 2));
}

/*
 TODO: realiza a subtração entre dois vetores a e b.
 */
XnPoint3D
MathUtil::subtract(XnPoint3D a, XnPoint3D b){
    a.X = a.X - b.X;
    a.Y = a.Y - b.Y;
    a.Z = a.Z - b.Z;
    return a;
}

/**
 Normaliza um vetor dividindo cada componente pelo
 módulo do vetor.
 */
XnPoint3D
MathUtil::normalize(XnPoint3D point) {
    float len = length(point);
    point.X = point.X / len;
    point.Y = point.Y / len;
    point.Z = point.Z / len;
    return point;
}

/**
 Calcula o ângulo formado entre dois vetores a e b, com
 uma reta saindo da origem e passando pelos pontos
 */
double
MathUtil::getAngleBetween2Points(XnPoint3D a, XnPoint3D b){
    double mU, mV, mUV, uv, angle;
    
    //Produto vetorial
    uv = a.X * b.X + a.Y * b.Y;
    
    //Módulo dos vetores
    mU = sqrt(pow(a.X, 2) + pow(a.Y, 2));
    mV = sqrt(pow(b.X, 2) + pow(b.Y, 2));
    
    //Produto vetorial dos módulos
    mUV = mU * mV;
    
    angle = cos(uv/mUV);
    
    angle = acos(angle);
    
    return angle;
}

/**
 Método que retorna o maior valor dentro de um vetor
 */
double
MathUtil::getMaxValue(vector<double> values)
{
    double max = 0.0;
    for (double value : values)
    {
        if (max < value) {
            max = value;
        }
    }
    
    return max;
}

/*
 Somatório das diferenças de distância euclidiana entre as n posições anteriores
 */
double
MathUtil::getSumDiff(vector<XnPoint3D> positions)
{
    float total = 0.0;
    size_t n = positions.size();
    size_t len = n - MAX_HAND_CONTROL_POINTS;
    
    XnPoint3D ant = positions[n-1];
    XnPoint3D result;
    for (size_t i = len; i < n; i++){
        result = MathUtil::subtract(ant, positions[i]);
        total += MathUtil::length(result);
        ant = positions[i];
    }
    
    return total;
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
MathUtil::calcCentroid(vector<XnPoint3D> positions)
{
    XnPoint3D centroid;
    centroid.X = centroid.Y = centroid.Z = 0;
    for(int i = 0; i < positions.size(); i++) {
        centroid.X += positions[i].X;
        centroid.Y += positions[i].Y;
        centroid.Z += positions[i].Z;
    }
    //Sum 1 to prevent division by zero
    centroid.X = (centroid.X + 1)/(positions.size() + 1) - 1;
    centroid.Y = (centroid.Y + 1)/(positions.size() + 1) - 1;
    centroid.Z = (centroid.Z + 1)/(positions.size() + 1) - 1;
    
    return centroid;
}

/*
 Método responsável por normalizar a trajetória para reduzir
 o impacto de gestos realizados por pessoas com aspectos físicos
 diferentes. A normalização é realizada através de uma translação
 da trajetória para a origem (0,0,0) usando a centróide da trajetória.
 */
vector<XnPoint3D>
MathUtil::translateToOrigin(vector<XnPoint3D> positions){
    
    XnPoint3D centroid = calcCentroid(positions);
    
    for(int i = 0; i < positions.size(); i++) {
        positions[i].X -= centroid.X;
        positions[i].Y -= centroid.Y;
        positions[i].Z -= centroid.Z;
    }
    
    return positions;
}