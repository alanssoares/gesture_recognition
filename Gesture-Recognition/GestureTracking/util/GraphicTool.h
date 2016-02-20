//
//  GraphicTool.h
//  GestureTracking
//
//  Created by Alan Santos on 13/02/16.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#ifndef __GestureTracking__GraphicTool__
#define __GestureTracking__GraphicTool__

#include <stdio.h>
#include <XnCppWrapper.h>
#include <vector>
#include <math.h>
#include "../gesture/Gesture.h"
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

static const float g_colours[6][3] = {
    { 0.5f, 0.5f, 0.5f},//Gray 0
    { 0.0f, 1.0f, 0.0f},//Green 1
    { 0.0f, 0.5f, 1.0f},//Blue 2
    { 1.0f, 1.0f, 0.0f},//Yellow 3
    { 1.0f, 0.5f, 0.0f},//Organge 4
    { 1.0f, 0.0f, 1.0f}//Pink 5
};

class GraphicTool {

public:
	
	GraphicTool(){};
	~GraphicTool(){};

	static void InitOpenGL();
	static void glutDisplay();
	static void glutIdle();
	static void InitOpenGLHooks();
	static void Display();
	static void DisplayPostDraw();
	static void plot3D(std::vector<XnPoint3D> points, int color);

	static GraphicTool& getInstance();
    static GraphicTool*	m_Instance;
};

#endif /* defined(__GestureTracking__GraphicTool__) */