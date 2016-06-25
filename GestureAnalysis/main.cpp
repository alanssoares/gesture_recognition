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
void saveAll();
void improveGestures();
std::vector<XnPoint3D> normCenterOrigin(std::vector<XnPoint3D> trajectory);
std::vector<XnPoint3D> smoothAndReduce(std::vector<XnPoint3D> trajectory);

int main(int argc, char* argv[])
{
    g_FileUtil.loadGestures();
    
    improveGestures();
    
    g_FileUtil.saveAll();

	return 0;
}

/**
    Improve all the gestures applying the methods of smooth, center and reduce
*/
void improveGestures(){  
    
    size_t n1 = g_FileUtil.mGesturesOneHand.size();
    size_t n2 = g_FileUtil.mGesturesTwoHands.size();
    
    for (int i = 0; i < n1; i++){
        //PRINT("A[" << i << "] - Before : " << g_FileUtil.mGesturesOneHand[i].handOne.positions.size());
        g_FileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::normCenterOrigin(g_FileUtil.mGesturesOneHand[i].handOne.positions);
        g_FileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::smoothAndReduce(g_FileUtil.mGesturesOneHand[i].handOne.positions);
        //PRINT("A[" << i << "] - After : " << g_FileUtil.mGesturesOneHand[i].handOne.positions.size());
    }
    
    for (int i = 0; i < n2; i++){
        //PRINT("B_1[" << i << "] - Before : " << g_FileUtil.mGesturesTwoHands[i].handOne.positions.size());
        g_FileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::normCenterOrigin(g_FileUtil.mGesturesTwoHands[i].handOne.positions);
        g_FileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::smoothAndReduce(g_FileUtil.mGesturesTwoHands[i].handOne.positions);
        //PRINT("B_1[" << i << "] - After : " << g_FileUtil.mGesturesTwoHands[i].handOne.positions.size());
        //PRINT("B_2[" << i << "] - Before : " << g_FileUtil.mGesturesTwoHands[i].handTwo.positions.size());
        g_FileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::normCenterOrigin(g_FileUtil.mGesturesTwoHands[i].handTwo.positions);
        g_FileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::smoothAndReduce(g_FileUtil.mGesturesTwoHands[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << g_FileUtil.mGesturesTwoHands[i].handTwo.positions.size());
    }
}