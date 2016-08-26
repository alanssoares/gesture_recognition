//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <iostream>

#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

#include "../Commons/commons.hpp"
#include "test/Test.h"

// ---- Global variables ----
std::vector<pcl::PointXYZ> g_PointsNormalA, g_PointsNormalB, g_PointsProcessedA, g_PointsProcessedB;
Test g_Test;
int g_IdView1(0), g_IdView2(0), g_np = 1, g_Gesture = 0;

std::string intToString(int n){
    std::ostringstream converter;
    converter << n;
    return converter.str();
}

std::vector<pcl::PointXYZ> converterToPointXYZ(std::vector<XnPoint3D> points){
  std::vector<pcl::PointXYZ> pointsConverted;
  pcl::PointXYZ newPoint;
  size_t n = points.size();
  for (int i = n; i >= 0; i--){
    newPoint.x = points[i].X;
    newPoint.y = points[i].Y;
    newPoint.z = points[i].Z;
    pointsConverted.push_back(newPoint);
  }
  return pointsConverted;
}

void clearAllVectores(){
  g_PointsNormalA.clear();
  g_PointsNormalB.clear();
  g_PointsProcessedA.clear();
  g_PointsProcessedB.clear();
}

void executeAll(){
    std::string nameFile = "../result_experiment";
    for (int i = 1; i < 8; i++){
        g_Test.experiment(i, nameFile + intToString(i) + ".txt");
    }
}

XnPoint3D converterToXnPoint3D(pcl::PointXYZ point){
    XnPoint3D newPoint;
    newPoint.X = point.x;
    newPoint.Y = point.y;
    newPoint.Z = point.z;
    return newPoint;
}

void reshape(pcl::visualization::PCLVisualizer *viewer){
  std::ostringstream os1, os2;
  XnPoint3D a, b, c;
  float curvature;

  clearAllVectores();

  viewer->removeAllShapes(g_IdView1);
  viewer->removeAllShapes(g_IdView2);

  g_Test.initAllSamples();

  g_PointsNormalA = converterToPointXYZ(g_Test.m_GesturesTemplate[g_Gesture].handOne.positions);
  g_PointsNormalB = converterToPointXYZ(g_Test.m_GesturesTemplate[g_Gesture].handTwo.positions);
  
  g_Test.applyProcess(g_np);
  
  g_PointsProcessedA = converterToPointXYZ(g_Test.m_GesturesTemplate[g_Gesture].handOne.positions);
  g_PointsProcessedB = converterToPointXYZ(g_Test.m_GesturesTemplate[g_Gesture].handTwo.positions);

  size_t n1 = g_PointsNormalA.size();
  for (int i = 1; i < n1 - 1; i++){
    os1 << "lineNormalA" << i;
    os2 << "lineNormalB" << i;
    
    a = converterToXnPoint3D(g_PointsNormalA[i - 1]);
    b = converterToXnPoint3D(g_PointsNormalA[i]);
    c = converterToXnPoint3D(g_PointsNormalA[i + 1]);
    curvature = MathUtil::calcCurvature(a, b, c);
    if(curvature > g_Test.m_Param){
      viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsNormalA[i], g_PointsNormalA[i + 1], 1.0, 0.0, 0.0, os1.str(), g_IdView1);
    } else {
      viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsNormalA[i], g_PointsNormalA[i + 1], 0.0, 0.0, 1.0, os1.str(), g_IdView1);
    }

    a = converterToXnPoint3D(g_PointsNormalB[i - 1]);
    b = converterToXnPoint3D(g_PointsNormalB[i]);
    c = converterToXnPoint3D(g_PointsNormalB[i + 1]);
    curvature = MathUtil::calcCurvature(a, b, c);
    if(curvature > g_Test.m_Param){
      viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsNormalB[i], g_PointsNormalB[i + 1], 1.0, 0.0, 0.0, os2.str(), g_IdView1);
    } else {
      viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsNormalB[i], g_PointsNormalB[i + 1], 0.0, 0.0, 1.0, os2.str(), g_IdView1);
    }
  }

  os1.clear();
  os2.clear();

  size_t n2 = g_PointsProcessedA.size();
  for (int i = 1; i < n2 - 1; i++){
    os1 << "lineProcessedA" << i;
    os2 << "lineProcessedB" << i;
    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsProcessedA[i], g_PointsProcessedA[i + 1], 0.0, 0.0, 1.0, os1.str(), g_IdView2);
    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (g_PointsProcessedB[i], g_PointsProcessedB[i + 1], 0.0, 0.0, 1.0, os2.str(), g_IdView2);
  }

  viewer->addText("Nº Points: " + intToString(n1), 10, 10, "v1 text", g_IdView1);
  viewer->addText("Nº Points: " + intToString(n2), 10, 10, "v2 text", g_IdView2);
}

void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event, void* viewer_void)
{
  pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);

  if(event.keyDown()){
    if(event.getKeySym() == "p"){
        cout<<"The curves were reploted: "<<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "i"){
        g_Test.m_Param += 0.001;
        cout<<"New value: "<<g_Test.m_Param<<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "u"){
        g_Test.m_Param -= 0.001;
        cout<<"New value: "<< g_Test.m_Param<<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "k" && g_Gesture > 0){
        g_Gesture -= 1;
        cout<<"Gesture : "<<g_Gesture <<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "l" && g_Gesture < g_Test.m_GesturesTemplate.size() - 1){
        g_Gesture += 1;
        cout<<"Gesture: "<<g_Gesture<<endl;
        reshape(viewer);
    }
  }
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> viewCurvesVis()
{
  std::ostringstream os1, os2;
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Curve Viewer"));
  
  viewer->initCameraParameters ();

  viewer->createViewPort(0.0, 0.0, 0.5, 1.0, g_IdView1);
  viewer->createViewPort(0.5, 0.0, 1.0, 1.0, g_IdView2);

  reshape(viewer.get());

  viewer->setBackgroundColor (0, 0, 0, g_IdView1);
  viewer->setBackgroundColor (0, 0, 0, g_IdView2);

  viewer->addCoordinateSystem (1.0);
  viewer->registerKeyboardCallback (keyboardEventOccurred, (void*)viewer.get ());

  return (viewer);
}

int main(int argc, char* argv[])
{
    if (pcl::console::find_argument (argc, argv, "-p") >= 0){
      g_np = atoi(argv[2]);
      std::cout<<" Visualization with process "<< g_np <<endl;
    } else {
      std::cout<<" The param -p is required. Try again!" <<endl;
      return 0;
    }

    // ---- Init Test -----
    g_Test.init();

    // ---- Initialize visualization ------
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = viewCurvesVis();

    // -----Main loop-----
    while (!viewer->wasStopped ())
    {
        viewer->spinOnce (100);
        //boost::this_thread::sleep (boost::posix_time::microseconds (100000));
    }

    return 0;
}
