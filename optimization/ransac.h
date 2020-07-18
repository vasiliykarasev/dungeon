#pragma once

#include <Eigen/Core>
#include <memory>
#include <random>

namespace dungeon {

struct RansacSolution;
struct RansacOptions;
struct RansacModelBase;

// Fits the provided model to observations using RANSAC.
// To use this function, the user needs to:
//   - Specify their model as a subclass of RansacModelBase (implementing
//     appropriate functions).
//   - Appropriately populate RansacOptions.
// Estimated model parameters will be in RansacSolution::params.
//
// Note: input to the model as well as model parameters are represented using
// Eigen::MatrixXf. This allows us to avoid templating on input/output type
// pairs, but is potentially slower (due to dynamic memory allocation).
RansacSolution RansacFit(std::unique_ptr<RansacModelBase> model,
                         const std::vector<Eigen::MatrixXf>& observations,
                         const RansacOptions& opt);

// Structure containing the solution to a RANSAC problem.
struct RansacSolution {
  // Whether the solution is valid.
  bool success{false};
  // Estimated model parameters.
  Eigen::MatrixXf params{};
  // Number of points determined to be inliers.
  int num_inliers{0};
  // Error aggregated over all points determined to be inliers.
  double error{std::numeric_limits<double>::max()};
  // Collection of inliers (length is the same as the number of observations fed
  // into RansacFit()).
  std::vector<bool> inliers;
};

// Options for solving a RANSAC problem.
struct RansacOptions {
  // Number of candidate models to evaluate.
  int num_iterations = 100;
  // Threshold used to decide whether a point is an inlier.
  float inlier_threshold = 0;
  // Minimum fraction of points deemed inliers, needed to determine whether
  // the model succeeded.
  // This value should be greater than the minimum fraction of observations
  // needed to fit the model, and should be lower than the expected overall
  // inlier ratio.
  float min_inlier_fraction = 0.5;
  // Seed used in random generator.
  int random_seed = 0;
};

// Interface for the model that the user should implement.
struct RansacModelBase {
  // Fit model to observations.
  virtual void Fit(const std::vector<Eigen::MatrixXf>& pts) = 0;

  // Evaluates a discrepancy (an error) between the current model fit and the
  // provided observation.
  virtual double EvaluateError(const Eigen::MatrixXf& pt) const = 0;

  // Returns model parameters as Eigen::MatrixXf.
  virtual Eigen::MatrixXf params() const = 0;

  // Smallest number of observations needed to fit the model.
  virtual int min_num_observations() const = 0;
};

}  // namespace dungeon
