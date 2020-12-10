# Assignment 6: Harold


## Sky Writing
Implementing drawing objects to the sky required the completion of the functions ```AddSkyStroke()``` and ```ScreenPtHitsSky()```. The work for translating 2D screen coordinates to 3D sky coordinates is done mainly in ```ScreenPtHitsSky()``` which uses the point of the camera's "eye" and the 2D Point's location on the Near Plane to calculate a vector which points towards the point in the stroke. This is then extended until it hits the 1500 unit radius sphere which acts as the sky. The point is simply moved along this ray until it hits the sky sphere and that is our new location for the 3D stroke point. ```AddSkyStroke()``` simply iterates through all the points in the stroke and applies this process to each one, finally adding the new 3D stroke to the list of strokes to draw.

## Terraforming Ground with the Crayon
All this "terraforming" takes place in the ```ReshapeGround()``` function. Here we know that the line which was drawn starts and ends on the ground. To start we define a plane based upon those two start and end points (after having transferred those two points from the 2D drawing plane to the scene's 3D coordinates). We can define a plane if we know these two things: 

- Normal Vector
	- The normal vector of the plane is calculated by taking the cross product of the "up" vector, aka <0, 1, 0>, with the vector between the start and end points of the stroke
- Point on the plane  
	- We can use the start or end point we already have

We then go through the exact same process as drawing on the sky above except that we are drawing to this plane instead. The final step is to use the formula outlined in the handout to calculate the y increment of each ground point given the stroke drawn on the plane in 3D space. This involves some math-definition-to-algorithm fun which eventually results in the new points for our drawn hill/valley.