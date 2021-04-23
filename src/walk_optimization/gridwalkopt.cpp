// Adopted from example given at
// https://github.com/mlpack/examples/blob/master/neural_network_regression/nn_regression.cpp
// Compile with:
// g++ -std=c++11 -Wa,-mbig-obj -O2 -o optimize gridwalkopt.cpp `pkg-config --libs mlpack armadillo` -fopenmp
#include <mlpack/prereqs.hpp>
#include <mlpack/core.hpp>
#include <mlpack/methods/ann/loss_functions/mean_squared_error.hpp>
#include <mlpack/core/data/scaler_methods/min_max_scaler.hpp>
#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/core/data/split_data.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/init_rules/he_init.hpp>
#include <ensmallen.hpp>

#include <iostream>
#include <string>

using namespace mlpack;
using namespace mlpack::ann;
using namespace ens;

/*
 * Function to calculate MSE for arma::cube.
 */
double MSE(arma::mat& pred, arma::mat& Y)
{
  return metric::SquaredEuclideanDistance::Evaluate(pred, Y) / (Y.n_elem);
}

int main(int argc, char* argv [])
{
  if (argc != 3) {
    std::cout << "Must pass training and testing data files" << std::endl;
    return 1;
  }

  // In Armadillo rows represent features, columns represent data points.
  // Input is tranposed upon reading
  arma::mat training_data;
  arma::mat testing_data;

  std::cout << "Reading training grid walk parameters and results from ";
  std::cout << argv[1] << std::endl;
  bool loadedDataset = data::Load(std::string(argv[1]), training_data, true, true, arma::csv_ascii);
  // If dataset is not loaded correctly, exit.
  if (!loadedDataset) {
    std::cout << "Error reading data file" << argv[1] << std::endl;
    return 2;
  }
  std::cout << "Reading testing grid walk parameters and results from ";
  std::cout << argv[2] << std::endl;
  loadedDataset = data::Load(std::string(argv[2]), testing_data, true, true, arma::csv_ascii);
  // If dataset is not loaded correctly, exit.
  if (!loadedDataset) {
    std::cout << "Error reading data file" << argv[2] << std::endl;
    return 2;
  }

  // The train and valid datasets contain both - the features as well as the
  // prediction. Split these into separate matrices.
  arma::mat training_outputs = training_data.row(training_data.n_rows - 1);
  arma::mat testing_outputs = testing_data.row(testing_data.n_rows - 1);
  training_data.shed_row(training_data.n_rows - 1);
  testing_data.shed_row(testing_data.n_rows - 1);
  
  //! - H1: The number of neurons in the 1st layer.
  constexpr int H1 = 64;
  //! - H2: The number of neurons in the 2nd layer.
  constexpr int H2 = 128;
  //! - H3: The number of neurons in the 3rd layer.
  constexpr int H3 = 64;

  // Specifying the NN model.
  FFN<MeanSquaredError<>, HeInitialization> model;

  // This intermediate layer is needed for connection between input
  // data and the next LeakyReLU layer.
  // Parameters specify the number of input features and number of
  // neurons in the next layer.
  model.Add<Linear<>>(training_data.n_rows, H1);
  // Activation layer:
  model.Add<LeakyReLU<>>();
  // Connection layer between two activation layers.
  model.Add<Linear<>>(H1, H2);
  // Activation layer.
  model.Add<LeakyReLU<>>();
  // Connection layer.
  model.Add<Linear<>>(H2, H3);
  // Activation layer.
  model.Add<LeakyReLU<>>();
  // Connection layer => output.
  // The output of one neuron is the regression output for one set of PMF
  // parameters
  model.Add<Linear<>>(H3, 1);

  // Set parameters for the Stochastic Gradient Descent (SGD) optimizer.
  ens::Adam optimizer(
      1e-3,  // Step size of the optimizer.
      36, // Batch size. Number of data points that are used in each
                  // iteration.
      0.9,        // Exponential decay rate for the first moment estimates.
      0.999,      // Exponential decay rate for the weighted infinity norm
                  // estimates.
      1e-8, // Value used to initialise the mean squared gradient parameter.
      1e8, // Max number of iterations, 0 = unlimited
      1e-2); // Tolerance.


  

  model.Train(training_data,
              training_outputs,
              optimizer,
              // PrintLoss Callback prints loss for each epoch.
              ens::PrintLoss());
              // Progressbar Callback prints progress bar for each epoch.
              // ens::ProgressBar(5),
              // Stops the optimization process if the loss stops decreasing
              // or no improvement has been made. This will terminate the
              // optimization once we obtain a minima on training set.
              // ens::EarlyStopAtMinLoss(20));

  std::cout << "Finished training." << std::endl;

  // Create predictions on the dataset.
  arma::mat predicted_num_steps;
  model.Predict(testing_data, predicted_num_steps);

  // We will test the quality of our model by calculating Mean Squared Error on
  // validation dataset.
  double testing_MSE = MSE(testing_outputs, predicted_num_steps);
  std::cout << "Mean Squared Error on Predicted Number of Steps: ";
  std::cout << testing_MSE << std::endl;

  // Save the prediction results.
  bool saved = data::Save("testing_predictions.csv", predicted_num_steps, true, true, arma::csv_ascii);
  if (!saved) {
    std::cout << "Failed to save prediction results" << std::endl;
    return 2;
  }

  return 0;
}