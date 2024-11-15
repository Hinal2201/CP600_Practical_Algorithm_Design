#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>

constexpr double COLLISION_DISTANCE = 5.0; // Safe distance in meters

struct Segment {
    int id;
    double length;
    double speed_limit;

    Segment(int id, double length, double speed_limit)
        : id(id), length(length), speed_limit(speed_limit) {}

    virtual ~Segment() = default; // Virtual destructor to make Segment polymorphic
};

struct Intersection : public Segment {
    bool pedestrian_signal_on;

    Intersection(int id, double length, double speed_limit, bool pedestrian_signal_on)
        : Segment(id, length, speed_limit), pedestrian_signal_on(pedestrian_signal_on) {}
};

struct Vehicle {
    int id;
    int route_index;
    double current_position;
    double velocity;
    double max_acceleration;
    double max_deceleration;
    std::vector<int> route;

    Vehicle(int id, double pos, double vel, double max_acc, double max_dec, const std::vector<int>& route)
        : id(id), route_index(0), current_position(pos), velocity(vel),
          max_acceleration(max_acc), max_deceleration(max_dec), route(route) {}
};

void determineSpeedForSegment(Vehicle &vehicle, const std::shared_ptr<Segment> &segment, double distance_to_intersection) {
    auto intersection = std::dynamic_pointer_cast<Intersection>(segment);

    if (intersection && intersection->pedestrian_signal_on && distance_to_intersection < 10) {
        double deceleration_factor = (10 - distance_to_intersection) / 10;
        double target_velocity = deceleration_factor * vehicle.max_deceleration;
        vehicle.velocity = std::max(0.0, vehicle.velocity - target_velocity);
    } else {
        vehicle.velocity = std::min(vehicle.velocity + vehicle.max_acceleration, segment->speed_limit);
    }
}

void checkCollisions(std::vector<Vehicle> &vehicles) {
    for (size_t i = 0; i < vehicles.size(); ++i) {
        for (size_t j = i + 1; j < vehicles.size(); ++j) {
            if (vehicles[i].route[vehicles[i].route_index] == vehicles[j].route[vehicles[j].route_index]) {
                double distance = std::abs(vehicles[i].current_position - vehicles[j].current_position);
                if (distance < COLLISION_DISTANCE) {
                    if (vehicles[i].current_position < vehicles[j].current_position) {
                        vehicles[i].velocity = std::max(0.0, vehicles[i].velocity - vehicles[i].max_deceleration);
                    } else {
                        vehicles[j].velocity = std::max(0.0, vehicles[j].velocity - vehicles[j].max_deceleration);
                    }
                }
            }
        }
    }
}

int main() {
    int num_segments;
    std::cout << "Enter number of segments: ";
    std::cin >> num_segments;

    std::vector<std::shared_ptr<Segment>> road;

    for (int i = 0; i < num_segments; ++i) {
        int id;
        double length, speed_limit;
        char is_intersection;
        std::cout << "Enter details for segment " << i + 1 << " (id, length, speed_limit, is_intersection (y/n)): ";
        std::cin >> id >> length >> speed_limit >> is_intersection;

        if (is_intersection == 'y') {
            char pedestrian_signal;
            std::cout << "Is the pedestrian signal on? (y/n): ";
            std::cin >> pedestrian_signal;
            bool signal_on = (pedestrian_signal == 'y');
            road.push_back(std::make_shared<Intersection>(id, length, speed_limit, signal_on));
        } else {
            road.push_back(std::make_shared<Segment>(id, length, speed_limit));
        }
    }

    int num_vehicles;
    std::cout << "Enter number of vehicles: ";
    std::cin >> num_vehicles;

    std::vector<Vehicle> vehicles;

    for (int i = 0; i < num_vehicles; ++i) {
        int id;
        double pos, vel, max_acc, max_dec;
        int route_length;
        std::vector<int> route;

        std::cout << "Enter details for vehicle " << i + 1 << " (id, initial_position, initial_velocity, max_acceleration, max_deceleration): ";
        std::cin >> id >> pos >> vel >> max_acc >> max_dec;

        std::cout << "Enter the number of segments in the route: ";
        std::cin >> route_length;
        route.resize(route_length);

        std::cout << "Enter the segment IDs in the route: ";
        for (int j = 0; j < route_length; ++j) {
            std::cin >> route[j];
        }

        vehicles.emplace_back(id, pos, vel, max_acc, max_dec, route);
    }

    for (int t = 0; t < 10; ++t) {
        std::cout << "Time: " << t << "s\n";

        checkCollisions(vehicles);

        for (auto &vehicle : vehicles) {
            int current_segment_id = vehicle.route[vehicle.route_index];
            auto segment = road[current_segment_id - 1];
            double distance_to_intersection = (segment->id == 2) ? 8 : 0;

            determineSpeedForSegment(vehicle, segment, distance_to_intersection);

            vehicle.current_position += vehicle.velocity;
            std::cout << "Vehicle " << vehicle.id << " - Position: " << vehicle.current_position
                      << " m, Velocity: " << vehicle.velocity << " m/s\n";

            if (vehicle.current_position >= segment->length) {
                vehicle.route_index++;
                vehicle.current_position = 0.0;
            }
        }
        std::cout << "\n";
    }

    std::cout << "Simulation complete.\n";
    return 0;
}
