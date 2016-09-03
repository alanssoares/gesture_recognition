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

Test g_Test;
std::vector<type_gesture> g_Gestures;
std::vector<pcl::PointXYZ> g_PointsNormalA, g_PointsNormalB, g_PointsProcessedA, g_PointsProcessedB;
int g_IdView1(0), g_IdView2(0), g_np = 1, id_Gesture = 0;

std::vector<pcl::PointXYZ> converterToPointXYZ(std::vector<XnPoint3D> points){
  std::vector<pcl::PointXYZ> pointsConverted;
  pcl::PointXYZ newPoint;
  size_t n = points.size();
  for (int i = 0; i < n; i++){
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
  g_Gestures.clear();
}

XnPoint3D converterToXnPoint3D(pcl::PointXYZ point){
    XnPoint3D newPoint;
    newPoint.X = point.x;
    newPoint.Y = point.y;
    newPoint.Z = point.z;
    return newPoint;
}

float calcCurvature(pcl::PointXYZ a, pcl::PointXYZ b, pcl::PointXYZ c){
    return MathUtil::calcCurvature(converterToXnPoint3D(a), converterToXnPoint3D(b), converterToXnPoint3D(c));
}

void viewLabels(pcl::visualization::PCLVisualizer *viewer, int n1, int n2){
  viewer->addText("Nº Points: " + MathUtil::intToString(n1), 10, 10, "v1 text", g_IdView1);
  viewer->addText("Nº Points: " + MathUtil::intToString(n2), 10, 10, "v2 text", g_IdView2);
  viewer->addText("Nº Total: " + MathUtil::intToString(g_Gestures.size()), 10, 20, "v3 text", g_IdView1);
  viewer->addText("Name : " + g_Gestures[id_Gesture].name, 10, 30, "v4 text", g_IdView1);
  viewer->addText("Id : " + MathUtil::intToString(id_Gesture), 10, 40, "v5 text", g_IdView1);
  viewer->addText("Param: " + MathUtil::intToString(g_Test.m_Param), 10, 0, "v6 text", g_IdView1);
}

void viewShapes(pcl::visualization::PCLVisualizer *viewer){
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloudA(new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloudB(new pcl::PointCloud<pcl::PointXYZ>);
  std::ostringstream os1, os2;
  std::string idCloudA = "cloudA", idCloudB = "cloudB";
  float curvature;

  clearAllVectores();

  g_Gestures = g_Test.m_AllGestures;

  viewer->removeAllShapes(g_IdView1);
  viewer->removeAllShapes(g_IdView2);
  viewer->removePointCloud(idCloudA, g_IdView1);
  viewer->removePointCloud(idCloudB, g_IdView2);

  g_PointsNormalA = converterToPointXYZ(g_Gestures[id_Gesture].handOne.positions);
  g_PointsNormalB = converterToPointXYZ(g_Gestures[id_Gesture].handTwo.positions);
  
  //g_Test.applyDouglasPeucker(&g_Gestures);
  g_Test.applyCurvature(&g_Gestures);
  //g_Test.applyLaplacian(&g_Gestures);
  g_Test.applyBSpline(&g_Gestures);
  
  g_PointsProcessedA = converterToPointXYZ(g_Gestures[id_Gesture].handOne.positions);
  g_PointsProcessedB = converterToPointXYZ(g_Gestures[id_Gesture].handTwo.positions);

  size_t n1 = g_PointsNormalA.size(), n2 = g_PointsProcessedA.size();
  
  viewLabels(viewer, n1, n2);

  for (int i = 1; i < n1 - 1; i++){
    
    os1 << "LNA" << i;
    os2 << "LNB" << i;

    if(g_Gestures[id_Gesture].numHands == 2){
        curvature = calcCurvature(g_PointsNormalA[i - 1], g_PointsNormalA[i], g_PointsNormalA[i + 1]);
        if(curvature > g_Test.m_Param){
          cloudA->points.push_back(g_PointsNormalA[i]);
        } else {
          cloudA->points.push_back(g_PointsNormalA[i]);
        }
    }

    curvature = calcCurvature(g_PointsNormalB[i - 1], g_PointsNormalB[i], g_PointsNormalB[i + 1]);
    if(curvature > g_Test.m_Param){
      cloudA->points.push_back(g_PointsNormalB[i]);
    } else {
      cloudA->points.push_back(g_PointsNormalB[i]);
    }
  }

  os1.clear();
  os2.clear();

  for (int i = 0; i < n2; i++){
    
    os1 << "LPA" << i;
    os2 << "LPB" << i;
    
    cloudB->points.push_back(g_PointsProcessedB[i]);
    if(g_Gestures[id_Gesture].numHands == 2){
      cloudB->points.push_back(g_PointsProcessedA[i]);
    }
  }

  viewer->addPointCloud<pcl::PointXYZ> (cloudA, idCloudA, g_IdView1);
  viewer->addPointCloud<pcl::PointXYZ> (cloudB, idCloudB, g_IdView2);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, idCloudA);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, idCloudB);
}

void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event, void* viewer_void)
{
  pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);

  if(event.keyDown()){
    size_t n = g_Gestures.size() - 1;
    if(event.getKeySym() == "i"){
        g_Test.m_Param += 0.0005;
    } else if(event.getKeySym() == "u"){
        g_Test.m_Param -= 0.0005;
    } else if(event.getKeySym() == "k" && id_Gesture > 0){
        id_Gesture -= 1;
    } else if(event.getKeySym() == "l" && id_Gesture < n){
        id_Gesture += 1;
    }
    viewShapes(viewer);
  }
}

boost::shared_ptr<pcl::visualization::PCLVisualizer> viewCurvesVis()
{
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Curve Viewer"));

  g_Test.loadAll();

  viewer->initCameraParameters ();

  viewer->createViewPort(0.0, 0.0, 0.5, 1.0, g_IdView1);
  viewer->createViewPort(0.5, 0.0, 1.0, 1.0, g_IdView2);

  viewShapes(viewer.get());

  viewer->setBackgroundColor (0, 0, 0, g_IdView1);
  viewer->setBackgroundColor (0, 0, 0, g_IdView2);

  viewer->addCoordinateSystem (1.0);
  viewer->registerKeyboardCallback (keyboardEventOccurred, (void*)viewer.get ());

  return (viewer);
}

int main(int argc, char* argv[])
{
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = viewCurvesVis();
  while (!viewer->wasStopped ()){
    viewer->spinOnce (100);
  }
  return 0;
}
