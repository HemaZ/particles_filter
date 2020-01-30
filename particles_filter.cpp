#include "Robot.hpp"
#include <iostream>
#include <random>
int main(){
    srand(time(0));
    //landmarks  = [[20.0, 20.0], [80.0, 80.0], [20.0, 80.0], [80.0, 20.0]]
    std::vector<std::vector<double>> landmarks;
    landmarks.push_back(std::vector<double>{20.0, 20.0});
    landmarks.push_back(std::vector<double>{80.0, 80.0});
    landmarks.push_back(std::vector<double>{20.0, 80.0});
    landmarks.push_back(std::vector<double>{80.0, 20.0});
    Robot mybot(100, landmarks);
    mybot.set_noise(5, 0.1, 5);
    mybot.set_states(30, 50, M_PI_2);
    mybot.move(-M_PI_2, 15);
    mybot.move(-M_PI_2, 10);
    std::vector<float> measurments = mybot.sense();

    return 0;
}