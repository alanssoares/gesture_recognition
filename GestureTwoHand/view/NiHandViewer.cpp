/*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "NiHandViewer.h"
#include "Gesture.h"
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define LENGTHOF(arr)			(sizeof(arr)/sizeof(arr[0]))

//---------------------------------------------------------------------------
// Method Definitions
//---------------------------------------------------------------------------
SimpleViewer&
HandViewer::CreateInstance( xn::Context& context )
{
	assert(!sm_pInstance);
	return *(sm_pInstance = new HandViewer(context));
}

HandViewer::HandViewer(xn::Context& context)
:SimpleViewer(context),
m_HandTracker(context)
{}

XnStatus
HandViewer::Init(int argc, char **argv)
{
	XnStatus rc;
	rc = SimpleViewer::Init(argc, argv);
	if(rc != XN_STATUS_OK)
	{
		return rc;
	}

	return m_HandTracker.Init();
}

XnStatus
HandViewer::Run()
{
	XnStatus rc = m_HandTracker.Run();

	if(rc != XN_STATUS_OK)
	{
		return rc;
	}

	return SimpleViewer::Run(); // Does not return, enters OpenGL main loop instead
}

void
HandViewer::DisplayPostDraw()
{
    Gesture& gesture = Gesture::getInstance();
    std::string labelGesture  = "Gesture Recognized - " + gesture.m_NameGestureRecognized;
    std::string labelTwoHands = "Number of Hands - ";

    map<int,type_hand>::iterator it;
    for (it = gesture.m_Hands.begin(); it != gesture.m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            XnUInt32 nColor = it->second.id_hand % LENGTHOF(g_colours);
            drawCurve(it->second.positions, nColor);
        }
    }

    if(!gesture.m_NameGestureRecognized.empty()){
        if(gesture.m_TwoHandsRecognized){
            labelTwoHands += "two";
        } else {
            labelTwoHands += "one";
        }
    }

    drawCurves();
    drawText(labelGesture.data(), labelGesture.size(), WHITE, 10, 30);
    drawText(labelTwoHands.data(), labelTwoHands.size(), WHITE, 10, 10);
}

void
HandViewer::drawCurves()
{
    Gesture& gesture = Gesture::getInstance();

    std::string labelPerformed = "Performed";
    std::string labelProcessed = "Processed";
    std::string labelTemplate = "Template";

    //Configuring viewport
    glViewport(512, 0, GL_WIN_SIZE_MAIN_X, GL_WIN_SIZE_MAIN_Y);
    //Draw the curves

    if(gesture.m_TwoHandsRecognized){
        drawCurve(gesture.m_GesturePerformedA, BLUE);
        drawCurve(gesture.m_GesturePerformedProcessedA, GREEN);
        drawCurve(gesture.m_GestureTemplateA, YELLOW);
        drawCurve(gesture.m_GesturePerformedB, BLUE);
        drawCurve(gesture.m_GesturePerformedProcessedB, GREEN);
        drawCurve(gesture.m_GestureTemplateB, YELLOW);
    } else {
        drawCurve(gesture.m_GesturePerformedA, BLUE);
        drawCurve(gesture.m_GesturePerformedProcessedA, GREEN);
        drawCurve(gesture.m_GestureTemplateA, YELLOW);
    }

    //Draw the the legend labels of the curves
    drawText(labelPerformed.c_str(), labelPerformed.size(), BLUE, 10, GL_WIN_SIZE_Y + 60);
    drawText(labelProcessed.c_str(), labelProcessed.size(), GREEN, 10, GL_WIN_SIZE_Y + 40);
    drawText(labelTemplate.c_str(), labelTemplate.size(), YELLOW, 10, GL_WIN_SIZE_Y + 20);
}

void
HandViewer::drawCurve(vector<XnPoint3D> curve, XnUInt32 nColor)
{
    size_t len = curve.size();
    XnFloat coordinates[len * 3]; // Size * (X, Y, Z)
    vector<XnPoint3D> trajectory;
    int numPoints = 0;
    
    trajectory = convertAndScale(curve);
    
    for (int i = 0; i < len; i+=2) {
        coordinates[numPoints * 3] = trajectory[i].X;
        coordinates[numPoints * 3 + 1] = trajectory[i].Y;
        coordinates[numPoints * 3 + 2] = 0;
        numPoints++;
    }

    glColor4f(g_colours[nColor][0],
              g_colours[nColor][1],
              g_colours[nColor][2],
              1.0f);

    glPointSize(2);
    glVertexPointer(3, GL_FLOAT, 0, coordinates);
    glDrawArrays(GL_LINE_STRIP, 0, numPoints);
    // Current point as a larger dot
    glPointSize(8);
    glDrawArrays(GL_POINTS, 0, 1);
    glFlush();
}

void
HandViewer::drawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
    GLfloat verts[8] = {	topLeftX, topLeftY,
        topLeftX, bottomRightY,
        bottomRightX, bottomRightY,
        bottomRightX, topLeftY
    };
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    
    //TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
    glFlush();
}

void
HandViewer::drawCircle(XnPoint3D point, float radius)
{
    m_depth.ConvertRealWorldToProjective(1, &point, &point);
    ScalePoint(point, GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
    
    glBegin(GL_TRIANGLE_FAN);
    
    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
        glVertex2f(point.X + cos(degInRad) * radius, point.Y + sin(degInRad) * radius);
    }
    
    glEnd();
}

void
HandViewer::drawText(const char* text, int length, XnUInt32 nColor, int x, int y)
{
    glColor4f(g_colours[nColor][0],
              g_colours[nColor][1],
              g_colours[nColor][2],
              1.0f);
    glMatrixMode(GL_PROJECTION);
    double * matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    for(int i = 0; i < length; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int) text[i]);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

XnStatus
HandViewer::InitOpenGL( int argc, char **argv )
{
	XnStatus rc = SimpleViewer::InitOpenGL(argc, argv); 

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	return rc;
}

vector<XnPoint3D>
HandViewer::convertAndScale(vector<XnPoint3D> points){
    vector<XnPoint3D> newPoints;
    for(int i = 0; i < points.size(); i++){
        XnPoint3D point = points[i];
        m_depth.ConvertRealWorldToProjective(1, &point, &point);
        ScalePoint(point, GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
        newPoints.push_back(point);
    }
    return newPoints;
}