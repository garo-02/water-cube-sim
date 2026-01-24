#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>

// ----------------------------
// ADJUSTABLE PARAMETERS
// ----------------------------

// Grid resolution (NxN over [0,1]x[0,1])
const int N = 64;

// Number of time steps / frames
const int NUM_FRAMES = 500;

// Time step size (MUST be small for stability)
const double dt = 0.001;

// Spatial resolution
const double dx = 1.0 / (N - 1);
const double inv_dx2 = 1.0 / (dx * dx);

// Wave speed
const double wave_speed = 3.0;

// Damping (controls under/over damping)
// ~1–4 = underdamped
const double damping = 2.0;

// Disturbance parameters
const double disturb_x = 0.5;
const double disturb_y = 0.5;
const double disturb_amp = 10;
const double disturb_sigma = 0.08;

// Output directory
const std::string OUTPUT_DIR = "output/";

// ----------------------------

int main()
{
  // Height field
  std::vector<std::vector<double>> h(N, std::vector<double>(N, 0.0));
  std::vector<std::vector<double>> h_new(N, std::vector<double>(N, 0.0));

  // Velocity field
  std::vector<std::vector<double>> v(N, std::vector<double>(N, 0.0));

  // -------------------------------------------------
  // Initialize flat water surface
  // -------------------------------------------------
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      h[i][j] = 0.5;
      v[i][j] = 0.0;
    }
  }

  // -------------------------------------------------
  // Initial velocity impulse (drop-style disturbance)
  // -------------------------------------------------
  for (int i = 0; i < N; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      double x = double(i) / (N - 1);
      double y = double(j) / (N - 1);

      double dx0 = x - disturb_x;
      double dy0 = y - disturb_y;
      double r2 = dx0 * dx0 + dy0 * dy0;

      // Downward impulse
      v[i][j] = -disturb_amp *
                std::exp(-r2 / (2.0 * disturb_sigma * disturb_sigma));
    }
  }

  // -------------------------------------------------
  // Main simulation loop
  // -------------------------------------------------
  for (int frame = 0; frame < NUM_FRAMES; ++frame)
  {
    // Interior update
    for (int i = 1; i < N - 1; ++i)
    {
      for (int j = 1; j < N - 1; ++j)
      {
        double lap =
            (h[i + 1][j] + h[i - 1][j] +
             h[i][j + 1] + h[i][j - 1] -
             4.0 * h[i][j]) *
            inv_dx2;

        // Velocity update with RESTORING FORCE (gravity)
        const double h0 = 0.5; // equilibrium surface height
        const double g = 20.0; // restoring strength (tune 10–50)

        // acceleration = curvature + gravity - damping
        v[i][j] += (wave_speed * wave_speed * lap - g * (h[i][j] - h0) - damping * v[i][j]) * dt;

        // Height update
        h_new[i][j] = h[i][j] + v[i][j] * dt;
      }
    }

    // Reflecting boundary conditions
    for (int i = 0; i < N; ++i)
    {
      h_new[i][0] = h_new[i][1];
      h_new[i][N - 1] = h_new[i][N - 2];
      h_new[0][i] = h_new[1][i];
      h_new[N - 1][i] = h_new[N - 2][i];

      v[i][0] = v[i][1];
      v[i][N - 1] = v[i][N - 2];
      v[0][i] = v[1][i];
      v[N - 1][i] = v[N - 2][i];
    }

    // Advance time
    h.swap(h_new);

    // ----------------------------
    // DEBUG: min/max
    // ----------------------------
    double mn = h[0][0], mx = h[0][0];
    for (int a = 0; a < N; ++a)
      for (int b = 0; b < N; ++b)
      {
        mn = std::min(mn, h[a][b]);
        mx = std::max(mx, h[a][b]);
      }

    std::cout << std::fixed << std::setprecision(6)
              << "frame " << frame
              << " min=" << mn
              << " max=" << mx << "\n";

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
  }

  return 0;
}
