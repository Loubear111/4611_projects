/** CSci-4611 Assignment 3:  Earthquake
 */

#include "quake_app.h"
#include "config.h"

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <sstream>

// Number of seconds in 1 year (approx.)
const int PLAYBACK_WINDOW = 12 * 28 * 24 * 60 * 60;

using namespace std;

QuakeApp::QuakeApp() : GraphicsApp(1280,720, "Earthquake"),
    playback_scale_(15000000.0), debug_mode_(false), globe_mode_(false), scale_(1.0)
{
    // Define a search path for finding data files (images and earthquake db)
    search_path_.push_back(".");
    search_path_.push_back("./data");
    search_path_.push_back(DATA_DIR_INSTALL);
    search_path_.push_back(DATA_DIR_BUILD);
    
    quake_db_ = EarthquakeDatabase(Platform::FindFile("earthquakes.txt", search_path_));
    current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();

 }


QuakeApp::~QuakeApp() {
}


void QuakeApp::InitNanoGUI() {
    // Setup the GUI window
    nanogui::Window *window = new nanogui::Window(screen(), "Earthquake Controls");
    window->setPosition(Eigen::Vector2i(10, 10));
    window->setSize(Eigen::Vector2i(400,200));
    window->setLayout(new nanogui::GroupLayout());
    
    date_label_ = new nanogui::Label(window, "Current Date: MM/DD/YYYY", "sans-bold");
    
    globe_btn_ = new nanogui::Button(window, "Globe");
    globe_btn_->setCallback(std::bind(&QuakeApp::OnGlobeBtnPressed, this));
    globe_btn_->setTooltip("Toggle between map and globe.");
    
    new nanogui::Label(window, "Playback Speed", "sans-bold");
    
    nanogui::Widget *panel = new nanogui::Widget(window);
    panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal,
                                            nanogui::Alignment::Middle, 0, 20));
    
    nanogui::Slider *slider = new nanogui::Slider(panel);
    slider->setValue(0.5f);
    slider->setFixedWidth(120);
    
    speed_box_ = new nanogui::TextBox(panel);
    speed_box_->setFixedSize(Eigen::Vector2i(60, 25));
    speed_box_->setValue("50");
    speed_box_->setUnits("%");
    slider->setCallback(std::bind(&QuakeApp::OnSliderUpdate, this, std::placeholders::_1));
    speed_box_->setFixedSize(Eigen::Vector2i(60,25));
    speed_box_->setFontSize(20);
    speed_box_->setAlignment(nanogui::TextBox::Alignment::Right);
    
    nanogui::Button* debug_btn = new nanogui::Button(window, "Toggle Debug Mode");
    debug_btn->setCallback(std::bind(&QuakeApp::OnDebugBtnPressed, this));
    debug_btn->setTooltip("Toggle displaying mesh triangles and normals (can be slow)");
    
    screen()->performLayout();
}

void QuakeApp::OnLeftMouseDrag(const Point2 &pos, const Vector2 &delta) {
    // Optional: In our demo, we adjust the tilt of the globe here when the
    // mouse is dragged up/down on the screen.
}


void QuakeApp::OnGlobeBtnPressed() {
    // TODO: This is where you can switch between flat earth mode and globe mode
    globe_mode_ = !globe_mode_;
    scale_ = 0.0;
}

void QuakeApp::OnDebugBtnPressed() {
    debug_mode_ = !debug_mode_;
}

void QuakeApp::OnSliderUpdate(float value) {
    speed_box_->setValue(std::to_string((int) (value * 100)));
    playback_scale_ = 30000000.0*value;
}


