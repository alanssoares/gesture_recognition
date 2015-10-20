//
//  DTW.cpp
//  GestureTracking
//
//  Created by Alan Santos on 26/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "DTW.h"

double
DTW::euclidDistance(const XnPoint3D &p, const XnPoint3D &q){
    return sqrt((p.X - q.X) * (p.X - q.X) + (p.Y - q.Y) * (p.Y - q.Y) + (p.Z - q.Z) * (p.Z - q.Z));
}

double
DTW::l1Distance(const XnPoint3D &p, const XnPoint3D &q){
    return fabs(p.X - q.X) + fabs(p.Y - q.Y) + fabs(p.Z - q.Z);
}

double
DTW::fastDynamic(vector<XnPoint3D> &v, vector<XnPoint3D> &w){
    
    assert(static_cast<int>(v.size()) == mN);
    assert(static_cast<int>(w.size()) == mN);
    assert(static_cast<int>(mGamma.size()) == mN);
    double Best(INF);
    for (int i = 0; i < mN; ++i)
    {
        assert(static_cast<int>(mGamma[i].size()) == mN);
        for(int j = max(0, i - mConstraint); j < min(mN, i + mConstraint + 1); ++j)
        {
            Best = INF;
            if(i > 0)
                Best = mGamma[i - 1][j];
            if(j > 0)
                Best = min(Best, mGamma[i][j - 1]);
            if((i > 0) && (j > 0))
                Best = min(Best, mGamma[i - 1][j - 1]);
            if((i == 0) && (j == 0))
                mGamma[i][j] = euclidDistance(v[i], w[j]);
            else
                mGamma[i][j] = Best + euclidDistance(v[i], w[j]);
        }
    }
    
    return mGamma[mN-1][mN-1];
    
}

DTW2::DTW2(){
    mWarpingDistance = 0.0;
    mK = 1;
}

DTW2::DTW2(vector<XnPoint3D> &v, vector<XnPoint3D> &w){
    mSequence1 = v;
    mSequence2 = w;
    mWarpingDistance = 0.0;
    mK = 1;
}

double
DTW2::euclidDistance(const XnPoint3D &p, const XnPoint3D &q){
    return sqrt((p.X - q.X) * (p.X - q.X) + (p.Y - q.Y) * (p.Y - q.Y) + (p.Z - q.Z) * (p.Z - q.Z));
}

double
DTW2::l1Distance(const XnPoint3D &p, const XnPoint3D &q){
    return fabs(p.X - q.X) + fabs(p.Y - q.Y) + fabs(p.Z - q.Z);
}

void
DTW2::compute(){
    double accumulatedDistance = 0.0;
    double d[mN][mM];// local distances
    double D[mN][mM];// global distances
    int warpingPath [mN + mM][2];
    
    for (int i = 0; i < mN; i++) {
        for (int j = 0; j < mM; j++) {
            d[i][j] = euclidDistance(mSequence1[i], mSequence2[j]);
        }
    }
    
    D[0][0] = d[0][0];
    for (int i = 1; i < mN; i++) {
        D[i][0] = d[i][0] + D[i - 1][0];
    }
    
    for (int j = 1; j < mM; j++) {
        D[0][j] = d[0][j] + D[0][j - 1];
    }
    
    for (int i = 1; i < mN; i++) {
        for (int j = 1; j < mM; j++) {
            accumulatedDistance = min(min(D[i-1][j], D[i-1][j-1]), D[i][j-1]);
            accumulatedDistance += d[i][j];
            D[i][j] = accumulatedDistance;
        }
    }
    
    accumulatedDistance = D[mN - 1][mM - 1];
    int i = mN - 1;
    int j = mM - 1;
    int minIndex = 1;
    
    // max(n, m) <= K < n + m
    warpingPath[mK - 1][0] = i;
    warpingPath[mK - 1][1] = j;
    
    while ((i + j) != 0) {
        if (i == 0) {
            j -= 1;
        } else if (j == 0) {
            i -= 1;
        } else {
            // i != 0 && j != 0
            std::vector<double> array = { D[i - 1][j], D[i][j - 1], D[i - 1][j - 1] };
            minIndex = getIndexOfMinimum(array);
            if (minIndex == 0) {
                i -= 1;
            } else if (minIndex == 1) {
                j -= 1;
            } else if (minIndex == 2) {
                i -= 1;
                j -= 1;
            }
        } // end else
        mK++;
        warpingPath[mK - 1][0] = i;
        warpingPath[mK - 1][1] = j;
    } // end while
    
    mWarpingDistance = accumulatedDistance / mK;
}

int
DTW2::getIndexOfMinimum(std::vector<double> array) {
    int index = 0;
    double val = array[0];
    for (int i = 1; i < array.size(); i++) {
        if (array[i] < val) {
            val = array[i];
            index = i;
        }
    }
    return index;
}

void
DTW2::setSequences(std::vector<XnPoint3D> &v, std::vector<XnPoint3D> &w){
    mSequence1 = v;
    mSequence2 = w;
}