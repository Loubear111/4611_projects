# Assignment 1 Text Rain
**Name:** Louis Barrett
**Class:** CSCI4611
**Date:** 2/4/2020
**x500:** barre613@umn.edu

## Text Selection
All the text in my text rain implementation is taken in through an array that is declared at the top of the file just like this:
```
String words[] = {"test", "hello", "imagine", "text", "learn", "rain", "create", "inspire"};
```
Next, the words are split up into characters and given a random y position and a x position that is set at a fixed length from the previous character in the word. Before the word is split up it is given a random color so that when falling it can be distinguished.

The characters are each stored in their own class instance called TextObj:
```
class TextObj {
 String s;
 color c;
 int x;
 int y;
 int lastUpdate = 0;
 int velx = 0;
 int vely = int(random(5, 15));
 
 TextObj(String s, int x, int y, color c)
 {
   this.s = s;
   this.x = x;
   this.y = y;
   this.c = c;
 }
}
```
As you can see that y velocity is randomly selected when each class instance is created so that the "rain" appears more realistic.

## Collision Detection
Collision detection was all handled in a single function that is called while doing the physics for each of the text objects above.

```
  for(int c = 0; c < int(textWidth(rainDrop.s)); c++)
  {
    if(c < inputImage.width)
    {
      if(green(inputImage.pixels[inputImage.width * (y) + (rainDrop.x + c)]) < threshold)
      {
        return true; 
      }
    }
  }
```
The above code simply runs through each of the bottom pixels of the character and checks if it is in the black region (returning true). Otherwise it returns false if it is not in a black region.

### Physics Related to Collision
The physics code, if it detects a collision in the collision code, will attempt to "teleport" the character up until no more collisions are detected. This gives the text an extremely smooth look when it is sitting on a black surface such as the top of the umbrella. This is all done in the ``` updateText() ``` function.

## Wizard Changes
Nothing to report for Wizardry this time around, but I hope the README looks pretty along with the colorful text in the program :)
