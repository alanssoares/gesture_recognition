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
std::vector<pcl::PointXYZRGB> g_PointsNormalA, g_PointsNormalB, g_PointsProcessedA, g_PointsProcessedB;
int g_IdView1(0), g_IdView2(0), g_np = 1, id_Gesture = 0, g_Methods = 1;
std::string g_IdCloudA = "cloudA", g_IdCloudB = "cloudB";

std::vector<pcl::PointXYZRGB> converterToPointXYZ(std::vector<XnPoint3D> points){
  std::vector<pcl::PointXYZRGB> pointsConverted;
  pcl::PointXYZRGB newPoint;
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

void removeAll(pcl::visualization::PCLVisualizer *viewer){
  viewer->removeAllShapes(g_IdView1);
  viewer->removeAllShapes(g_IdView2);
  viewer->removePointCloud(g_IdCloudA, g_IdView1);
  viewer->removePointCloud(g_IdCloudB, g_IdView2);
}

XnPoint3D converterToXnPoint3D(pcl::PointXYZRGB point){
    XnPoint3D newPoint;
    newPoint.X = point.x;
    newPoint.Y = point.y;
    newPoint.Z = point.z;
    return newPoint;
}

float calcCurvature(pcl::PointXYZRGB a, pcl::PointXYZRGB b, pcl::PointXYZRGB c){
    return MathUtil::calcCurvature(converterToXnPoint3D(a), converterToXnPoint3D(b), converterToXnPoint3D(c));
}

void improveCurrentGesture(){

  g_PointsNormalA = converterToPointXYZ(g_Gestures[id_Gesture].handOne.positions);
  g_PointsNormalB = converterToPointXYZ(g_Gestures[id_Gesture].handTwo.positions);

  if(g_Methods == 1){
    g_PointsProcessedA = converterToPointXYZ(MathUtil::smoothMeanNeighboring(MathUtil::simplify(g_Gestures[id_Gesture].handOne.positions, g_Test.m_DougThreshold, false)));
    g_PointsProcessedB = converterToPointXYZ(MathUtil::smoothMeanNeighboring(MathUtil::simplify(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_DougThreshold, false)));    
  } else if(g_Methods == 2) {
    g_PointsProcessedA = converterToPointXYZ(BSpline::curvePoints(MathUtil::simplify(g_Gestures[id_Gesture].handOne.positions, g_Test.m_DougThreshold, false), NUM_STEP_BSPLINE));
    g_PointsProcessedB = converterToPointXYZ(BSpline::curvePoints(MathUtil::simplify(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_DougThreshold, false), NUM_STEP_BSPLINE));
  } else if(g_Methods == 3) {
    g_PointsProcessedA = converterToPointXYZ(MathUtil::smoothMeanNeighboring(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handOne.positions, g_Test.m_CurvThreshold)));
    g_PointsProcessedB = converterToPointXYZ(MathUtil::smoothMeanNeighboring(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_CurvThreshold)));
  } else if(g_Methods == 4) {
    g_PointsProcessedA = converterToPointXYZ(BSpline::curvePoints(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handOne.positions, g_Test.m_CurvThreshold), NUM_STEP_BSPLINE));
    g_PointsProcessedB = converterToPointXYZ(BSpline::curvePoints(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_CurvThreshold), NUM_STEP_BSPLINE));
  } else if(g_Methods == 5) {
    g_PointsProcessedA = converterToPointXYZ(MathUtil::simplify(g_Gestures[id_Gesture].handOne.positions, g_Test.m_DougThreshold, false));
    g_PointsProcessedB = converterToPointXYZ(MathUtil::simplify(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_DougThreshold, false));
  } else if(g_Methods == 6) {
    g_PointsProcessedA = converterToPointXYZ(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handOne.positions, g_Test.m_CurvThreshold));
    g_PointsProcessedB = converterToPointXYZ(MathUtil::reduceByCurvature(g_Gestures[id_Gesture].handTwo.positions, g_Test.m_CurvThreshold));
  } else if(g_Methods == 7) {
    g_PointsProcessedA = converterToPointXYZ(BSpline::curvePoints(g_Gestures[id_Gesture].handOne.positions, NUM_STEP_BSPLINE));
    g_PointsProcessedB = converterToPointXYZ(BSpline::curvePoints(g_Gestures[id_Gesture].handTwo.positions, NUM_STEP_BSPLINE));
  } else if(g_Methods == 8) {
    g_PointsProcessedA = converterToPointXYZ(MathUtil::smoothMeanNeighboring(g_Gestures[id_Gesture].handOne.positions));
    g_PointsProcessedB = converterToPointXYZ(MathUtil::smoothMeanNeighboring(g_Gestures[id_Gesture].handTwo.positions));
  }
  
 }

