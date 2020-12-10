## Assessment for Assignment 04 (Graded By: CSCI 4611 TAs)

#### Total score: _3_ / _100_

Run on April 08, 16:39:26 PM.


### Necessary Files and Structure

+ :heavy_check_mark:  Check that directory "dev" exists.

+ :heavy_check_mark:  Copy directory "Files for Testing".



+ :heavy_check_mark:  Check that directory "dev/MinGfx" exists.


### Compile Tests

+ :heavy_check_mark:  _1_ / _1_ :  Check that directory "dev/a4-dance" exists.

+ :heavy_check_mark:  Change into directory "dev/a4-dance".

+ :heavy_check_mark:  _1_ / _1_ :  Configuring assignment for grading.



+ :heavy_check_mark:  Make directory "build".

+ :heavy_check_mark:  Change into directory "build".

+ :heavy_check_mark:  _1_ / _1_ :  Check that CMake Configures.

+ :x:  _0_ / _1_ :  Check that make compiles.

    Make compile fails with errors:.
<pre>Scanning dependencies of target a4-dance
[  5%] Building CXX object CMakeFiles/a4-dance.dir/animated_character.cc.o
In file included from /project/grades/Spring-2020/csci4611/kimx3133/grading-env/grading-scripts/grading/Assignment_04_Assessment/repo-barre613/dev/a4-dance/animated_character.cc:1:0:
/project/grades/Spring-2020/csci4611/kimx3133/grading-env/grading-scripts/grading/Assignment_04_Assessment/repo-barre613/dev/a4-dance/animated_character.h:140:10: error: extra qualification AnimatedCharacter:: on member DrawSnowmanSphere [-fpermissive]
     void AnimatedCharacter::DrawSnowmanSphere(const Matrix4& modelMatrix,
          ^~~~~~~~~~~~~~~~~
CMakeFiles/a4-dance.dir/build.make:62: recipe for target 'CMakeFiles/a4-dance.dir/animated_character.cc.o' failed
make[2]: *** [CMakeFiles/a4-dance.dir/animated_character.cc.o] Error 1
CMakeFiles/Makefile2:67: recipe for target 'CMakeFiles/a4-dance.dir/all' failed
make[1]: *** [CMakeFiles/a4-dance.dir/all] Error 2
Makefile:129: recipe for target 'all' failed
make: *** [all] Error 2
</pre>



+ :leftwards_arrow_with_hook:  _0_ / _1_ :  Check that a GoogleTest test passes. (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _50_ :  Check project was submitted on time. (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _10_ :  Program can be compiled and for grading based on the version submitted in github at the deadline. (Test not run because of an earlier failing test)


### Assignment Tests


#### Work in the "C" Range

+ :leftwards_arrow_with_hook:  _0_ / _8_ :  The correct current transformation matrix is used for drawing each bone (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _7_ :  The child_root_transform is correctly set before the recursive call at the end of the function. (Test not run because of an earlier failing test)


#### Work in the "B" Range

+ :leftwards_arrow_with_hook:  _0_ / _4_ :  Draw at least a stick fgure (e.g., line segment or cylinder) representation for the each bone (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _4_ :  Four additional special motions are defined, loaded, trimmed, and then played. (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _2_ :  Motions smoothly transition from the ballet dancer’s base loop motion into all of these special motions whenever a new motion is played. (Test not run because of an earlier failing test)


#### Work in the "A" Range

+ :leftwards_arrow_with_hook:  _0_ / _4_ :  QuickShapes are used to draw simple geometries like spheres, cubes, cylinders, cones, etc.. (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _3_ :  Transformation matrices (Matrix4s) are used to apply scales, translations, and rotations to the model matrix. (Test not run because of an earlier failing test)

+ :leftwards_arrow_with_hook:  _0_ / _3_ :  Colors and several geometries are used to make a convincing character. (Test not run because of an earlier failing test)

#### Total score: _3_ / _100_

