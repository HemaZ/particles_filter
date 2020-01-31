#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Robot.hpp"
#include <iostream>
#include <random>


void visParticles(Robot myBot, std::vector<Robot> particles){
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
    cv::circle(img, cv::Point(myBot.getX()*5, myBot.getY()*5), 5, cv::Scalar(255,0,0), 10);
    cv::namedWindow("Particles");
    cv::imshow("Particles", img);
    cv::waitKey(0);

    
}





int main(){
    srand(time(0));
    // Create landmarks  = [[20.0, 20.0], [80.0, 80.0], [20.0, 80.0], [80.0, 20.0]]
    std::vector<std::vector<double>> landmarks;
    landmarks.push_back(std::vector<double>{20.0, 20.0});
    landmarks.push_back(std::vector<double>{80.0, 80.0});
    landmarks.push_back(std::vector<double>{20.0, 80.0});
    landmarks.push_back(std::vector<double>{80.0, 20.0});
    // Create Our Robot
    Robot mybot(100, landmarks);
    std::vector<float> measurements;
    // Define number of particles and set noises
    int N = 1000;
    std::vector<Robot> p;
    for(int i=0; i<N; i++){
        Robot particl(100, landmarks);
        particl.set_noise(0.05, 0.05, 5);
        p.push_back(particl);
    }
    // define number of time steps.
    int T = 10;

    for (size_t ti = 0; ti < T; ti++)
    {
        mybot.move(0.1, 5.0);
        measurements = mybot.sense();
        std::vector<Robot> p2;
        p2.assign(p.begin(), p.end());
        for (size_t i = 0; i < N; i++)
        {
            p2[i].move(0.1, 5);
        }
        visParticles(mybot, p2);
        
        std::vector<double> w;
        for (size_t i = 0; i < N; i++)
        {
            w.push_back(p2[i].measurement_prob(measurements));
        }
        
        std::vector<Robot> p3;
        int index = rand() %N;
        double mw = *std::max_element(w.begin(), w.end());
        double beta = 0;
        for (size_t i = 0; i < N; i++)
        {
            beta += ((double) rand() / (RAND_MAX)) * 2 * mw;
            while(beta>w[index]){
                beta -= w[index];
                index = (index+1)%N;
            }
            p3.push_back(p2[index]);
        }
        // visualize the particles and the robot.
        visParticles(mybot, p3);
        p = p3;
    }
    

   
    

    return 0;
}