void viewLabels(pcl::visualization::PCLVisualizer *viewer){
  viewer->addText("Nº Points: " + MathUtil::intToString(g_PointsNormalA.size()), 10, 10, "v1 text", g_IdView1);
  viewer->addText("Nº Points: " + MathUtil::intToString(g_PointsProcessedA.size()), 10, 10, "v2 text", g_IdView2);
  viewer->addText("Nº Total: " + MathUtil::intToString(g_Gestures.size()), 10, 20, "v3 text", g_IdView1);
  viewer->addText("Name : " + g_Gestures[id_Gesture].name, 10, 30, "v4 text", g_IdView1);
  viewer->addText("Id : " + MathUtil::intToString(id_Gesture), 10, 40, "v5 text", g_IdView1);
}

void viewNormalGesture(pcl::visualization::PCLVisualizer *viewer){
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudA(new pcl::PointCloud<pcl::PointXYZRGB>);
  size_t n = g_PointsNormalA.size();
  std::ostringstream os1, os2;
  for (int i = 0; i < n; i++){
    os1 << "LNA" << i;
    os2 << "LNB" << i;
    cloudA->points.push_back(g_PointsNormalB[i]);
    if(g_Gestures[id_Gesture].numHands == 2){
      cloudA->points.push_back(g_PointsNormalA[i]);
    }
  }
  viewer->addPointCloud<pcl::PointXYZRGB> (cloudA, g_IdCloudA, g_IdView1);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, g_IdCloudA);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_COLOR, 0.0f, 0.0f, 1.0f, g_IdCloudA);
}

void viewProcessedGesture(pcl::visualization::PCLVisualizer *viewer){
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloudB(new pcl::PointCloud<pcl::PointXYZRGB>);
  size_t n = g_PointsProcessedA.size();
  std::ostringstream os1, os2;
  for (int i = 0; i < n; i++){
    os1 << "LPA" << i;
    os2 << "LPB" << i;
    cloudB->points.push_back(g_PointsProcessedB[i]);
    if(g_Gestures[id_Gesture].numHands == 2){
      cloudB->points.push_back(g_PointsProcessedA[i]);
    }
  }
  viewer->addPointCloud<pcl::PointXYZRGB> (cloudB, g_IdCloudB, g_IdView2);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, g_IdCloudB);
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_COLOR, 1.0f, 0.0f, 0.0f, g_IdCloudB);
}

void viewShapes(pcl::visualization::PCLVisualizer *viewer){
  //Clear all global declared vectores
  clearAllVectores();
  //Remove all shapes, lines, etc from the view
  removeAll(viewer);
  //Reload all gestures
  g_Gestures = g_Test.m_AllGestures;
  //Transform and process the gestures of the viewport 1 and 2
  improveCurrentGesture();
  //Plot all labels in the screen
  viewLabels(viewer);
  //Plot the normal gesture
  viewNormalGesture(viewer);
  //Plot the processed gesture
  viewProcessedGesture(viewer);
}

void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event, void* viewer_void)
{
  pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);

  if(event.keyDown()){
    size_t n = g_Gestures.size() - 1;
    if(event.getKeySym() == "i"){
        g_Test.m_CurvThreshold += 0.0001;
        g_Test.m_DougThreshold += 0.0001;
    } else if(event.getKeySym() == "u"){
        if(g_Test.m_CurvThreshold > 0.0001) g_Test.m_CurvThreshold -= 0.0001;
        if(g_Test.m_DougThreshold > 0.0001) g_Test.m_DougThreshold -= 0.0001;
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

void generateMedians(){
  g_Test.init();
  g_Test.generateMedianGesture(g_Test.m_AllGestures);
  g_Test.saveMedianGestures();
}

int main(int argc, char* argv[])
{
  
  if(pcl::console::find_argument (argc, argv, "-t") >= 0){
    g_Test.executeAll();
    return 0;
  }

  if(pcl::console::find_argument (argc, argv, "-g") >= 0){
    generateMedians();
    return 0;
  }

  if(pcl::console::find_argument (argc, argv, "-m") >= 0){
    g_Methods = atoi(argv[2]);
  }

  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = viewCurvesVis();
  while (!viewer->wasStopped ()){
    viewer->spinOnce (100);
  }

  return 0;
}
