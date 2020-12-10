/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}


void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
        ball_.Reset();
    }
}


void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the assignment.
    
    //timeStep = timeStep;

    UpdateBall(timeStep);
    UpdateCar(timeStep);

    // Check for ball and car collisions with each other!
    Vector3 vToBall = car_.position() - ball_.position();

    if (vToBall.Length() <= car_.collision_radius() + ball_.radius())
    {
        // Ball and car are colliding!
        //ball_.set_velocity(ball_.velocity() + car_.velocity());
        ball_.set_velocity((ball_.velocity() - 2*(ball_.velocity().Dot(Vector3::Normalize(vToBall))* Vector3::Normalize(vToBall))) + 100*car_.velocity());
        car_.set_was_ball_collision(true);

        while (vToBall.Length() <= car_.collision_radius() + ball_.radius() + epsilon)
        {
            ball_.set_position(ball_.position() + Vector3::Normalize(ball_.velocity()));
            vToBall = car_.position() - ball_.position();
        }
    }
}

void CarSoccer::UpdateBall(double timeStep)
{
    // Move the ball before checking for collisions
    Point3 newBallPos = ball_.position() + ball_.velocity() * timeStep;
    ball_.set_position(newBallPos);

    // X: -12, 12
    // Y: 0, 14
    // Z: -48 or 49.2
    // Calculate if the ball is in one of the goals (using above coordinates that represent the square that is the goal)
    if (ball_.position()[0] - ball_.radius() >= -13 && ball_.position()[0] + ball_.radius() <= 13 && ball_.position()[1] - ball_.radius() >= 0 && ball_.position()[1] + ball_.radius() <= 14)
    {
        if (ball_.position()[2] - ball_.radius() <= -49)
        {
            std::cout << "Blue scored!" << std::endl;
            ball_.Reset();
        }

        if (ball_.position()[2] + ball_.radius() >= 49)
        {
            std::cout << "Red scored!" << std::endl;
            ball_.Reset();
        }
    }

    // Check for ball collision with left and right walls
    if (ball_.position()[0] - ball_.radius() <= -40 || ball_.position()[0] + ball_.radius() >= 40)
    {
        ball_.set_velocity(Vector3(-ball_.velocity()[0] * ball_.friction(), ball_.velocity()[1], ball_.velocity()[2]));

        // Adjust position so the ball isn't "in" the wall
        if (ball_.position()[0] > 0)
        {
            ball_.set_position(Point3(40 - ball_.radius() - epsilon, ball_.position()[1], ball_.position()[2]));
        }
        else
        {
            ball_.set_position(Point3(-40 + ball_.radius() + epsilon, ball_.position()[1], ball_.position()[2]));
        }
    }

    // Check for ball collisions with ground and ceiling
    if (ball_.position()[1] - ball_.radius() <= 0 || ball_.position()[1] + ball_.radius() >= 30)
    {
        ball_.set_velocity(Vector3(ball_.velocity()[0], -ball_.velocity()[1] * ball_.friction(), ball_.velocity()[2]));

        float y_val = ball_.position()[1];

        // Move the ball accordingly so it isn't "in" the ground
        if (ball_.position()[1] - ball_.radius() <= 0)
        {
            ball_.set_position(Point3(ball_.position()[0], ball_.radius() + epsilon, ball_.position()[2]));
        }
        else
        {
            ball_.set_position(Point3(ball_.position()[0], 30 - ball_.radius() - epsilon, ball_.position()[2]));
        }
    }
    else
    {
        ball_.set_velocity(Vector3(ball_.velocity()[0], ball_.velocity()[1] + gravity * timeStep, ball_.velocity()[2]));
    }

    // Check for ball collisions with front and rear walls
    if (ball_.position()[2] - ball_.radius() <= -49.2 || ball_.position()[2] + ball_.radius() >= 49.2)
    {
        ball_.set_velocity(Vector3(ball_.velocity()[0], ball_.velocity()[1], -ball_.velocity()[2] * ball_.friction()));

        // Adjust position so the ball isn't "in" the wall
        if (ball_.position()[2] > 0)
        {
            ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], 49.2 - ball_.radius() - epsilon));
        }
        else
        {
            ball_.set_position(Point3(ball_.position()[0], ball_.position()[1], -49.2 + ball_.radius() + epsilon));
        }
    }
}

