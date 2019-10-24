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
  return 0;
}




