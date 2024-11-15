#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

struct Vehicle {
    int id;
    double position;   // Position of the vehicle 
    double velocity;   // Current velocity 
    double eta;        // Estimated Time of Arrival at the intersection (s)
    double safe_velocity; // Calculated safe velocity 
};

double calculateSafeVelocity(double eta_diff, double safe_distance) {
    if (eta_diff <= 0) return 0; 
    //std::cout << "distance " << safe_distance << eta_diff <<"\n";
    return safe_distance / eta_diff;
}

int main() {
    //safe distance
    const double safe_distance = 10.0; //meters

    Vehicle vehicle1 = {1, 100.0, 20.0, 5.0, 20.0}; 
    Vehicle vehicle2 = {2, 120.0, 25.0, 7.0, 25.0}; 
    
    bool intersection=1;

    std::vector<Vehicle> vehicles = {vehicle1, vehicle2};
    std::sort(vehicles.begin(), vehicles.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.eta < b.eta;
    });

    // Calculate safe velocities 
    for (size_t i = 0; i < vehicles.size(); ++i) {
        if (i > 0) {
            double eta_diff = vehicles[i].eta - vehicles[i - 1].eta;
            double safe_velocity = calculateSafeVelocity(eta_diff, safe_distance);

            if (vehicles[i].velocity > safe_velocity) {
                vehicles[i].safe_velocity = safe_velocity;
            }
        }
        std::cout << "Vehicle " << vehicles[i].id << " - Safe Velocity: " << vehicles[i].safe_velocity << " m/s\n";
    }


    if(intersection)
    {

        
    }

    return 0;
}
