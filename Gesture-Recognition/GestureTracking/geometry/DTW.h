//
//  DTW.h
//  GestureTracking
//
//  Created by Alan Santos on 26/09/15.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__DTW__
#define __GestureTracking__DTW__

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include <XnCppWrapper.h>

using namespace std;

/*
 *	Author: Kan Ouivirach
 *	email: zkan at cs dot ait dot ac dot th
 * link - https://github.com/zkan/DTW
 *
 */
/**
 * (c) Daniel Lemire, 2008
 * (c) Earlence Fernandes, Vrije Universiteit Amsterdam 2011
 * https://github.com/lemire/lbimproved/blob/master/vector_dtw/vector_test.cpp
 * This C++ library implements dynamic time warping (DTW).
 * This library includes the dynamic programming solution for vectored input signals represented
 * by the class Point. Currently, it has 3 dimensions - x, y, z. More can easily be added to this class.
 * No change would be required to the DTW class. Only keep in mind that the distance code has to be updated
 * to accomodate more dimensions.
 *
 * Time series are represented using STL vectors.
 */
class DTW {
    
private:
    std::vector<vector<double> > mGamma;
    unsigned long mN, mConstraint;
    
public:
    //some big number
    enum {  INF = 100000000 };
    
    ~DTW() {};
    DTW(){};
    
    /**
     * n is the length of the time series
     *
     * constraint is the maximum warping distance.
     * Typically: constraint = n/10.
     * If you set constraint = n, things will be slower.
     *
     */
    DTW(unsigned long n, uint constraint) : mGamma(n, std::vector<double>(n, INF)), mN(n), mConstraint(constraint) { };
    
    /**
     * This currently uses euclidean distance.
     * You can change it to whatever is needed for your application
     */
    double fastDynamic(vector<XnPoint3D> &v, vector<XnPoint3D> &w);
    
    /**
     Calc the Euclidian Distance of p to q
     */
    double euclidDistance(const XnPoint3D &p, const XnPoint3D &q);
    
    /**
     computes the l1 distance with another point
     */
    double l1Distance(const XnPoint3D &p, const XnPoint3D &q);
    
    /*
     Calc the minimum value between two values
     */
    static inline double min (double x, double y ) { return x < y ? x : y; }
    
    static inline double max (double x, double y ) { return x > y ? x : y; }
};


/**
 This class implements the Dynamic Time Warping algorithm
 given two sequences
  <pre>
    X = x1, x2,..., xi,..., xn
    Y = y1, y2,..., yj,..., ym
   </pre>
 
  @author   Cheol-Woo Jung (cjung@gatech.edu)
  @version  1.0
  @link http://trac.research.cc.gatech.edu/GART/browser/GART/weka/edu/gatech/gart/ml/weka/DTW.java?rev=9
 */
class DTW2 {
private:
    int mN, mM, mK;
    double mWarpingDistance;
    vector<XnPoint3D> mSequence1;
    vector<XnPoint3D> mSequence2;
public:
    
    DTW2();
    DTW2(std::vector<XnPoint3D> &v, std::vector<XnPoint3D> &w);
    ~DTW2(){};
    
    void compute();
    
    /**
     Specific the sequences that are matching
     @param v sequence sample
     @param w sequence template
     **/
    void setSequences(std::vector<XnPoint3D> &v, std::vector<XnPoint3D> &w);
    /**
     Finds the index of the minimum element from the given array
     @param array the array containing numeric values
     @return the min value among elements
     **/
    int getIndexOfMinimum(std::vector<double> array);
    /**
     Return the best distance path
     @return warpingDistance
     **/
    double getDistance() { return mWarpingDistance; };
    /**
     Calc the Euclidian Distance of p to q
     */
    double euclidDistance(const XnPoint3D &p, const XnPoint3D &q);
    
    /**
     computes the l1 distance with another point
     */
    double l1Distance(const XnPoint3D &p, const XnPoint3D &q);
};
#endif /* defined(__GestureTracking__DTW__) */