void CarSoccer::UpdateCar(double timeStep)
{
    // Move the car before checking for collisions
    Point3 oldCarPos = car_.position();
    Vector2 car_control = joystick_direction();
    car_.set_angle(car_.angle() - (car_control[0] * 10 * timeStep));

    car_.set_velocity(Vector3(-sin(car_.angle()) * (car_control[1] * 40), car_.velocity()[1], -cos(car_.angle()) * (car_control[1] * 40)));
    car_.set_position(car_.position() + car_.velocity() * timeStep);

    // Check for car collision with left and right wall
    if (car_.position()[0] - car_.collision_radius() <= -40 || car_.position()[0] + car_.collision_radius() >= 40)
    {
        car_.set_position(oldCarPos);
        car_.set_velocity(Vector3(0, car_.velocity()[1], -cos(car_.angle()) * (car_control[1] / 10)));
        car_.set_position(car_.position() + car_.velocity());

        // Check for car collisions with front and rear walls again just in case we're in a corner
        if (car_.position()[2] - car_.collision_radius() <= -49.2 || car_.position()[2] + car_.collision_radius() >= 49.2)
        {
            car_.set_position(oldCarPos);
            car_.set_velocity(Vector3(0, car_.velocity()[1], 0));
            car_.set_position(car_.position() + car_.velocity());
        }
    }

    // Check for car collisions with front and rear walls
    if (car_.position()[2] - car_.collision_radius() <= -49.2 || car_.position()[2] + car_.collision_radius() >= 49.2)
    {
        car_.set_position(oldCarPos);
        car_.set_velocity(Vector3(-sin(car_.angle()) * (car_control[1] / 10), car_.velocity()[1], 0));
        car_.set_position(car_.position() + car_.velocity());

        // Check for car collision with left and right wall again just in case we're in a corner
        if (car_.position()[0] - car_.collision_radius() <= -40 || car_.position()[0] + car_.collision_radius() >= 40)
        {
            car_.set_position(oldCarPos);
            car_.set_velocity(Vector3(0, car_.velocity()[1], 0));
            car_.set_position(car_.position() + car_.velocity());
        }
    }
}

void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    viewMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);
    
    // Draw the field with the field texture on it.
    Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::RotationY(car_.angle()) *
        Matrix4::Scale(car_.size()) *
        Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);
    
    
    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);
    
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);
    
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()

    /* FIRST NET */
    Matrix4 m_netOne;
    std::vector<Point3> netOne;

    // Horizontal bars of the net
    for (int i = 1; i < 14; i+=2)
    {
        netOne.push_back(Point3(-12, i, 49.2));
        netOne.push_back(Point3(12, i, 49.2));
    }

    // Vertical bars of the net
    for (int i = -12; i < 14; i += 2)
    {
        netOne.push_back(Point3(i, 1, 49.2));
        netOne.push_back(Point3(i, 13, 49.2));
    }
    quickShapes_.DrawLines(m_netOne, viewMatrix_, projMatrix_, Color(0,0,1), netOne, QuickShapes::LinesType::LINES, 0.1);


    /* SECOND NET */
    Matrix4 m_netTwo;
    std::vector<Point3> netTwo;

    // Horizontal bars of the net
    for (int i = 1; i < 14; i += 2)
    {
        netTwo.push_back(Point3(-12, i, -48));
        netTwo.push_back(Point3(12, i, -48));
    }

    // Vertical bars of the net
    for (int i = -12; i < 14; i += 2)
    {
        netTwo.push_back(Point3(i, 1, -48));
        netTwo.push_back(Point3(i, 13, -48));
    }
    quickShapes_.DrawLines(m_netTwo, viewMatrix_, projMatrix_, Color(1, 0, 0), netTwo, QuickShapes::LinesType::LINES, 0.1);

    /* PITCH BOUNDARIES */
    Matrix4 m_pitch;
    std::vector<Point3> pitch;
    // Lower box
    pitch.push_back(Point3(-40, 0, -50));
    pitch.push_back(Point3(40, 0, -50));

    pitch.push_back(Point3(40, 0, -50));
    pitch.push_back(Point3(40, 0, 50));

    pitch.push_back(Point3(40, 0, 50));
    pitch.push_back(Point3(-40, 0, 50));

    pitch.push_back(Point3(-40, 0, 50));
    pitch.push_back(Point3(-40, 0, -50));

    // Upper box
    pitch.push_back(Point3(-40, 35, -50));
    pitch.push_back(Point3(40, 35, -50));

    pitch.push_back(Point3(40, 35, -50));
    pitch.push_back(Point3(40, 35, 50));

    pitch.push_back(Point3(40, 35, 50));
    pitch.push_back(Point3(-40, 35, 50));

    pitch.push_back(Point3(-40, 35, 50));
    pitch.push_back(Point3(-40, 35, -50));

    // Lines connecting upper and lower boxes
    pitch.push_back(Point3(-40, 0, -50));
    pitch.push_back(Point3(-40, 35, -50));

    pitch.push_back(Point3(40, 0, -50));
    pitch.push_back(Point3(40, 35, -50));

    pitch.push_back(Point3(-40, 0, 50));
    pitch.push_back(Point3(-40, 35, 50));

    pitch.push_back(Point3(40, 0, 50));
    pitch.push_back(Point3(40, 35, 50));

    quickShapes_.DrawLines(m_pitch, viewMatrix_, projMatrix_, Color(0, 1, 0), pitch, QuickShapes::LinesType::LINES, 0.1);
}
