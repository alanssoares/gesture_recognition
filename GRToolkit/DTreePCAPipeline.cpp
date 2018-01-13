/*
 This example demonstrates how to use the PCA Feature Extraction module with a GestureRecognitionPipeline.
 The PCA Feature Extraction module provides an interface for the GRT PrincipalComponentAnalysis algorithm,
 enabling the PrincipalComponentAnalysis algorithm to be used within a Gesture Recognition Pipeline.
 This example shows how to:
 - Load an existing classification data from a file.
 - Create a new PCA feature extraction instance and add it to a GestureRecognitionPipeline.
 - Add a GRT classifier (KNN) to the pipeline.
 - Train the PCA model.
 - Train the KNN model, using the PCA for feature extraction and input to the KNN classifier.
 - Print out the results.
 Run this example with one argument, pointing to the input classification data file you want to load.
 For example, running this from the grt build directory: ./PCAPipelineExample ../../data/IrisData.grt
 */

#include <GRT/GRT.h>
using namespace GRT;

int main (int argc, const char * argv[]) {

  InfoLog info("");

  // Parse the filename from the input
  if (argc != 2) {
    info << "Failed to parse filename from input, run this example with the path to the file you want to parse." << std::endl;
    return EXIT_FAILURE;
  }
  const std::string filename = argv[1];
  double accuracy = 0, bestAccuracy = 0;
  int isRecognized = 0;
  UINT bestMinNumSamplesPerNode = 5, bestMaxDepth = 20, bestSplittingSteps = 150;

  // Set the node that the DecisionTree will use - different nodes may result in different decision boundaries and some nodes may provide better accuracy than others on specific classification tasks
  //The current node options are:
  //- DecisionTreeClusterNode - 1
  //- DecisionTreeThresholdNode - 2
  // const DecisionTreeNode &decisionTreeNode = DecisionTreeClusterNode();
  const DecisionTreeNode &decisionTreeNode = DecisionTreeThresholdNode();
  size_t mD = 20;
  size_t mS = 5;
  size_t mT = 1000;

  // The minimum number of samples that are allowed per node, if the number of samples is below that, the node will become a leafNode.  Default value = 5
  const UINT minNumSamplesPerNode = mS;

  // The maximum depth of the tree. Default value = 10
  const UINT maxDepth = mD;

  // Sets if a feature is removed at each split so it can not be used again. Default value = false
  const bool removeFeaturesAtEachSplit = false;

  // Sets the training mode, this should be one of the TrainingMode enums. Default value = BEST_ITERATIVE_SPILT
  // If the trainingMode is set to BEST_ITERATIVE_SPILT, then the numSplittingSteps controls how many iterative steps there will be per feature.
  // If the trainingMode is set to BEST_RANDOM_SPLIT, then the numSplittingSteps controls how many random searches there will be per feature.
  const Tree::TrainingMode trainingMode = Tree::TrainingMode::BEST_ITERATIVE_SPILT;
  // const Tree::TrainingMode trainingMode = Tree::TrainingMode::BEST_RANDOM_SPLIT;

  // Set the number of steps that will be used to choose the best splitting values more steps will give you a better model, but will take longer to train
  const UINT numSplittingSteps = mT;

  // Sets if the training and real-time data should be scaled between [0 1]. Default value = false
  const bool useScaling = false;

  //Create a new DecisionTree instance
  DecisionTree dTree(decisionTreeNode, minNumSamplesPerNode, maxDepth,
    removeFeaturesAtEachSplit, trainingMode, numSplittingSteps, useScaling);

  ClassificationData trainingData;

  info << "Loading data..." << std::endl;
  if (!trainingData.load(filename)) {
    info << "Failed to load data from file: " << filename << std::endl;
    return EXIT_FAILURE;
  }

  //Use 30% of the training dataset to create a test dataset
  ClassificationData testData = trainingData.split( 30, true );

  //Create a new pipeline
  GestureRecognitionPipeline pipeline;

  const UINT numTestSamples = testData.getNumSamples();
  const UINT numTrainSamples = trainingData.getNumSamples();
  const UINT numInputDimensions = trainingData.getNumDimensions();
  const UINT numOutputDimensions = numInputDimensions * 0.4;

  info << "Num Test Samples: " << numTestSamples << std::endl;
  info << "Num Train Samples: " << numTrainSamples << std::endl;
  info << "Num Input Dimensions: " << numInputDimensions << std::endl;
  info << "Num Output Dimensions (Principal Components): " << numOutputDimensions << std::endl;

  // Create an instance of the PCA feature extraction module and add it to the pipeline
  pipeline << PCA(numInputDimensions, numOutputDimensions);

  // Add a DTree Classifier to the pipeline
  pipeline << dTree;

  // Get a pointer to the PCA module we just added to the pipeline so we can train it
  // This is a little bit of a hack until the pipeline supports automatic training of the feature modules
  {
    PCA *pca = pipeline.getFeatureExtractionModule<PCA>(0);
    info << "Training PCA..." << std::endl;
    if (!pca->train(trainingData)) {
      info << "Failed to train PCA model!" << std::endl;
      return EXIT_FAILURE;
    }
  }

  // Train the model, this first project the data through the trained PCA module before inputting it
  // into the classifier
  info << "Training model..." << std::endl;
  if (!pipeline.train(trainingData)) {
    info << "Failed to train pipeline model!" << std::endl;
    return EXIT_FAILURE;
  }

  info << "Model trained." << std::endl;

  //Test the accuracy of the model on the test data
  accuracy = 0;
  isRecognized = 0;

  for(UINT i=0; i<testData.getNumSamples(); i++){
      //Get the i'th test sample
      UINT classLabel = testData[i].getClassLabel();
      VectorFloat inputVector = testData[i].getSample();

      //Perform a prediction using the classifier
      bool predictSuccess = pipeline.predict( inputVector );

      if( !predictSuccess ){
          info << "Failed to perform prediction for test sampel: " << i << std::endl;
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

      info << i <<  ";" << classLabel << ";" << predictedClassLabel << ";" << maximumLikelihood << ";" << isRecognized << std::endl;
  }

  info << "Nº Training = " << trainingData.getNumSamples() << ", Nº Test = " << testData.getNumSamples() << ", Accuracy = " << accuracy/double(testData.getNumSamples()) * 100.0<< '\n';

  return EXIT_SUCCESS;
}
