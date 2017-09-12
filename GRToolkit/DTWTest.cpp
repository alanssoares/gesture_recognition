/*
 GRT MIT License
 Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 GRT DTW Example
 This examples demonstrates how to initialize, train, and use the DTW algorithm for classification.

 The Dynamic Time Warping (DTW) algorithm is a powerful classifier that works very well for recognizing temporal gestures.

 In this example we create an instance of an DTW algorithm and then train the algorithm using some pre-recorded training data.
 The trained DTW algorithm is then used to predict the class label of some test data.

 This example shows you how to:
 - Create an initialize the DTW algorithm
 - Load some LabelledTimeSeriesClassificationData from a file and partition the training data into a training dataset and a test dataset
 - Trim any periods of non-movement from the start and end of each timeseries recording
 - Train the DTW algorithm using the training dataset
 - Test the DTW algorithm using the test dataset
 - Manually compute the accuracy of the classifier
*/

//You might need to set the specific path of the GRT header relative to your project
// #include "GRT.h"
#include <GRT/GRT.h>
using namespace GRT;
using namespace std;

int main (int argc, const char * argv[])
{

	// Constants
	// TEMPLATE_THRESHOLDS=0,CLASS_LIKELIHOODS,THRESHOLDS_AND_LIKELIHOODS
	// ABSOLUTE_DIST=0, EUCLIDEAN_DIST, NORM_ABSOLUTE_DIST

	// Sets if the training and prediction data should be scaled to a specific range.  Default value is useScaling = false
	bool useScaling = true;

	// Sets if null rejection will be used for the realtime prediction.  If useNullRejection is set to true then the predictedClassLabel will be set to 0 (which is the default null label) if the distance between the inputVector and the closest template is greater than the null rejection threshold for the top predicted class.  The null rejection threshold is computed for each class during the training phase. Default value is useNullRejection = false
	bool useNullRejection = false;

	// Sets if the DTW warping path should be constrained within a specific distance from the main radius of the cost matrix.  Default value = true
	bool constrainWarpingPath = false;

	// Sets if each timeseries should be offset by the first sample in the timeseries. Default value = false
	bool offsetUsingFirstSample = false;

	// Sets if the input timeseries should be smoothed (i.e. averaged and downsampled). Default value = false
	bool useSmoothing = false;

	// Sets the null rejection coefficient, this is a multipler controlling the null rejection threshold for each class.  This will only be used if the useNullRejection parameter is set to true.  Default value is nullRejectionCoeff = 3.0
	float nullRejectionCoeff = 3.0;

	// Controls the radius of the warping path, which is used if the dtwConstrain is set to true. Should be a value between [0 1]. Default value = 0.2
	float radius = 0.2;

	// Set the null rejection threshold for likelihoods when CLASS_LIKELIHOODS or THRESHOLDS_AND_LIKELIHOODS modes are used for rejectionMode. Default value = 0.99
	float nullRejectionLikelihoodThreshold = 0.99;

	// Sets the method used for null rejection. The options are TEMPLATE_THRESHOLDS, CLASS_LIKELIHOODS or THRESHOLDS_AND_LIKELIHOODS.  Default = TEMPLATE_THRESHOLDS
	UINT rejectionMode = DTW::THRESHOLDS_AND_LIKELIHOODS;

	// Controls the amount of downsampling if the useSmoothing parameter is set to true. Default value = 5
	UINT smoothingFactor = 5;

	// constrainWarpingPath -> radius
	// useSmoothing -> smoothingFactor
	// useNullRejection -> rejectionMode -> nullRejectionCoeff

	//Create a new DTW instance, using the default parameters
	DTW dtw(useScaling, useNullRejection, nullRejectionCoeff, rejectionMode, constrainWarpingPath, radius,
	offsetUsingFirstSample, useSmoothing, nullRejectionLikelihoodThreshold);

	//Parse the training data filename from the command line
	if( argc != 2 ){
			cout << "Error: failed to parse data filename from command line. You should run this example with one argument pointing to a data file\n";
			return EXIT_FAILURE;
	}
	const string filename = argv[1];

	//Load some training data to train the classifier - the DTW uses TimeSeriesClassificationData
	TimeSeriesClassificationData trainingData;

	if( !trainingData.load(filename) ){
		cout << "Failed to load training data!\n";
		return EXIT_FAILURE;
	}

	//Use 30% of the training dataset to create a test dataset
	TimeSeriesClassificationData testData = trainingData.split( 70 );

	//Trim the training data for any sections of non-movement at the start or end of the recordings
	dtw.enableTrimTrainingData(true,0.1,90);

	//Train the classifier
	if( !dtw.train( trainingData ) ){
		cout << "Failed to train classifier!\n";
		return EXIT_FAILURE;
	}

	//Save the DTW model to a file
	if( !dtw.save("DTWModel.grt") ){
		cout << "Failed to save the classifier model!\n";
		return EXIT_FAILURE;
	}

	//Load the DTW model from a file
	if( !dtw.load("DTWModel.grt") ){
		cout << "Failed to load the classifier model!\n";
		return EXIT_FAILURE;
	}

	//Use the test dataset to test the DTW model
	double accuracy = 0;
	int isRecognized = 0;
	for(UINT i=0; i<testData.getNumSamples(); i++){
		//Get the i'th test sample - this is a timeseries
		UINT classLabel = testData[i].getClassLabel();
		MatrixDouble timeseries = testData[i].getData();

		//Perform a prediction using the classifier
		if( !dtw.predict( timeseries ) ){
			cout << "Failed to perform prediction for test sampel: " << i <<"\n";
			return EXIT_FAILURE;
		}

		//Get the predicted class label
		UINT predictedClassLabel = dtw.getPredictedClassLabel();
		double maximumLikelihood = dtw.getMaximumLikelihood();
		VectorDouble classLikelihoods = dtw.getClassLikelihoods();
		VectorDouble classDistances = dtw.getClassDistances();

		//Update the accuracy
		if( classLabel == predictedClassLabel ) {
				accuracy++;
				isRecognized = 1;
		} else {
				isRecognized = 0;
		}

		cout << i <<  " " << classLabel << " " << predictedClassLabel << " " << maximumLikelihood << " " << isRecognized << endl;
	}

	cout << "Test Accuracy: " << accuracy/double(testData.getNumSamples())*100.0 << "%" << endl;

	return EXIT_SUCCESS;
}
