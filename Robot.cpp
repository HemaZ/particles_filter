#include "Robot.hpp"
#include <random>
#include <iostream>
Robot::Robot(double w_size, std::vector<std::vector<double>> landmarks){
    world_size_ = w_size;
    landmarks_ = landmarks;
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    x_ = r * world_size_;
    r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    y_ = r * world_size_;
    r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    orientation_ = r * 2 * M_PI;
    forward_noise_ = 0;
    turn_noise_ = 0;
    sense_noise_ = 0;
}

void Robot::print(){
    std::cout << "My current position is: X=" << x_ << " Y=" << y_ << " Orientation=" <<orientation_ <<std::endl;
}

void Robot::set_states(float new_x, float new_y, float new_orientation){
    if (new_x < 0 or new_x >= world_size_)
        throw "X coordinate out of bound";
    if (new_y < 0 or new_y >= world_size_)
            throw "Y coordinate out of bound";
    if (new_orientation < 0 or new_orientation >= 2 * M_PI)
        throw "Orientation must be in [0..2pi]";
    x_ = new_x;
    y_ = new_y;
    orientation_ = orientation_;
    std::cout << "=============== States Updated ===============" <<std::endl;
    Robot::print();
}

void Robot::set_noise(double new_f_noise, double new_t_noise, double new_s_noise){
    forward_noise_ = new_f_noise;
    turn_noise_ = new_t_noise;
    sense_noise_ = new_s_noise;
}


std::vector<float> Robot::sense(){
    std::vector<float> Z;
    float dist;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0, sense_noise_);
    for(int i=0; i<landmarks_.size(); i++){
        float x, y;
        x = landmarks_[i][0];
        y = landmarks_[i][1];
        dist = sqrtf((x-x_)*(x-x_)+(y-y_)*(y-y_));
        dist += distribution(generator);
        Z.push_back(dist);

    }
    std::cout << "=============== Sensors Readings ===============" <<std::endl;
    Robot::print();
    return Z;
}

void Robot::move(float turn, float forward){
    if (forward < 0)
            throw "Robot cant move backwards";  
    std::random_device dev;
    std::mt19937 rng(dev());
    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0, turn_noise_);
    orientation_ +=  turn + distribution(generator);
    while (orientation_ >2*M_PI)
    {
        orientation_ -= 2*M_PI;
    }
    std::normal_distribution<float> distributionf(0, forward_noise_);
    forward += distributionf(generator);
    x_ += cosf(orientation_) * forward;
    y_ += sinf(orientation_) * forward;
    while (x_ >world_size_)
    {
        x_ -= world_size_;
    }
    while (y_ >world_size_)
    {
        y_ -= world_size_;
    }
    std::cout << "=============== Moved ===============" <<std::endl;
    std::cout << "Forward: " <<forward <<" Turn: " << turn <<std::endl;;
    Robot::print();
    
}

float Robot::gaussian(double mu, double sigma, float x){
    return exp(- ((mu - x) *(mu - x)) / (sigma *sigma) / 2.0) / sqrt(2.0 * M_PI * (sigma *sigma));
}

double Robot::measurement_prob(std::vector<float> measurement){
    double prob = 1;
    float dist;
    for(int i=0; i<landmarks_.size(); i++){
        float x, y;
        x = landmarks_[i][0];
        y = landmarks_[i][1];
        dist = sqrtf((x-x_)*(x-x_)+(y-y_)*(y-y_));
        prob *= Robot::gaussian(dist, sense_noise_, measurement[i]);
    }
    return prob;
}

float Robot::getX(){return x_;}
float Robot::getY(){return y_;}
std::vector<std::vector<double>> Robot::getLandMarks(){return landmarks_;}