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