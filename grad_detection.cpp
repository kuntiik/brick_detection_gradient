#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include <cmath>
#include "functions.hpp"
using namespace cv;
using namespace std;
extern const int HEIGHT;
extern const int WIDTH;
//
const int X_GRAD_KERNEL[9] = {-1, 0, 1, -2, 0, 2, -1 , 0, 1};
const int Y_GRAD_KERNEL[9] = {-1, -2, -1, 0 , 0, 0, 1, 2, 1};
const int G_BLUR_KERNEL[9] = {1, 2, 1, 2, 4 , 2, 1, 2, 1};

struct Coord {
  int x;
  int y;
};

class Stack{
  public:
    int size;
    int *storage;
    int tail;
  Stack(int s){
    size = s;
    tail = 0;
    storage = (int*)malloc(2*s*sizeof(int));
  }
  ~Stack(){
    free(storage);
  }
  void push(int x, int y){
   if(tail >= size - 3){
     size = 2*size;
     storage = (int*)realloc(storage, sizeof(int)*size);
   }
   tail++;
   storage[tail] = x;
   tail++;
   storage[tail] = y;
  }
  Coord pop(){
    struct Coord ret;
    ret.y = storage[tail];
    tail--;
    ret.x = storage[tail];
    tail--;
    return ret;
  }
  bool is_empty(){
    if(tail <= 0){ return true; }
    else{return false;}
  }
};


int make_regions(int16_t **grad, float diff);
void expand(int16_t **grad, float diff,int **completed, int region_number, Stack* stack, bool *out, Coord *out_coord, int region_value, int *not_sorted);
bool in_picture(Coord coord);
Coord find_zero(int** completed);
void color_regions(int **completed);
int16_t **combine_grad(int16_t **x_grad, int16_t **y_grad);
void free_2d(int16_t **pointer);
void free_2d(int **pointer);


void free_2d(int16_t **pointer){
  for(int i = 0; i < HEIGHT; i++){
    free(pointer[i]);
        }
  free(pointer);
}

void free_2d(int **pointer){
  for(int i = 0; i < HEIGHT; i++){
    free(pointer[i]);
        }
  free(pointer);
}

int make_regions(int16_t **grad, float diff){
  Coord first_out;
  int region_number = 1;
  bool found_out = true;
  int not_sorted = HEIGHT * WIDTH -1; 
  //int completed[HEIGHT][WIDTH] = { 0 };
  //array of int, it represents number of region for each pixel
  //init it all to zero
  int **completed = (int**)malloc(HEIGHT*sizeof(int*));
  for(int i = 0; i < HEIGHT; i++){
    completed[i] = (int*)malloc(WIDTH*sizeof(int));
  }
  for(int i =0; i < HEIGHT;i++){
    for(int j = 0; j < WIDTH; j++){
      completed[i][j] = 0;
    }
  }
  Stack stack(4000);
  
  first_out.x = 0;
  first_out.y = 0;
  int region_value;
    
  while(not_sorted != 0){
    if(found_out){
      found_out = false;
    }
    else{
      first_out = find_zero(completed);
    }
    stack.push(first_out.x, first_out.y);  
    completed[first_out.y][first_out.x] = region_number;
    region_value = grad[first_out.y][first_out.x];
    not_sorted--;
  while(!stack.is_empty()){
    expand(grad, diff, completed, region_number, &stack, &found_out, &first_out, region_value, &not_sorted);
  }
  //found whole region 
  region_number++; 
  }
  cout << region_number << endl;
  color_regions(completed);
  free_2d(completed);
  return 0;
  }

void expand(int16_t **grad, float diff,int **completed, int region_number, Stack* stack, bool *out, Coord *out_coord, int region_value, int *not_sorted){

  //init coord, tkat i will iterate over

  Coord it_tuple[] = {
    { it_tuple[0].x = -1, it_tuple[0].y = 0},
    { it_tuple[1].x =  1, it_tuple[1].y = 0},
    { it_tuple[2].x =  0, it_tuple[2].y = 1},
    { it_tuple[3].x =  0, it_tuple[3].y = -1}};
    
  Coord coord;
  Coord t;
  //pop array element and expand
  coord = stack->pop();
  for(int i = 0; i <4; i++){
    t.x = coord.x + it_tuple[i].x;
    t.y = coord.y + it_tuple[i].y; 
    if(in_picture(t) && completed[t.y][t.x] == 0){
      if(grad[t.y][t.x] + diff >= region_value && region_value >= grad[t.y][t.x] - diff){
        stack->push(t.x, t.y);
        completed[t.y][t.x] = region_number;
        (*not_sorted)--;
      }
      //first out is stored, to save some computaton time 
      else{
        if (*out == false){
          out_coord->x = t.x;
          out_coord->y = t.y;
          *out = true;
        }
    }
    }
  } 
}

