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
unsigned int text_id = 0;
int g_IdView1(0), g_IdView2(0);
Test g_Test;
int g_np = 1;
std::vector<std::string> idLines;

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

void removeAllLines(pcl::visualization::PCLVisualizer *viewer){
  for (int i = 0; i < idLines.size(); i++){
    cout<<" id "<<idLines[i].c_str()<<endl;
    viewer->removeShape(idLines[i].c_str());
  }
}

void executeAll(){
    std::string nameFile = "../result_experiment";
    for (int i = 1; i < 8; i++){
        g_Test.experiment(i, nameFile + intToString(i) + ".txt");
    }
}

void reshape(pcl::visualization::PCLVisualizer *viewer){
  std::ostringstream os1, os2;

  removeAllLines(viewer);

  viewer->resetCamera();

  g_Test.initAllSamples();

  std::vector<pcl::PointXYZ> pointsNormalA = converterToPointXYZ(g_Test.m_GesturesTemplate[0].handOne.positions);
  std::vector<pcl::PointXYZ> pointsNormalB = converterToPointXYZ(g_Test.m_GesturesTemplate[0].handTwo.positions);
  
  g_Test.applyProcess(g_np);
  
  std::vector<pcl::PointXYZ> pointsProcessedA = converterToPointXYZ(g_Test.m_GesturesTemplate[0].handOne.positions);
  std::vector<pcl::PointXYZ> pointsProcessedB = converterToPointXYZ(g_Test.m_GesturesTemplate[0].handTwo.positions);

  for (int i = 0; i < pointsNormalA.size() - 1; i++){
    os1 << "lineNormalA" << i;
    os2 << "lineNormalB" << i;

    idLines.push_back(os1.str());
    idLines.push_back(os2.str());

    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (pointsNormalA[i], pointsNormalA[i + 1], 0.0, 1.0, 0.0, os1.str(), g_IdView1);
    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (pointsNormalB[i], pointsNormalB[i + 1], 1.0, 1.0, 0.0, os2.str(), g_IdView1);
  }

  os1.clear();
  os2.clear();

  for (int i = 0; i < pointsProcessedA.size() - 1; i++){
    os1 << "lineProcessedA" << i;
    os2 << "lineProcessedB" << i;
    
    idLines.push_back(os1.str());
    idLines.push_back(os2.str());

    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (pointsProcessedA[i], pointsProcessedA[i + 1], 0.0, 1.0, 0.0, os1.str(), g_IdView2);
    viewer->addLine<pcl::PointXYZ, pcl::PointXYZ> (pointsProcessedB[i], pointsProcessedB[i + 1], 1.0, 1.0, 0.0, os2.str(), g_IdView2);
  }
}

void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event, void* viewer_void)
{
  pcl::visualization::PCLVisualizer *viewer = static_cast<pcl::visualization::PCLVisualizer *> (viewer_void);

  if(event.keyDown()){
    if(event.getKeySym() == "p"){
        cout<<"The curves were reploted: "<<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "i"){
        g_Test.m_Param += 0.01;
        cout<<"New value: " << g_Test.m_Param<<endl;
        reshape(viewer);
    } else if(event.getKeySym() == "d"){
        g_Test.m_Param -= 0.01;
        cout<<"New value: " << g_Test.m_Param<<endl;
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
      std::cout<<" Default visualization with process " << g_np <<endl;
    }

    // ---- Init Test -----
    g_Test.init();

    // ---- Initialize visualization ------
    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = viewCurvesVis();

    // -----Main loop-----
    while (!viewer->wasStopped ())
    {
        viewer->spinOnce (100);
        boost::this_thread::sleep (boost::posix_time::microseconds (100000));
    }

    return 0;
}
