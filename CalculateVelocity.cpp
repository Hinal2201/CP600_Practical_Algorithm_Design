#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

struct Vehicle {
    int id;
    double position;   // Position of the vehicle along the directed path
    double velocity;   // Current velocity of the vehicle (m/s)
    double eta;        // Estimated Time of Arrival at the intersection (s)
    double safe_velocity; // Calculated safe velocity (m/s)
};

// Function to calculate the safe velocity for a vehicle based on the ETA difference and a safe distance
double calculateSafeVelocity(double eta_diff, double safe_distance) {
    if (eta_diff <= 0) return 0; // Avoid division by zero if ETA difference is zero
    return safe_distance / eta_diff;
}

int main() {
    // Define a safe distance to be maintained between vehicles at the intersection
    const double safe_distance = 10.0; // Safe distance in meters

    // Define two vehicles approaching the intersection
    Vehicle vehicle1 = {1, 100.0, 20.0, 5.0, 20.0}; // Position, velocity, ETA
    Vehicle vehicle2 = {2, 120.0, 25.0, 7.0, 25.0}; // Position, velocity, ETA

    // Store vehicles in a vector and sort by ETA
    std::vector<Vehicle> vehicles = {vehicle1, vehicle2};
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.eta < b.eta;
    });

    // Calculate safe velocities for each vehicle
    for (size_t i = 0; i < vehicles.size(); ++i) {
        if (i > 0) {
            // Calculate the ETA difference with the previous vehicle
            double eta_diff = vehicles[i].eta - vehicles[i - 1].eta;
            double safe_velocity = calculateSafeVelocity(eta_diff, safe_distance);

            // Adjust the vehicle's velocity if needed
            if (vehicles[i].velocity > safe_velocity) {
                vehicles[i].safe_velocity = safe_velocity;
            }
        }
        // Print the updated velocity for the vehicle
        std::cout << "Vehicle " << vehicles[i].id << " - Safe Velocity: " << vehicles[i].safe_velocity << " m/s\n";
    }

    return 0;
}
