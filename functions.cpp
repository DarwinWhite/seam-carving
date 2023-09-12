#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // delete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// Implement for part 1

int* createSeam(int length) {
  int* ptrArray = new int[length] {};
  return ptrArray;
}

void deleteSeam(int* seam) {
  delete[] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {

  //Opening file with file stream
  ifstream inFS;
  inFS.open(filename);

  //Making sure the file is open and outputting an error message if not
  if (!inFS.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }

  //Checking to make sure first line of file == P3 or p3
  string firstLine = "";
  inFS.clear();
  inFS >> firstLine;
  if (firstLine != "P3" && firstLine != "p3") {
    cout << "Error: type is " << firstLine << " instead of P3" << endl;
    return false;
  }

  //Checking to make sure stream has no errors after inputting from the
  // file and putting the input into integers, if there is an error,
  // it's probably not an integer in the file
  int fileWidth = 0; //For width and height from file
  int fileHeight = 0;
  inFS >> fileWidth >> fileHeight;
  if (inFS.fail()) {
    cout << "Error: read non-integer value" << endl;
    return false;
  }

  //Checking to make sure width and height in file == input width and
  // height
  if (fileWidth != width) {
    cout << "Error: input width (" << width << ") does not match value in file (" << fileWidth << ")" << endl;
    return false;
  }
  if (fileHeight != height) {
    cout << "Error: input height (" << height << ") does not match value in file (" << fileHeight << ")" << endl;
    return false;
  }

  //Initializing and assigning the max value to have something to compare against
  // and to make sure it's actually an int, also the min value to but just with 0
  int maxValue = 0;
  int minValue = 0;
  inFS >> maxValue;
  if (inFS.fail()) {
    cout << "Error: read non-integer value" << endl;
    return false;
  }

  //Reading in the values from the file in groups of 3, then assigning
  // those values to the struct's parameters
  int col = 0;
  int row = 0;
  int r = 0; //Just to error check before assinging to struct
  int g = 0;
  int b = 0;
  while (!inFS.eof() && inFS.good()) {
    inFS >> r >> g >> b;

    //Making sure the inputs are integers and making sure
    // that there are enough color values
    if (inFS.fail()) {
      inFS.clear();
      string check = "";
      getline(inFS, check);
      if (inFS.fail()) {
        cout << "Error: not enough color values" << endl;
      } else {
        cout << "Error: read non-integer value" << endl;
        return false;
      }
    }

    //Making sure the inputs are in the correct range of 0 - 255
    if (r < minValue || r > maxValue) {
      cout << "Error: invalid color value " << r << endl;
      return false;
    } else if (g < minValue || g > maxValue) {
      cout << "Error: invalid color value " << g << endl;
      return false;
    } else if (b < minValue || b > maxValue) {
      cout << "Error: invalid color value " << b << endl;
      return false;
    }

    //Assigning the values to the image 2D array using the
    // Pixel struct
    image[col][row].r = r;
    image[col][row].g = g;
    image[col][row].b = b;

    //Incrementing the col and row iterators based on the width
    // and height of the image
    if (col == width - 1) {
      row++;
    }
    if (col == width - 1) {
      col = 0;
    } else {
      col++;
    }

    //Checking to make sure there aren't too many color values
    // and also checking to make sure a valid file for whitespace
    // at the end
    if (row > height - 1 && !inFS.eof()) {
      string check = "";
      inFS >> check;
      if (inFS.fail()) {
        return true;
      } else {
        cout << "Error: too many color values" << endl;
        return false;
      }
    }
  }

  //Closing the file
  inFS.close();

  return true;
}

bool outputImage(string filename, const Pixel*const* image, int width, int height) {

  //Opening the file with the output stream
  ofstream outFS;
  outFS.open(filename);

  //Checking to make sure the output file is open and
  // ready to be written to
  if (!outFS.is_open()) {
    cout << "Error: failed to open output file - " << filename << endl;
    return false;
  }

  //Writing the preamble to the file in the form of
  // firstLine = P3, secondLine = width height,
  // thirdLine = maxColorValue
  outFS << "P3" << endl;
  outFS << width << " " << height << endl;
  outFS << "255" << endl;

  //Looping through the 2D array and outputting the values
  // of each color to the output file with spaces in between
  // numbers and row-by-row
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      outFS << image[j][i].r << " " << image[j][i].g << " " << image[j][i].b << " ";
    }
    outFS << endl;
  }

  //Closing the file
  outFS.close();

  return true;
}

int energy(const Pixel*const* image, int x, int y, int width, int height) { 
  
  //Initializing variables to represent the pixel to the right and left
  // of x, and the top and bottom of y
  int lowerX = 0;
  int upperX = 0;
  int lowerY = 0;
  int upperY = 0;

  //Initializing variables to hold the calculated energy values
  int xGradient = 0;
  int yGradient = 0;
  int totalEnergy = 0;

  //Writing if-else statements to represent the specific cases where x
  // or y is on the edge of the image and setting UpperX, LowerX, UpperY,
  // and LowerY to the correct pixel if it is on the edge
  if (x == 0) {
    lowerX = width - 1;
    upperX = x + 1;
  } else if (x == width - 1) {
    lowerX = x - 1;
    upperX = 0;
  } else {
    lowerX = x - 1;
    upperX = x + 1;
  }

  if (y == 0) {
    lowerY = height - 1;
    upperY = y + 1;
  } else if (y == height - 1) {
    lowerY = y - 1;
    upperY = 0;
  } else {
    lowerY = y - 1;
    upperY = y + 1;
  }

  //Calculating the dual gradient of both x and y using
  // the upperX and lowerX to calculate the x-gradient, and
  // the upperY and lowerY to calculate the y-gradient
  if (width < 3) {
    xGradient = 0;
  } else {
    xGradient = pow((image[upperX][y].r - image[lowerX][y].r), 2) 
      + pow((image[upperX][y].g - image[lowerX][y].g), 2) 
      + pow((image[upperX][y].b - image[lowerX][y].b), 2);
  }

  if (height < 3) {
    yGradient = 0;
  } else {
    yGradient = pow((image[x][upperY].r - image[x][lowerY].r), 2) 
      + pow((image[x][upperY].g - image[x][lowerY].g), 2) 
      + pow((image[x][upperY].b - image[x][lowerY].b), 2);
  }
  totalEnergy = xGradient + yGradient;

  return totalEnergy;
}

