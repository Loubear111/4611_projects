# CSCI 4611 Assignment 3: Earthquake
**Name:** Louis Barrett
**Class:** CSCI4611
**Date:** 3/4/2020
**x500:** barre613@umn.edu

## Mesh Creation
There were 3 things that were created in order to mesh the earth texture onto the rectangle/sphere of the earth. Vertices, Texture coordinates, and Indices.

### Vertices and Texture Coordinates
These were calculated in a nest set of for loops which looped `from 0 to nstacks` and `from 0 to nslices`. Within these loops we calculate the actual x and y coordinates in the plane based on an increment calculated using the iterated value 0 to nslices or 0 to nstacks. It looks somewhat like this:

```
// X mapped to range (-PI, PI)
float xcoord = -M_PI + (2*M_PI/nslices)*x;

// Y mapped to range (-PI/2, PI/2)
float ycoord = -(M_PI / 2) + (M_PI/nstacks)*y;
```

for the actual x and y coordinates in the texture we have to do something a bit different since the texture is only mapped from 0->1 in x and y. It looks somewhat like this:

```
// X mapped to range (0, 1)
float textX = (1 / nslices) * x;

// Y mapped to range (1, 0)
float textY = 1 - (1 / nstacks) * y;
```
### Indices
For this we use a similar loop to above where we loop to nstacks and nslices but we do some calculations so that the order in which the indices are declared is such that the order is counter-clockwise so that they "point" towards our eyes so we can see the texture.

### Converting to Sphere
To convert to a sphere all we had to do was modify the vertices such that they were in spherical coordinates. There is a for loop identical to the one for calculating vertices and texture coordinates except that it does not touch texture coordinates and simply applies a circular transformation to the vertices like so:

```
Point3 new_point = Point3(sin(xcoord) * cos(ycoord), sin(ycoord), cos(xcoord) * cos(ycoord))
```

The vertices needed to then be sent to the GPU memory using these commands:
```
earth_mesh_.SetVertices(vertices);
earth_mesh_.UpdateGPUMemory();
```
## Smooth Transitions
Smooth transitions between sphere and flat Earths were done using the Lerp function. A scale variable which was slowly incremented each draw frame was used as the second argument to Lerp while the new point location was used as the first argument like so:

```
// scale incremented each draw frame by .01 until it hits 1.0
vertices.push_back(old_point.Lerp(new_point, scale))
```

## Earthquakes!
Earthquakes in my representation were shown as spheres that start out big and red when the earthquake first happens and then slowly get smaller and less red until they disappear.

These values for size and redness are calculated based upon their position in the vector array which holds the most recent quakes. So the most recent will be bigger and redder!

This vector array which holds the quakes is given a max size and simply sends items out the back into the trash when the max size is reached. This allows old earthquakes to be discarded when their time has passed.

### Latitude and Longitude to Spheres and Planes
This was done using the mapping equation presented in the assignment3 handout. To calculate the location for the sphere the equation which calculates its position on the plane is used and then the same function which maps planer vertices to new locations is used to map the earthquake planer points to their spherical counterparts.

**To Plane:**
```
double x = -M_PI + (2 * M_PI) * (longitude + 180) / (180 + 180);
double y = -M_PI/2 + (M_PI) * (latitude + 90) / (90 + 90);

return Point3(x, y, 0);
```
**To Sphere:**
```
Point3 plane_coord = LatLongToPlane(latitude, longitude);

Point3 sphere_coord = Point3(sin(plane_coord.x())*cos(plane_coord.y()), sin(plane_coord.y()), cos(plane_coord.x()) * cos(plane_coord.y()));
```

## Other Additions
I also added a "spin" while in globe mode. I never got around to adding mouse dragging, but the spin allows you to see the whole globe and the correct placement of all the different earthquakes that are happening while in globe mode.

**Note:** Program lags a lot when you are in sphere mode and you toggle debug mode at the same time. It still works, its just really slow!
