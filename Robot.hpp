#ifndef ROBOT_CLASS
#define ROBOT_CLASS
#include <vector>

class Robot{
    private:
    float x_, y_, orientation_;
    double forward_noise_, turn_noise_, sense_noise_;
    double world_size_;
    std::vector<std::vector<double>> landmarks_;
    public:
    Robot(double, std::vector<std::vector<double>>);
    void set_states(float new_x, float new_y, float new_orientation);
    void set_noise(double new_f_noise, double new_t_noise, double new_s_noise);
    std::vector<float> sense();
    void move(float turn, float forward);
    float gaussian(double miu, double sigma, float x);
    double measurement_prob(std::vector<float> measurement); 
    void print();
    float getX();
    float getY();
    std::vector<std::vector<double>> getLandMarks();
};



#endif