// Implement for part 2

int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {

  //Variables to store the total energy and current column
  // and to determine if the seam is on the border
  int totalEnergy = 0;
  int currCol = 0;
  bool isLeftBorder = false;
  bool isRightBorder = false;
  int energyForward = 0;
  int energyLeft = 0;
  int energyRight = 0;

  //For the specific case of the width being 1
  if (width == 1) {
    for (int i = 0; i < height; i++) {
      seam[i] = 0;
      totalEnergy += energy(image, start_col, i, width, height);
    }
    return totalEnergy;
  }

  //Setting seam for the first row to the starting column
  seam[0] = start_col;
  totalEnergy = energy(image, start_col, 0, width, height);
  currCol = start_col;

  //Looping through the seam array using height as the maximum
  // and calculating the energy for each pixel ahead of the current
  // pixel and then adding it to the seam array
  for (int i = 1; i < height; i++) {

    //Checking if the start_col is on one of the borders and
    // changing isLeftBorder and isRightBorder based on the
    // result
    if (currCol == width - 1) {
      isLeftBorder = true;
    }
    if (currCol == 0) {
      isRightBorder = true;
    }

    //When assigning energy values, whether or not the orignal
    // pixel is on the border will determine which values we
    // need to use
    if (isLeftBorder) {
      energyForward = energy(image, currCol, i, width, height);
      energyRight = energy(image, currCol - 1, i, width, height);
    } else if (isRightBorder) {
      energyForward = energy(image, currCol, i, width, height);
      energyLeft = energy(image, currCol + 1, i, width, height);
    } else {
      energyForward = energy(image, currCol, i, width, height);
      energyLeft = energy(image, currCol + 1, i, width, height);
      energyRight = energy(image, currCol - 1, i, width, height);
    }

    //Conditionals to determine direction of path, right if < both,
    // forward if < both, left if < both, forward if forward is tied
    // for min, or left if left is tied for min
    if (isLeftBorder) {
      if (energyRight < energyForward) {
        currCol--;
        seam[i] = currCol;
        totalEnergy += energyRight;
        isLeftBorder = false;
      } else {
        seam[i] = currCol;
        totalEnergy += energyForward;
      }
    } else if (isRightBorder) {
      if (energyLeft < energyForward) {
        currCol++;
        seam[i] = currCol;
        totalEnergy += energyLeft;
        isRightBorder = false;
      } else {
        seam[i] = currCol;
        totalEnergy += energyForward;
      }
    } else {
      if (energyRight < energyLeft && energyRight < energyForward) {
        currCol--;
        seam[i] = currCol;
        totalEnergy += energyRight;
      } else if (energyLeft < energyRight && energyLeft < energyForward) {
        currCol++;
        seam[i] = currCol;
        totalEnergy += energyLeft;
      } else if (energyLeft < energyForward && energyLeft == energyRight) {
        currCol++;
        seam[i] = currCol;
        totalEnergy += energyLeft;
      } else {
        seam[i] = currCol;
        totalEnergy += energyForward;
      }
    }
  }

  return totalEnergy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {

  //Variables to hold minimum energy, current energy,
  // minimun energy index, and the pointer to
  // the seam array
  int minEnergy = 0;
  int currEnergy = 0;
  int minIndex = 0;
  int* seam = createSeam(height);

  //Setting the seam of the first column of the image
  // to the minEnergy to be used further down
  minEnergy = loadVerticalSeam(image, 0, width, height, seam);
  minIndex = 0;

  //Looping through each column and calling the loadVerticalSeam
  // function to figure out the column with the lowest energy
  for (int i = 1; i < width; i++) {
    currEnergy = loadVerticalSeam(image, i, width, height, seam);
    if (currEnergy < minEnergy) {
      minEnergy = loadVerticalSeam(image, i, width, height, seam);
      minIndex = i;
    } else if (currEnergy == minEnergy) {
      minEnergy = loadVerticalSeam(image, minIndex, width, height, seam);
    }
  }

  return seam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {

  //Looping through the rows of the image using the seam array
  // and removing the pixels in the seam from the image
  for (int i = 0; i < height; i++) {
    if (verticalSeam[i] == width - 1) {
      continue;
    } else {
      for (int j = verticalSeam[i]; j < width - 1; j++) {
        image[j][i] = image[j + 1][i];
      }
    }
  }

  //Deleting the verticalSeam at the end to ensure there are
  // no memory leaks
  deleteSeam(verticalSeam);
}

/*
int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  return 0;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  return nullptr;
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
}
*/