void QuakeApp::UpdateSimulation(double dt)  {
    // Advance the current time and loop back to the start if time is past the last earthquake
    current_time_ += playback_scale_ * dt;
    if (current_time_ > quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds();
    }
    if (current_time_ < quake_db_.earthquake(quake_db_.min_index()).date().ToSeconds()) {
        current_time_ = quake_db_.earthquake(quake_db_.max_index()).date().ToSeconds();
    }
    
    Date d(current_time_);
    stringstream s;
    s << "Current date: " << d.month()
        << "/" << d.day()
        << "/" << d.year();
    date_label_->setCaption(s.str());
    
    // TODO: Any animation, morphing, rotation of the earth, or other things that should
    // be updated once each frame would go here.
    for (int i = 0; i < quake_spheres.size(); i++)
    {
        float time = (float)d.year();
        time += (float)d.month() / 25.0;
        time += (float)d.day() / 64.0;

        float quake_time = (float)quake_spheres[i].date().year();
        quake_time += (float)quake_spheres[i].date().month() / 25.0;
        quake_time += (float)quake_spheres[i].date().day() / 64.0;

        //std::cout << "t:   " << quake_spheres[i].date().SecondsUntil(d) << std::endl;
        if (quake_time < time - 0.5) //1 month
        {
            //quake_spheres.erase(quake_spheres.begin() + i);
        }
    }

    static int lastQuakeIndex = 0;
    int currentQuakeIndex = quake_db_.FindMostRecentQuake(d);

    if (lastQuakeIndex != currentQuakeIndex)
    {
        Earthquake e = quake_db_.earthquake(quake_db_.FindMostRecentQuake(d));
        quake_spheres.push_back(e);

        if (quake_spheres.size() > 15)
        {
            quake_spheres.erase(quake_spheres.begin());
        }
    }

    lastQuakeIndex = currentQuakeIndex;

    if (globe_mode_ && scale_ < 1.0)
    {
        earth_.GoRound(scale_);
        scale_ += 0.01;
    }
    else
    {
        if (globe_mode_)
        {
            earth_.Spin();
        }
    }
    if (!globe_mode_ && scale_ < 1.0)
    {
        earth_.GoFlat(scale_);
        scale_ += 0.01;
    }
}


void QuakeApp::InitOpenGL() {
    // Set up the camera in a good position to see the entire earth in either mode
    proj_matrix_ = Matrix4::Perspective(60, aspect_ratio(), 0.1, 50);
    view_matrix_ = Matrix4::LookAt(Point3(0,0,3.5), Point3(0,0,0), Vector3(0,1,0));
    glClearColor(0.0, 0.0, 0.0, 1);
    
    // Initialize the earth object
    earth_.Init(search_path_);

    // Initialize the texture used for the background image
    stars_tex_.InitFromFile(Platform::FindFile("iss006e40544.png", search_path_));
}


void QuakeApp::DrawUsingOpenGL() {
    quick_shapes_.DrawFullscreenTexture(Color(1,1,1), stars_tex_);
    
    // You can leave this as the identity matrix and we will have a fine view of
    // the earth.  If you want to add any rotation or other animation of the
    // earth, the model_matrix is where you would apply that.
    Matrix4 model_matrix;
    
    // Draw the earth
    earth_.Draw(model_matrix, view_matrix_, proj_matrix_);
    if (debug_mode_) {
        earth_.DrawDebugInfo(model_matrix, view_matrix_, proj_matrix_);
    }

    // TODO: You'll also need to draw the earthquakes.  It's up to you exactly
    // how you wish to do that.
    for (int i = 0; i < quake_spheres.size(); i++)
    {
        //Matrix4 sphere_model = Matrix4::Translation(Vector3(-M_PI, M_PI / 2, -.025)) * Matrix4::Scale(Vector3(0.05, 0.05, 0.05));
        Matrix4 sphere_model = Matrix4::Scale(Vector3(0.05*((float)i / (float)quake_spheres.size()), 0.05 * ((float)i / (float)quake_spheres.size()), 0.05 * ((float)i / (float)quake_spheres.size())));

        //quake_model = quake_model.Translation(Vector3(-M_PI, -M_PI/2, -.05));
        Point3 quake_point;
        if (!globe_mode_)
        {
            quake_point = earth_.LatLongToPlane(quake_spheres[i].latitude(), quake_spheres[i].longitude());
        }
        else
        {
            quake_point = earth_.LatLongToSphere(quake_spheres[i].latitude(), quake_spheres[i].longitude());
        }

        if (globe_mode_ && scale_ < 1.0)
        {
            quake_point = earth_.LatLongToPlane(quake_spheres[i].latitude(), quake_spheres[i].longitude()).Lerp(earth_.LatLongToSphere(quake_spheres[i].latitude(), quake_spheres[i].longitude()), scale_);
        }
        if (!globe_mode_ && scale_ < 1.0)
        {
            quake_point = earth_.LatLongToSphere(quake_spheres[i].latitude(), quake_spheres[i].longitude()).Lerp(earth_.LatLongToPlane(quake_spheres[i].latitude(), quake_spheres[i].longitude()), scale_);
        }
        //std::cout << "QX: " << quake_point.x() << std::endl;
        //std::cout << "QY: " << quake_point.y() << std::endl;

        sphere_model = Matrix4::Translation(Vector3(quake_point.x(), quake_point.y(), quake_point.z())) * sphere_model;

        quick_shapes_.DrawSphere(sphere_model, view_matrix_, proj_matrix_, Color(1,1-((float)i/(float)quake_spheres.size()), 1 - ((float)i / (float)quake_spheres.size())));
    }
}



