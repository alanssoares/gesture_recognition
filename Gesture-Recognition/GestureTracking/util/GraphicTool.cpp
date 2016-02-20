//
//  GraphicTool.h
//  GestureTracking
//
//  Created by Alan Santos on 13/02/16.
//  Copyright (c) 2015 Alan Santos. All rights reserved.
//

#include "GraphicTool.h"

GraphicTool* GraphicTool::m_Instance = NULL;

GraphicTool&
GraphicTool::getInstance()
{
    if(m_Instance == NULL)
    {
        return *(m_Instance = new GraphicTool());
    }

    return *m_Instance;
}

void
GraphicTool::InitOpenGL()
{
   //Create windows 2
   glutCreateWindow("Gesture Analyses");
   // Set the window's initial width & height
   glutInitWindowSize(450, 450);
   // Position the window's initial top-left corner
   glutInitWindowPosition(50, 50);
}

void
GraphicTool::glutDisplay()
{
   Display();
}

void
GraphicTool::glutIdle(void)
{
   // Display the frame
   glutPostRedisplay();
}

void
GraphicTool::InitOpenGLHooks()
{
   // Register display callback handler for window re-paint
   glutDisplayFunc(glutDisplay);
   glutIdleFunc(glutIdle);
}

void
GraphicTool::Display()
{
   // Set background color to black and opaque
   glClearColor(0.0, 0.0, 0.0, 1.0);
   // Clear the color buffer
   glClear(GL_COLOR_BUFFER_BIT);
   // Display the curve post draw
   DisplayPostDraw();
   // Swap the OpenGL display buffers
   glutSwapBuffers();
}

void
GraphicTool::DisplayPostDraw(){
   plot3D(Gesture::getInstance().m_pointsRecognized, 1);
   plot3D(Gesture::getInstance().m_pointsOriginal, 2);
}

void
GraphicTool::plot3D(std::vector<XnPoint3D> points, int color){
   // Define the color line
   glColor3f(g_colours[color][0],
             g_colours[color][1],
             g_colours[color][2]);
   // Init parser points to plot
   glBegin(GL_LINE_STRIP);
   for (int i = 0; i < points.size(); i++){
      glVertex3f(points[i].X, points[i].Y, points[i].Z);
   }
   // End parser points to plot
   glEnd();
   //Flush the buffer
   glFlush();
}