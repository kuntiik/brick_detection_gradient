#include <stdlib.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;


int main(int argc, char **argv){
  
  Mat image;
  FileStorage fs;
  if (argc<2) {
    fs.open("prob0000.xml", FileStorage::READ);
  } else {
    fs.open(argv[1], FileStorage::READ);
  }
  fs["depth"] >> image;
  image = image/25; 
  Size img_size = image.size();
  cout << "image dimensions are" <<"height" <<  img_size.height << "width" <<img_size.width << endl;
  cout << "Image = "<< endl << " "  << image << endl << endl;
  imshow("Example Canny", image);
  waitKey(0);
  return 0;
}