bool in_picture(Coord coord){
  if(coord.x < 0 || coord.x >= WIDTH){return false;}
  else if(coord.y < 0 || coord.y >= HEIGHT){return false;}
  else{return true;}
}

Coord find_zero(int** completed){
  Coord first_zero;
  bool found = false;
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      if(completed[i][j] == 0){
        found = true;
        first_zero.x = j;
        first_zero.y = i;
        break;
      }  
    }
    if(found){break;}
  }
  return first_zero;
}

void color_regions(int **completed){
  int tmp;
  uint8_t data[HEIGHT][WIDTH][3];
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      tmp = completed[i][j] % 15;
      data[i][j][tmp % 3] = tmp%5 * 64 -1; 
      data[i][j][tmp+1 % 3] = (tmp+1)%5 * 64 -1; 
      data[i][j][tmp+2 % 3] = (tmp+3)%5 * 64 -1; 
      //data[i][j][0] = 255; 
      //data[i][j][1] =  0;
      //data[i][j][2] =  255;
    }
  }
  Mat image_col(HEIGHT, WIDTH, CV_8UC3, data);
  imshow("Colors", image_col);
}

int16_t **combine_grad(int16_t **x_grad, int16_t **y_grad){
  int16_t **combined = (int16_t**)malloc(HEIGHT*sizeof(int16_t*));
  for(int i =0; i < HEIGHT; i++){
    combined[i] = (int16_t*)malloc(WIDTH*sizeof(int16_t));
  }
  int tmp;
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      tmp = sqrt(x_grad[i][j]^2 + y_grad[i][j]^2);
      if(tmp > 32000){tmp = 32000;}
      combined[i][j] = (int16_t)tmp;  
    }
  }
  return combined;
}
int main(int argc, char **argv){
  
  Mat image;
  FileStorage fs;
  if (argc<2) {
    fs.open("prob0001.xml", FileStorage::READ);
  } else {
    fs.open(argv[1], FileStorage::READ);
  }
  fs["depth"] >> image;

  //loaded data and converting them to 0-255 to display them properly
  uint16_t tmp;
  uint8_t im_array[480][848];
  for(int i = 0; i < 480; i++){
    for(int j = 0; j < 848; j++){
      tmp = image.at<uint16_t>(i,j)/20;
      if(tmp >= 255){tmp = 255;}
      im_array[i][j] = (uint8_t)tmp;
    }
  }  
  int16_t **x_grad = calculate_gradient(image, X_GRAD_KERNEL);   
  int16_t **y_grad = calculate_gradient(image, Y_GRAD_KERNEL);
  cout << "grad calculated" << endl;
  long long int result = 0;
  int16_t max = 0;
  int16_t min = 0;
  //for(int i = 270; i < 370;i++){
    //for(int j = 570; j < 630; j++){
    ////cout << y_grad[i][j] << " ";
    //result += x_grad[i][j];
    //if(x_grad[i][j] > max){ max = x_grad[i][j];}
    //if(x_grad[i][j] < min){ min = x_grad[i][j];}
    //}
    //cout << endl;
  //}
  //int16_t **grad = combine_grad(x_grad,y_grad);
  float limit =70;
  Rect Rec(570, 270, 60, 100);
  
  cout << "make_regions" <<endl;
  //int normalization = 16;
  //int16_t **x_blur =calculate_blur(image,G_BLUR_KERNEL, normalization); 
  //make_regions(grad, limit);
  make_regions(x_grad, limit);
  //make_regions(y_grad, limit);
  //make_regions(x_blur, limit);
  cout << "maximal element was:" << max << endl << "minimal element was: " << min << endl << "average is: " << result/(HEIGHT*WIDTH) << endl;
  Mat grey_image(HEIGHT, WIDTH, CV_8U, im_array);

  rectangle(grey_image, Rec, Scalar(255), 1, 8, 0);
  //Canny(grey_image, grey_image, 10, 100, 3, true);
  imshow("array", grey_image);
  waitKey(0);
  free_2d(x_grad);
  free_2d(y_grad);
  return 0;
}




