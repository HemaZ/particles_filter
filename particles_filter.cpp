#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Robot.hpp"
#include <iostream>
#include <random>


void visParticles(std::vector<Robot> particles){
    cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);
    std::vector<std::vector<double>> landmarks = particles[0].getLandMarks();   
    for (size_t i = 0; i < particles.size(); i++)
    {
        cv::circle(img, cv::Point(particles[i].getX()*5, particles[i].getY()*5), 2, cv::Scalar(0, 0, 255));
    }
    for (size_t i = 0; i < landmarks.size(); i++)
    {
        cv::circle(img, cv::Point(landmarks[i][0]*5, landmarks[i][1]*5), 5, cv::Scalar(0,255,0), 10);
    }
    cv::namedWindow("Particles");
    cv::imshow("Particles", img);
    cv::waitKey(0);

    
}





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
    int N = 1000;
    std::vector<Robot> p;
    for(int i=0; i<N; i++){
        p.push_back(Robot(100, landmarks));
    }
    std::vector<Robot> p2;
    p2.assign(p.begin(), p.end());
    for (size_t i = 0; i < N; i++)
    {
        p2[i].move(0.1, 5);
    }
    visParticles(p2);
    return 0;
}