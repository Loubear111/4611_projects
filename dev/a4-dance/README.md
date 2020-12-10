# Assignment 4: So You Think ~~Ants~~ Snowmen Can Dance
I chose to draw some cool snowmen with top hats! I say cool because they have some pretty rad glasses on :)

## Drawing Bones
The first order of business was correctly setting up the DrawBoneRecursive() function so that it could draw the whole "skeleton". This means we have to rotate each individual bone to the correct angle by utilizing built in functions which move our bone space into something called rotation space. Then once we have draw everything we want to draw for that given bone (for example a sphere for the snowman's head in my case), we move the transform into the space of the next child bone like so: 
```
Matrix4 child_root_transform = ctm * skeleton_.BoneSpaceToChildrenSpace(bone_name);
```
where `child_root_transform` is the matrix we pass to the next recursive call of DrawBoneRecursive(), given that the current bone has children.

## Drawing An Interesting Character
Building on top of what was said in the previous "Drawing Bones" section I'll now explain how my snowman was draw.

### The Head
The head of the snowman consists of a sphere for the head, a top hat, some glasses, and a nose. Each element is initially placed at the same node as the head and then translated/rotated/scaled to look nice and go where it should.

- __The head__ is simply a sphere placed at the location of the "upperneck" bone. This was done because the head had to be translated down some to fit on top of the body, and placing the sphere at the upperneck bone allowed for less translation. 

- __The top hat__ was made using two cylinders: one which is scaled to be very short but wide to look like a brim, and another which is scaled to be very tall but thinner to look like the top part of the hat. 
- __The glasses__ consist of two thin cylinders which are placed where eyes would be and then a stretched out rectangle which connects them like glasses!
- __The nose__ is stretched out tall cone which is rotated and colored orange to look like a carrot nose!

### The Rest
The rest of the body of the snowman is just a compilation of spheres. Two larger ones for the lower and upper body, two tiny spheres for the feet, and three small spheres for the charcoal buttons.

__NOTE:__ The feet are indeed supposed to "hover" and be disconnected from the body much like other animated characters such as Olaf the snowman from Frozen.

## Adding Animations
Adding animations was pretty simple and consisted of opening the file with the correct animation and then trimming the frames of the non-interesting bits of the animation.