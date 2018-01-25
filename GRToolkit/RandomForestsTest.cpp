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
 GRT Random Forests Example

 This examples demonstrates how to initialize, train, and use the RandomForests algorithm for classification.

 RandomForests are an ensemble learning method that operate by building a number of decision trees at training
 time and outputting the class with the majority vote over all the trees in the ensemble.

 In this example we create an instance of a RandomForests algorithm and then train a model using some pre-recorded training data.
 The trained RandomForests model is then used to predict the class label of some test data.

 This example shows you how to:
 - Create and initialize the RandomForests algorithm
 - Load some ClassificationData from a file and partition the training data into a training dataset and a test dataset
 - Train a RandomForests model using the training dataset
 - Test the RandomForests model using the test dataset
 - Manually compute the accuracy of the classifier

You should run this example with one argument pointing to the data you want to load. A good dataset to run this example is acc-orientation.grt, which can be found in the GRT data folder.
*/

//You might need to set the specific path of the GRT header relative to your project
#include <GRT/GRT.h>
using namespace GRT;
using namespace std;

int main(int argc, const char * argv[])
{
    //Parse the data filename from the argument list
    if( argc != 2 ){
        cout << "Error: failed to parse data filename from command line. You should run this example with one argument pointing to the data filename!\n";
        return EXIT_FAILURE;
    }
    const string filename = argv[1];

    InfoLog info("");

    //Create a new RandomForests instance
    RandomForests forest;

    //Set the number of trees in the forest - 10, 30 or 100
    //https://stats.stackexchange.com/questions/36165/does-the-optimal-number-of-trees-in-a-random-forest-depend-on-the-number-of-pred
    forest.setForestSize( 100 );

    //Set the number of random candidate splits that will be used to choose the best splitting values
    //More steps will give you a better model, but will take longer to train
    forest.setNumRandomSplits( 1000 );

    //Set the maximum depth of the tree
    forest.setMaxDepth( 20 );

    //Set the minimum number of samples allowed per node
    forest.setMinNumSamplesPerNode( 5 );

    //Set the training mode
    forest.setTrainingMode(Tree::TrainingMode::BEST_ITERATIVE_SPILT);

    //Set if remove features at split
    forest.setRemoveFeaturesAtEachSplit(false);

    //Set tree node type
    forest.setDecisionTreeNode(DecisionTreeThresholdNode());

    //Load some training data to train the classifier
    ClassificationData trainingData;

    cout << "Loading Training Data\n";
    if( !trainingData.load( filename ) ){
        cout << "Failed to load training data: " << filename << endl;
        return EXIT_FAILURE;
    }

    //Use 30% of the training dataset to create a test dataset
    ClassificationData testData = trainingData.split( 70, true );

    const UINT numTestSamples = testData.getNumSamples();
    const UINT numTrainSamples = trainingData.getNumSamples();
    const UINT numInputDimensions = trainingData.getNumDimensions();
    const UINT numOutputDimensions = numInputDimensions * 0.3;

    info << "Num Test Samples: " << numTestSamples << std::endl;
    info << "Num Train Samples: " << numTrainSamples << std::endl;
    info << "Num Input Dimensions: " << numInputDimensions << std::endl;
    info << "Num Output Dimensions (Principal Components): " << numOutputDimensions << std::endl;

    double median = 0.0;
    size_t crossValid = 3;
    for (size_t i = 0; i < crossValid; i++) {

      //Create a new pipeline
      GestureRecognitionPipeline pipeline;

      // Create an instance of the PCA feature extraction module and add it to the pipeline
      pipeline << PCA(numInputDimensions, numOutputDimensions);

      // Add classifier to the pipeline
      pipeline << forest;

      // Get a pointer to the PCA module we just added to the pipeline so we can train it
      {
        PCA *pca = pipeline.getFeatureExtractionModule<PCA>(0);
        info << "Training PCA..." << std::endl;
        if (!pca->train(trainingData)) {
          info << "Failed to train PCA model!" << std::endl;
          return EXIT_FAILURE;
        }
      }

      //Train the classifier
      if( !pipeline.train( trainingData ) ){
          cout << "Failed to train classifier!\n";
          return EXIT_FAILURE;
      }

      //Test the accuracy of the model on the test data
      double accuracy = 0;
      int isRecognized = 0;
      for(UINT i=0; i<testData.getNumSamples(); i++){
          //Get the i'th test sample
          UINT classLabel = testData[i].getClassLabel();
          VectorFloat inputVector = testData[i].getSample();

          //Perform a prediction using the classifier
          bool predictSuccess = pipeline.predict( inputVector );

          if( !predictSuccess ){
              cout << "Failed to perform prediction for test sampel: " << i <<"\n";
              return EXIT_FAILURE;
          }

          //Get the predicted class label
          UINT predictedClassLabel = pipeline.getPredictedClassLabel();
          VectorDouble classLikelihoods = pipeline.getClassLikelihoods();
          VectorDouble classDistances = pipeline.getClassDistances();
          Float maximumLikelihood = pipeline.getMaximumLikelihood();
          //Update the accuracy
      		if( classLabel == predictedClassLabel ) {
      				accuracy++;
      				isRecognized = 1;
      		} else {
      				isRecognized = 0;
      		}

      		// cout << i <<  " " << classLabel << " " << predictedClassLabel << " " << maximumLikelihood << " " << isRecognized << endl;
      }

      median += accuracy/double(testData.getNumSamples())*100.0;
      // cout << "Test Accuracy: " << accuracy/double(testData.getNumSamples())*100.0 << "%" << endl;
    }

    info << "A = " << median / crossValid << endl;

    return EXIT_SUCCESS;
}
