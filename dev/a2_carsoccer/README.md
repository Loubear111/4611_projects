# Assignment 2 Car Soccer
**Name:** Louis Barrett
**Class:** CSCI4611
**Date:** 2/20/2020
**x500:** barre613@umn.edu

## Movement
The ball and the car both move by using their velocities and positions each frame. To do this we multiply their velocity vectors by a time-step and then move their position by that calculated velocity vector (by using '+' operater with the position and velocity).

We also add gravity to the ball which is also added as an acceleration: 
`newVelY = oldVelY + Gravity * timestep_`

The last step is controlling the car with the arrow keys. The two left and right keys will change the car's angle variable. This angle is then broken down into a 2D vector with an x and z component and then used as the x and z componant of the car's velocity vector. The up and down keys will then decrement or increment the velocity to be along that x-z plane positively or negatively.

**Note:** I noticed that depending on the machine which it was ran the ball and car would move faster or slower even though it was based on a timestep.

## Collision
Collision is done per object when detecting collision with the walls or goals. Each has their own update function where it is moved as described above and then any collisions are checked for with its new position.

For the ball we first check for collisions with the goal before anything else. If it did collide with the goal we reset the ball to the middle and print out who scored to the terminal (if the ball goes in the red goal it means blue scored, if the ball goes in the blue goal it means red scored).

### Logic / Calculations
Both the ball and the car are treated as spheres with a given radius. Any time we test for a collision we add or subtract the radius of the sphere to see if it has gone through (or is touching) a wall or another sphere. If a collision is detected we move the object so that it is not inside the other object it is colliding with (also off-putting it by a factor epsilon to avoid any minor differences with the floating point calculations)
