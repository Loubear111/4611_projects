/**
    CSci-4611 Assignment #1 Text Rain
**/


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;
PFont font;
boolean debugMode = false;
int threshold = 128;
int textSize = 24;
String words[] = {"test", "hello", "imagine", "text", "learn", "rain", "create", "inspire"};

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
ArrayList<TextObj> rainDrops = new ArrayList<TextObj>();

/*
 * updateText()
 * Here is where we do all the physics and hit detection for each individual letter
 */
void updateText()
{
  for(int i = 0; i < rainDrops.size(); i++)
  {
    // Check for any collisions and handle the collision physics accordingly
    if(collides(rainDrops.get(i), rainDrops.get(i).y))
    {
      // This looks for white space so our text doesn't get "stuck" in a black area
      // it will teleport it upwards to make the text sit smoothly on black surfaces
      boolean foundWhite = false;
      for(int n = 0; n < rainDrops.get(i).y; n++)
      {
        if(rainDrops.get(i).y - n > 0)
        {
          if(!collides(rainDrops.get(i), rainDrops.get(i).y - n))
          {
            rainDrops.get(i).y -= n;
            foundWhite = true;
            break;
          }
        }
      }
      
      if(!foundWhite)
      {
        rainDrops.get(i).y = 0; 
      }
    }
    else
    {
      // If we got here it means this text object is free-falling with no black below it
      // so increase it's y by it's velocity
      rainDrops.get(i).y += rainDrops.get(i).vely;
      
      if(rainDrops.get(i).y > inputImage.height-10)
      {
        rainDrops.get(i).y = 0; 
      }
      
      // This looks for white space so our text doesn't get "stuck" in a black area when it got moved downwards from its velocity
      // it will teleport it upwards to make the text sit smoothly on black surfaces
      boolean foundWhite = false;
      for(int n = 0; n < rainDrops.get(i).y; n++)
      {
        if(rainDrops.get(i).y - n > 0)
        {
          if(!collides(rainDrops.get(i), rainDrops.get(i).y - n))
          {
            rainDrops.get(i).y -= n;
            foundWhite = true;
            break;
          }
        }
      }
      
      if(!foundWhite)
      {
        rainDrops.get(i).y = 0; 
      }
    }
    rainDrops.get(i).lastUpdate = millis();
  }
}

/*
 * collides(TextObj rainDrop, int y)
 * This is strictly a utility function used by updateText()
 * to do pixel perfect bottom-edge detection
 *
 */
boolean collides(TextObj rainDrop, int y)
{
  // This loop makes sure we collision detect for the whole width of the character's bottom
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
  return false;
}

/*
 * debugView()
 * Colors pixels on screen as white or black depending
 * on the number for the threshold.
 *
 */
void debugView()
{
  inputImage.loadPixels();
  
  for(int i = 0; i < inputImage.pixels.length; i++)
  {
    if(green(inputImage.pixels[i]) < threshold)
    {
      inputImage.pixels[i] = color(0,0,0); 
    }
    else
    {
      inputImage.pixels[i] = color(255,255,255);  
    }
  }
  
  inputImage.updatePixels();
}

/*
 * flipImage()
 * Flips the image around the Y-axis to create a mirror
 * effect if using a webcam.
 * 
 */
void flipImage()
{
  // iterate first through each row individually so we can flip it
  for(int r = 0; r < inputImage.height; r++)
  {
    // create a temp array to hold the old row pixel values
    color[] pixelRow = new color[inputImage.width];
    for(int c = 0; c < inputImage.width; c++)
    {
      pixelRow[c] = inputImage.pixels[inputImage.width * (r) + c]; 
    }
    
    // now that we have a temp array, assign flipped values
    for(int c = 0; c < inputImage.width; c++)
    {
      inputImage.pixels[inputImage.width * (r) + c] = pixelRow[inputImage.width - c - 1];
    }
  }
}

void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  
  // load in font data
  font = loadFont("CourierNewPSMT-48.vlw");
  textFont(font, 48);
 
  // Load in hard coded array of strings and split them up into chars
  // randomly color coding each word so they're identifiable
  int lastx = 0;
  for(int i = 0; i < words.length; i++)
  {
    color c = color(int(random(70, 255)), int(random(70, 255)), int(random(70, 255)));
    for(int n = 0; n < words[i].length(); n++)
    {
      if(n != 0)
      {
        // Space out text according to text width
        // had to do this hackily with below line 
        lastx += int(textWidth(rainDrops.get(0).s));
      }
      
      // Don't let the text go off the screen
      if(lastx > inputImage.width)
      {
        lastx = int(random(textWidth(rainDrops.get(0).s)));
      }
      int y = int(random(inputImage.height));
      rainDrops.add(new TextObj(Character.toString(words[i].charAt(n)), lastx, y, c));
    }
    // Space out our words randomly
    lastx += random(int(textWidth(rainDrops.get(0).s)), int(textWidth(rainDrops.get(0).s)*1.5));
    
    // Again don't let them go off the screen
    if(lastx > inputImage.width)
    {
      lastx = int(random(textWidth(rainDrops.get(0).s)));
    }
  }
}


void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }


  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.

  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, cam.width,0,inputImage.width,inputImage.height);
    flipImage();
    inputImage.filter(GRAY);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
    flipImage();
    inputImage.filter(GRAY);
  }

  if(debugMode)
  {
    debugView(); 
  }

  // Tip: This code draws the current input image to the screen
  set(0, 0, inputImage);

  // update text physics
  updateText();

  fill(#CA50F7);
  // actually draw the text
  for(int i = 0; i < rainDrops.size(); i++)
  {
    fill(rainDrops.get(i).c);
    text(rainDrops.get(i).s, rainDrops.get(i).x, rainDrops.get(i).y);
  }
  
  fill(#000000);
  textFont(font, 24);
  text("Press 'Space' for debug", 0, 670);
  text("Threshold:", 0, 700);
  text(threshold, 150, 700);
  textFont(font, textSize);
  fill(#FFFFFF);
}



void keyPressed() {
  
  if (!inputMethodSelected) {
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }


  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..
  
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      threshold++;
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      threshold--;  
    }
  }
  else if (key == ' ') {
    debugMode = !debugMode;
  } 
  
}
