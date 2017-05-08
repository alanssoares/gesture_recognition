#!/bin/bash

echo "***** Compiling ****** "

echo "Start ->>>>>>> Decision Tree..."
g++ -std=c++11 -c DecisionTreeTest.cpp -I/usr/local/include
g++ DecisionTreeTest.o -o startDecisionTree -I/usr/local/include -L/usr/local/lib -lgrt
echo "Finish ->>>>>> Decision Tree..."

# echo "Start ->>>>>>> GMM..."
# g++ -std=c++11 -c GMMTest.cpp -I/usr/local/include
# g++ GMMTest.o -o startGMM -I/usr/local/include -L/usr/local/lib -lgrt
# echo "Finish ->>>>>> GMM..."

echo "Start ->>>>>>> DTW..."
g++ -std=c++11 -c DTWTest.cpp -I/usr/local/include
g++ DTWTest.o -o startDTW -I/usr/local/include -L/usr/local/lib -lgrt
echo "Finish ->>>>>> DTW..."

echo "Start ->>>>>>> HMM..."
g++ -std=c++11 -c HMMTest.cpp -I/usr/local/include
g++ HMMTest.o -o startHMM -I/usr/local/include -L/usr/local/lib -lgrt
echo "Finish ->>>>>> HMM..."

# echo "Start ->>>>>>> KNN..."
# g++ -std=c++11 -c KNNTest.cpp -I/usr/local/include
# g++ KNNTest.o -o startKNN -I/usr/local/include -L/usr/local/lib -lgrt
# echo "Finish ->>>>>> KNN..."

echo "Start ->>>>>>> SVM..."
g++ -std=c++11 -c SVMTest.cpp -I/usr/local/include
g++ SVMTest.o -o startSVM -I/usr/local/include -L/usr/local/lib -lgrt
echo "Finish ->>>>>> SVM..."

# echo "Start ->>>>>>> Random Forests..."
# g++ -std=c++11 -c RandomForestsTest.cpp -I/usr/local/include
# g++ RandomForestsTest.o -o startRandomForests -I/usr/local/include -L/usr/local/lib -lgrt
# echo "Finish ->>>>>> Random Forests..."

rm *.o
