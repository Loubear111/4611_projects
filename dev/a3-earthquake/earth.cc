/** CSci-4611 Assignment 3:  Earthquake
 */

#include "earth.h"
#include "config.h"

#include <vector>

// for M_PI constant
#define _USE_MATH_DEFINES
#include <math.h>

Earth::Earth() : nslices(20), nstacks(20), spin(0.0){
}

Earth::~Earth() {
}

void Earth::Init(const std::vector<std::string> &search_path) {
    // init shader program
    shader_.Init();
    
    // init texture: you can change to a lower-res texture here if needed
    earth_tex_.InitFromFile(Platform::FindFile("earth-2k.png", search_path));

    // TODO: This is where you need to set the vertices and indiceds for earth_mesh_.

    // As a demo, we'll add a square with 2 triangles.
    std::vector<unsigned int> indices;
    std::vector<Point3> vertices;
    std::vector<Vector3> normals;
    std::vector<Point2> tex_coords;

    // Setup vertices and texture coords
    for (int y = 0; y <= nstacks; y++)
    {
        for (int x = 0; x <= nslices; x++)
        {
            float xcoord = -M_PI + (2*M_PI/nslices)*x;
            float ycoord = -(M_PI / 2) + (M_PI/nstacks)*y;

            float textX = (float)(1 / (float)nslices) * (float)x;
            float textY = 1 - (float)(1 / (float)nstacks) * (float)y;

            vertices.push_back(Point3(xcoord, ycoord, 0));
            tex_coords.push_back(Point2(textX, textY));

            //std::cout << Point3(xcoord, ycoord, 0) << std::endl;
            //std::cout << Point2(textX, textY) << std::endl;
        }
    }

    // Setup indicies
    for (int y = 1; y <= nstacks; y++)
    {
        for (int x = 1; x <= nslices; x++)
        {
            int i = (nslices+1) * (y - 1) + (x-1);

            indices.push_back(i + nslices + 2); 
            indices.push_back(i + nslices + 1); 
            indices.push_back(i); 

            normals.push_back(Vector3(0, 0, 1));

            indices.push_back(i + 1);
            indices.push_back(i + nslices + 2);
            indices.push_back(i);

            normals.push_back(Vector3(0, 0, 1));
        }
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.SetIndices(indices);
    //earth_mesh_.SetNormals(normals);
    earth_mesh_.SetTexCoords(0, tex_coords);

    earth_mesh_.UpdateGPUMemory();
}

void Earth::GoFlat(float scale)
{
    std::vector<Point3> vertices;

    // Setup vertices and texture coords
    for (int y = 0; y <= nstacks; y++)
    {
        for (int x = 0; x <= nslices; x++)
        {
            float xcoord = -M_PI + (2 * M_PI / nslices) * x;
            float ycoord = -(M_PI / 2) + (M_PI / nstacks) * y;

            Point3 old_point = Point3(sin(xcoord) * cos(ycoord), sin(ycoord), cos(xcoord) * cos(ycoord));

            Point3 new_point = Point3(xcoord, ycoord, 0);

            vertices.push_back(old_point.Lerp(new_point, scale));
        }
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.UpdateGPUMemory();
}

void Earth::GoRound(float scale)
{
    std::vector<Point3> vertices;

    // Setup vertices and texture coords
    for (int y = 0; y <= nstacks; y++)
    {
        for (int x = 0; x <= nslices; x++)
        {
            float xcoord = -M_PI + (2 * M_PI / nslices) * x;
            float ycoord = -(M_PI / 2) + (M_PI / nstacks) * y;

            Point3 old_point = Point3(xcoord, ycoord, 0);

            Point3 new_point = Point3(sin(xcoord) * cos(ycoord), sin(ycoord), cos(xcoord) * cos(ycoord));

            vertices.push_back(old_point.Lerp(new_point, scale));
        }
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.UpdateGPUMemory();
}

void Earth::Spin()
{
    std::vector<Point3> vertices;

    // Setup vertices and texture coords
    for (int y = 0; y <= nstacks; y++)
    {
        for (int x = 0; x <= nslices; x++)
        {
            float xcoord = -M_PI + (2 * M_PI / nslices) * x;
            float ycoord = -(M_PI / 2) + (M_PI / nstacks) * y;

            //Point3 old_point = Point3(xcoord, ycoord, 0);

            Point3 new_point = Point3(sin(spin+xcoord) * cos(ycoord), sin(ycoord), cos(spin + xcoord) * cos(ycoord));

            vertices.push_back(new_point);
        }
    }

    spin += .005;
    if (spin > 2 * M_PI)
    {
        spin = 0;
    }

    earth_mesh_.SetVertices(vertices);
    earth_mesh_.UpdateGPUMemory();
}

void Earth::Draw(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // Define a really bright white light.  Lighting is a property of the "shader"
    DefaultShader::LightProperties light;
    light.position = Point3(10,10,10);
    light.ambient_intensity = Color(1,1,1);
    light.diffuse_intensity = Color(1,1,1);
    light.specular_intensity = Color(1,1,1);
    shader_.SetLight(0, light);

    // Adust the material properties, material is a property of the thing
    // (e.g., a mesh) that we draw with the shader.  The reflectance properties
    // affect the lighting.  The surface texture is the key for getting the
    // image of the earth to show up.
    DefaultShader::MaterialProperties mat;
    mat.ambient_reflectance = Color(0.5, 0.5, 0.5);
    mat.diffuse_reflectance = Color(0.75, 0.75, 0.75);
    mat.specular_reflectance = Color(0.75, 0.75, 0.75);
    mat.surface_texture = earth_tex_;

    // Draw the earth mesh using these settings
    if (earth_mesh_.num_triangles() > 0) {
        shader_.Draw(model_matrix, view_matrix, proj_matrix, &earth_mesh_, mat);
    }
}


Point3 Earth::LatLongToSphere(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> sphere calculations in one place.

    Point3 plane_coord = LatLongToPlane(latitude, longitude);

    Point3 sphere_coord = Point3(sin(spin + plane_coord.x())*cos(plane_coord.y()), sin(plane_coord.y()), cos(spin + plane_coord.x()) * cos(plane_coord.y()));

    return sphere_coord;
}

Point3 Earth::LatLongToPlane(double latitude, double longitude) const {
    // TODO: We recommend filling in this function to put all your
    // lat,long --> plane calculations in one place.
    double x = -M_PI + (2 * M_PI) * (longitude + 180) / (180 + 180);
    double y = -M_PI/2 + (M_PI) * (latitude + 90) / (90 + 90);

    //std::cout << "lon: " << longitude << std::endl;
    //std::cout << "x:   " << x << std::endl;
    //std::cout << "y:   " << y << std::endl;
    return Point3(x,y,0);
}



void Earth::DrawDebugInfo(const Matrix4 &model_matrix, const Matrix4 &view_matrix, const Matrix4 &proj_matrix) {
    // This draws a cylinder for each line segment on each edge of each triangle in your mesh.
    // So it will be very slow if you have a large mesh, but it's quite useful when you are
    // debugging your mesh code, especially if you start with a small mesh.
    for (int t=0; t<earth_mesh_.num_triangles(); t++) {
        std::vector<unsigned int> indices = earth_mesh_.triangle_vertices(t);
        std::vector<Point3> loop;
        loop.push_back(earth_mesh_.vertex(indices[0]));
        loop.push_back(earth_mesh_.vertex(indices[1]));
        loop.push_back(earth_mesh_.vertex(indices[2]));
        quick_shapes_.DrawLines(model_matrix, view_matrix, proj_matrix,
            Color(1,1,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.005);
    }
}

