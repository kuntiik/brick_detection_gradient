#include "functions.hpp"
using namespace cv;

extern const int HEIGHT= 480;
extern const int WIDTH = 848;

int16_t ** calculate_gradient(Mat image, const int *kernel){  
 //function to compute gradient given 3x3 kernel, modification needed for different kernel size
 //start with center of the picture (no problem with exceeding picture bounds) 
  int16_t **gradient = (int16_t **)malloc(HEIGHT * sizeof(int16_t *)); 
  //if(gradient == NULL){std::cout << "allocation problem" << std::endl;}
  for(int i=0; i<HEIGHT; i++){
       gradient[i] = (int16_t *)malloc(WIDTH * sizeof(int16_t)); 
  }
  //int16_t tmp = 0;
  int tmp = 0;
  for(int i = 1; i < HEIGHT - 1; i++){
    for(int j = 1; j < WIDTH - 1; j++){
      for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
          tmp += kernel[k*3+l]*image.at<uint16_t>(i+k-1, j+l-1);
        }
      }
      if(tmp >= 32000){tmp = 32000;}
      if(tmp <= -32000){tmp = -32000;}
      gradient[i][j] = tmp; 
      tmp = 0;
    }
  }
  //now compute gradient at the top row and the bottom row
  for(int j = 1; j < WIDTH-1; j++){
    for(int k = 1; k < 3; k++){
      for(int l = 0; l < 3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k-1, j+l-1);
      } 
    }
    if(tmp >= 32000){tmp = 32000;}
    if(tmp <= -32000){tmp = -32000;}
    gradient[0][j] = tmp;
    tmp = 0;

    for(int k = 0; k < 2; k++){
      for(int l = 0; l < 3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k-1, j+l-1);
      } 
    } 
    if(tmp >= 32000){tmp = 32000;}
    if(tmp <= -32000){tmp = -32000;}
    gradient[HEIGHT-1][j] = tmp;
    tmp = 0;
  }
  //the leftmost and rightmost column
  for(int i = 1; i < HEIGHT-1; i++){
    for(int k = 0; k < 3; k++){
      for(int l = 1; l <3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k+i-1,l-1);
      }
    }
    if(tmp >= 32000){tmp = 32000;}
    if(tmp <= -32000){tmp = -32000;}
    gradient[i][0] = tmp;
    tmp = 0;
    for(int k = 0; k < 3; k++){
      for(int l = 0; l <2; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k+i-1,l-1);
      }
    }
    if(tmp >= 32000){tmp = 32000;}
    if(tmp <= -32000){tmp = -32000;}
    gradient[i][WIDTH - 1] = tmp;
    tmp = 0;
  }
  //all four corners
  for(int k = 1; k < 3; k++){
    for(int l = 1; l <3; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1,l-1);
    }
  }
  if(tmp >= 32000){tmp = 32000;}
  if(tmp <= -32000){tmp = -32000;}
  gradient[0][0] = tmp;
  tmp = 0;
  for(int k = 1; k < 3; k++){
    for(int l = 0; l <2; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1,l-1 + WIDTH -1);
    }
  }
  if(tmp >= 32000){tmp = 32000;}
  if(tmp <= -32000){tmp = -32000;}
  gradient[0][WIDTH -1] = tmp;
  tmp = 0;
  for(int k = 0; k < 2; k++){
    for(int l = 1; l <3; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1+HEIGHT-1,l-1);
    }
  }
  if(tmp >= 32000){tmp = 32000;}
  if(tmp <= -32000){tmp = -32000;}
  gradient[HEIGHT-1][0] = tmp;
  tmp = 0;
  for(int k = 0; k < 2; k++){
    for(int l = 0; l <2; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1+HEIGHT-1,l-1+WIDTH-1);
    }
  }
  if(tmp >= 32000){tmp = 32000;}
  if(tmp <= -32000){tmp = -32000;}
  gradient[HEIGHT-1][WIDTH -1] = tmp;
  tmp = 0;

  return gradient;
}


