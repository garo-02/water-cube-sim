#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>

// ----------------------------
// ADJUSTABLE PARAMETERS
// ----------------------------

// Grid resolution (NxN over [0,1]x[0,1])
// Bigger = smoother surface, but slower
const int N = 64;

// Number of time steps / frames
const int NUM_FRAMES = 200;

// Time step size
// Too large -> instability
const double dt = 0.02;

// Wave speed factor (acts like gravity in shallow-water sense)
// Larger -> faster waves
const double wave_speed = 1.0;

// Simple damping coefficient
// 0.0 = waves never die
// ~0.01–0.05 = realistic settling
const double damping = 0.02;

// Disturbance parameters
const double disturb_x = 0.5; // location in [0,1]
const double disturb_y = 0.5;
const double disturb_amp = 0.1;    // wave height
const double disturb_sigma = 0.08; // width of bump

// Output directory
const std::string OUTPUT_DIR = "output/";

// ----------------------------

int main()
{
  // Height field at current and next timestep
  std::vector<std::vector<double>> h(N, std::vector<double>(N, 0.0));
  std::vector<std::vector<double>> h_new(N, std::vector<double>(N, 0.0));

  // Initialize flat water surface
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      h[i][j] = 0.5; // base water height

  // Apply a one-time Gaussian disturbance
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      double x = double(i) / (N - 1);
      double y = double(j) / (N - 1);

      double dx = x - disturb_x;
      double dy = y - disturb_y;
      double r2 = dx * dx + dy * dy;

      h[i][j] += disturb_amp * std::exp(-r2 / (2.0 * disturb_sigma * disturb_sigma));
    }
  }

  // Main simulation loop
  for (int frame = 0; frame < NUM_FRAMES; ++frame)
  {

    // Simple discrete wave update (very basic)
    for (int i = 1; i < N - 1; ++i)
    {
      for (int j = 1; j < N - 1; ++j)
      {

        // Laplacian (measures curvature)
        double lap =
            h[i + 1][j] + h[i - 1][j] +
            h[i][j + 1] + h[i][j - 1] -
            4.0 * h[i][j];

        // Update rule
        h_new[i][j] = h[i][j] + wave_speed * lap * dt - damping * (h[i][j] - 0.5);
      }
    }

    // Copy boundaries unchanged (solid walls)
    for (int i = 0; i < N; ++i)
    {
      h_new[i][0] = h_new[i][1];
      h_new[i][N - 1] = h_new[i][N - 2];
      h_new[0][i] = h_new[1][i];
      h_new[N - 1][i] = h_new[N - 2][i];
    }

    // Swap buffers
    h.swap(h_new);

    // ----------------------------
    // WRITE CSV FRAME
    // ----------------------------
    std::ostringstream fname;
    fname << OUTPUT_DIR
          << "frame_"
          << std::setw(4) << std::setfill('0')
          << frame << ".csv";

    std::ofstream file(fname.str());
    if (!file)
    {
      std::cerr << "Failed to write " << fname.str() << "\n";
      return 1;
    }

    for (int i = 0; i < N; ++i)
    {
      for (int j = 0; j < N; ++j)
      {
        file << h[i][j];
        if (j < N - 1)
          file << ",";
      }
      file << "\n";
    }

    std::cout << "Wrote frame " << frame << "\n";
  }

  return 0;
}
