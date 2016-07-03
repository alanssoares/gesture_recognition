//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "../Commons/commons.hpp"

/*
    1 - Carregar os gestos
    2 - Chamar o método para normalizar e centralizar na origem
    3 - Chamar o método para suavizar a trajetória
    4 - Aplicar o método de curvatura para reduzir a quantidade de pontos
    5 - Salvar em um novo arquivo os gestos processados
*/
void improveGestures();

int main(int argc, char* argv[])
{
    FileUtil::getInstance().loadGestures();
    
    improveGestures();
    
    FileUtil::getInstance().saveAll();

	return 0;
}

/**
    Improve all the gestures applying the methods of smooth, center and reduce
*/
void improveGestures(){  
    FileUtil& fileUtil = FileUtil::getInstance();
    size_t n1 = fileUtil.mGesturesOneHand.size();
    size_t n2 = fileUtil.mGesturesTwoHands.size();
    
    for (int i = 0; i < n1; i++){
        //PRINT("A[" << i << "] - Before : " << fileUtil.mGesturesOneHand[i].handOne.positions.size());
        fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesOneHand[i].handOne.positions);
        fileUtil.mGesturesOneHand[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesOneHand[i].handOne.positions);
        //PRINT("A[" << i << "] - After : " << fileUtil.mGesturesOneHand[i].handOne.positions.size());
    }
    
    for (int i = 0; i < n2; i++){
        //PRINT("B_1[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handOne.positions);
        fileUtil.mGesturesTwoHands[i].handOne.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handOne.positions);
        //PRINT("B_1[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handOne.positions.size());
        //PRINT("B_2[" << i << "] - Before : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
        fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::normCenterOrigin(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        fileUtil.mGesturesTwoHands[i].handTwo.positions = MathUtil::smoothAndReduce(fileUtil.mGesturesTwoHands[i].handTwo.positions);
        //PRINT("B_2[" << i << "] - After : " << fileUtil.mGesturesTwoHands[i].handTwo.positions.size());
    }
}