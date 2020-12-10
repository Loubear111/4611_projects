/** CSci-4611 Assignment 2:  Car Soccer
 */

#ifndef BALL_H_
#define BALL_H_

#include <mingfx.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

/// Small data structure for a ball
class Ball {
public:
    
    /// The constructor sets the radius and calls Reset() to start the ball at
    /// the center of the field
    Ball() : radius_(2.6), friction_(0.8) {
        srand(time(NULL));
        max_velocity_ = 40;
        Reset();
    }
    
    /// Nothing special needed in the constructor
    virtual ~Ball() {}

    
    void Reset() {
        // Generate some random floats for the X and Z velocities
        //float randomX = (float)(rand() % 10 + 1) / (float)(rand() % 100 + 50) - (float)(rand() % 1);
        //float randomZ = (float)(rand() % 10 + 1) / (float)(rand() % 100 + 50) - (float)(rand() % 1);

        float randomX = (float)(rand() % 20 + 1) / (float)(rand() % 10 + 1);
        float randomZ = (float)(rand() % 20 + 1) / (float)(rand() % 10 + 1);

        //std::cout << "X: " << randomX << std::endl;
        //std::cout << "Z: " << randomX << std::endl;

        // Make sure we have a total velocity that isn't too slow
        while (sqrt(randomX * randomX + randomZ * randomZ) < 4)
        {
            randomX += 0.1;
            randomZ += 0.1;
        }

        std::cout << "X: " << randomX << std::endl;
        std::cout << "Z: " << randomX << std::endl;

        if (rand() % 1)
        {
            randomX = randomX * -1;
        }

        if (rand() % 1)
        {
            randomZ = randomZ * -1;
        }

        position_ = Point3(0, radius_ + 20, 0);
        velocity_ = Vector3(randomX, 0, randomZ);
    }

    float radius() { return radius_; }
    
    Point3 position() { return position_; }
    void set_position(const Point3 &p) { position_ = p; }

    Vector3 velocity() { return velocity_; }
    void set_velocity(Vector3 new_velocity) 
    { 
        float velX, velY, velZ;

        // Threshold ball X velocity
        if (new_velocity[0] > max_velocity_)
        {
            velX = max_velocity_;
        }
        else if (new_velocity[0] < -max_velocity_)
        {
            velX = -max_velocity_;
        }
        else
        {
            velX = new_velocity[0];
        }

        // Threshold ball Y velocity
        if (new_velocity[1] > max_velocity_)
        {
            velY = max_velocity_;
        }
        else if (new_velocity[1] < -max_velocity_)
        {
            velY = -max_velocity_;
        }
        else
        {
            velY = new_velocity[1];
        }

        velY = new_velocity[1];

        // Threshold ball X velocity
        if (new_velocity[2] > max_velocity_)
        {
            velZ = max_velocity_;
        }
        else if (new_velocity[2] < -max_velocity_)
        {
            velZ = -max_velocity_;
        }
        else
        {
            velZ = new_velocity[2];
        }

        velocity_ = Vector3(velX, velY, velZ); 
    }

    float friction() { return friction_; }
    void set_friction(float new_friction) { friction_ = new_friction; }
    
private:
    // You will probably need to store some additional data here, e.g., velocity
    
    Point3 position_;
    float radius_;
    Vector3 velocity_;
    float friction_;
    float max_velocity_;
};

#endif
