//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "util/FileUtil.h"
#include "util/ConstantsUtil.h"
#include "util/MathUtil.h"

/*
 Definition of Global variables
*/
 FileUtil g_FileUtil;

/*
    1 - Carregar os gestos
    2 - Chamar o método para normalizar e centralizar na origem
    3 - Chamar o método para suavizar a trajetória
    4 - Aplicar o método de curvatura para reduzir a quantidade de pontos
    5 - Salvar em um novo arquivo os gestos processados
*/
void                   saveAll();
void                   improveGestures();
std::vector<XnPoint3D> normCenterOrigin(std::vector<XnPoint3D> trajectory);
std::vector<XnPoint3D> smoothAndReduce(std::vector<XnPoint3D> trajectory);

int main(int argc, char* argv[])
{
    // Load the gestures data
    g_FileUtil.loadGestures();
    
    improveGestures();
    
	return 0;
}

std::vector<XnPoint3D> normCenterOrigin(std::vector<XnPoint3D> trajectory) {
    //Translate the hand trajectory to origin
    trajectory = MathUtil::translateToOrigin(trajectory);
    //Normalize between the interval -1 to 1
    return MathUtil::normalizeTrajectory(trajectory);
}

std::vector<XnPoint3D> smoothAndReduce(std::vector<XnPoint3D> trajectory) {
    //Smooth the trajectory
    trajectory = MathUtil::smooth(trajectory);
    //Remove points according with curvature
    return MathUtil::reduceByCurvature(trajectory);
}

void saveAll(){

}

void improveGestures(){
    std::vector<type_gesture> templates1 = g_FileUtil.getGesturesOneHand();
    std::vector<type_gesture> templates2 = g_FileUtil.getGesturesTwoHands();    
    
    size_t n1 = templates1.size();
    size_t n2 = templates2.size();
    
    for (int i = 0; i < n1; i++){
        PRINT("A[" << i << "] - Before : " << templates1[i].handOne.positions.size());
        templates1[i].handOne.positions = smoothAndReduce(normCenterOrigin(templates1[i].handOne.positions));
        PRINT("A[" << i << "] - After : " << templates1[i].handOne.positions.size());
    }
    
    for (int i = 0; i < n2; i++){
        PRINT("B_1[" << i << "] - Before : " << templates2[i].handOne.positions.size());
        templates2[i].handOne.positions = smoothAndReduce(normCenterOrigin(templates2[i].handOne.positions));
        PRINT("B_1[" << i << "] - After : " << templates2[i].handOne.positions.size());
        PRINT("B_2[" << i << "] - Before : " << templates2[i].handTwo.positions.size());
        templates2[i].handTwo.positions = smoothAndReduce(normCenterOrigin(templates2[i].handTwo.positions));
        PRINT("B_2[" << i << "] - After : " << templates2[i].handTwo.positions.size());
    }

    saveAll();
}