int16_t ** calculate_blur(Mat image, const int *kernel, int norm){  
 //function to compute gradient given 3x3 kernel, modification needed for different kernel size
 //start with center of the picture (no problem with exceeding picture bounds) 
  int16_t **gradient = (int16_t **)malloc(HEIGHT * sizeof(int16_t *)); 
  if(gradient == NULL){std::cout << "allocation problem" << std::endl;}
  for(int i=0; i<HEIGHT; i++){
       gradient[i] = (int16_t *)malloc(WIDTH * sizeof(int16_t)); 
  }
  //int16_t tmp = 0;
  int tmp = 0;
  for(int i = 1; i < HEIGHT - 1; i++){
    for(int j = 1; j < WIDTH - 1; j++){
      for(int k = 0; k < 3; k++){
        for(int l = 0; l < 3; l++){
          tmp += kernel[k*3+l]*image.at<uint16_t>(i+k-1, j+l-1);
        }
      }
      tmp = tmp / norm;
      if(tmp >= 32000){tmp = 32000;}
      gradient[i][j] = tmp; 
      tmp = 0;
    }
  }
  //now compute gradient at the top row and the bottom row
  for(int j = 1; j < WIDTH-1; j++){
    for(int k = 1; k < 3; k++){
      for(int l = 0; l < 3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k-1, j+l-1);
      } 
    }
    tmp = tmp / norm;
    if(tmp >= 32000){tmp = 32000;}
    gradient[0][j] = tmp;
    tmp = 0;

    for(int k = 0; k < 2; k++){
      for(int l = 0; l < 3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k-1, j+l-1);
      } 
    } 
    tmp = tmp / norm;
    if(tmp >= 32000){tmp = 32000;}
    gradient[HEIGHT-1][j] = tmp;
    tmp = 0;
  }
  //the leftmost and rightmost column
  for(int i = 1; i < HEIGHT-1; i++){
    for(int k = 0; k < 3; k++){
      for(int l = 1; l <3; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k+i-1,l-1);
      }
    }
    tmp = tmp / norm;
    if(tmp >= 32000){tmp = 32000;}
    gradient[i][0] = tmp;
    tmp = 0;
    for(int k = 0; k < 3; k++){
      for(int l = 0; l <2; l++){
        tmp += kernel[k*3+l]*image.at<uint16_t>(k+i-1,l-1);
      }
    }
    tmp = tmp / norm;
    if(tmp >= 32000){tmp = 32000;}
    gradient[i][WIDTH - 1] = tmp;
    tmp = 0;
  }
  //all four corners
  for(int k = 1; k < 3; k++){
    for(int l = 1; l <3; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1,l-1);
    }
  }
    tmp = tmp / norm;
  if(tmp >= 32000){tmp = 32000;}
  gradient[0][0] = tmp;
  tmp = 0;
  for(int k = 1; k < 3; k++){
    for(int l = 0; l <2; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1,l-1 + WIDTH -1);
    }
  }
    tmp = tmp / norm;
  if(tmp >= 32000){tmp = 32000;}
  gradient[0][WIDTH -1] = tmp;
  tmp = 0;
  for(int k = 0; k < 2; k++){
    for(int l = 1; l <3; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1+HEIGHT-1,l-1);
    }
  }
    tmp = tmp / norm;
  if(tmp >= 32000){tmp = 32000;}
  gradient[HEIGHT-1][0] = tmp;
  tmp = 0;
  for(int k = 0; k < 2; k++){
    for(int l = 0; l <2; l++){
      tmp += kernel[k*3+l]*image.at<uint16_t>(k-1+HEIGHT-1,l-1+WIDTH-1);
    }
  }
    tmp = tmp / norm;
  if(tmp >= 32000){tmp = 32000;}
  gradient[HEIGHT-1][WIDTH -1] = tmp;
  tmp = 0;

  return gradient;
}

