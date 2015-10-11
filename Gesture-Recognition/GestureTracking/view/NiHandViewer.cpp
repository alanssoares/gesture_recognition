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
    
    map<int,type_hand>::iterator it;
    for (it = gesture.m_Hands.begin(); it != gesture.m_Hands.end(); ++it){
        if(!it->second.positions.empty()){
            drawHand(it->second.id_hand, it->second.positions);
            //drawCircle(it->second.positions.back(), MAX_RADIUS);
        }
    }
}
/*
 Método drawHand recebe como parametros o id da mão e a trajetória e converte
 as coordenadas do mundo real em projetivas para renderiza-la no monitor.
 */
void
HandViewer::drawHand(int idHand, vector<XnPoint3D> positions)
{
    size_t len = positions.size();
    XnFloat coordinates[len * 3]; // Size * (X, Y, Z)
    vector<XnPoint3D> trajectory;
    int numPoints = 0;
    
    for(int i = 0; i < len; i++){
        
        XnPoint3D point = positions[i];
        m_depth.ConvertRealWorldToProjective(1, &point, &point);
        ScalePoint(point);
        trajectory.push_back(point);
    }
    
    for (int i = 0; i < len; i+=2) {
        coordinates[numPoints * 3] = trajectory[i].X;
        coordinates[numPoints * 3 + 1] = trajectory[i].Y;
        coordinates[numPoints * 3 + 2] = 0;
        
        numPoints++;
    }
    
    draw(idHand, numPoints, coordinates);
}

/*
 Método draw renderiza a trajetória da mão
 */
void
HandViewer::draw(int idHand, int numPoints, XnFloat coordinates[]){
    
    //printf("id %d n %d\n", idHand, numPoints);
    
    static const float colours[][3] =
    {
        { 0.5f, 0.5f, 0.5f},//Gray
        { 0.0f, 1.0f, 0.0f},//Green
        { 0.0f, 0.5f, 1.0f},//Blue
        { 1.0f, 1.0f, 0.0f},//Yellow
        { 1.0f, 0.5f, 0.0f},//Organge
        { 1.0f, 0.0f, 1.0f}//Pink
    };
    
    // Draw the hand trail history
    XnUInt32 nColor = idHand % LENGTHOF(colours);
    glColor4f(colours[nColor][0],
              colours[nColor][1],
              colours[nColor][2],
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
    ScalePoint(point);
    
    glBegin(GL_TRIANGLE_FAN);
    
    for (int i = 0; i < 360; i++)
    {
        float degInRad = i * DEG2RAD;
        glColor4f(0.0f, 0.5f, 1.0f, 1.0f);
        glVertex2f(point.X + cos(degInRad) * radius, point.Y + sin(degInRad) * radius);
    }
    
    glEnd();
}

XnStatus
HandViewer::InitOpenGL( int argc, char **argv )
{
	XnStatus rc = SimpleViewer::InitOpenGL(argc, argv); 

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	return rc;
}