#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include<bits/stdc++.h>
using namespace cv;
using namespace std;
Mat img1 = imread("left_image.jpg",0);
Mat img2 = imread("right_image.jpg",0);
map<pair<int,int>,double> mp;
double dist(int sx,int sy,int dx,int dy){
  return sqrt((sx-dx)*(sx-dx) + (sy-dy)*(sy-dy));
}
int main()
{
  int rows = img1.rows, cols = img1.cols;
  int arr[img1.rows+1][img1.cols+1];
  int x,y,i,j,k,l;
  
  for( x=0;x<img1.rows;x++)
  {
    for( y=0;y<img1.cols;y++)
    {
        arr[x][y] = 0;
    }
  }
  cout<<"a"<<endl;
  for( i=0;i<rows;++i)
  {
    for( j=0;j<cols;++j)
    {
      // store the pixel color of (i,j)
      bool flag=false;
      int steps;
      for( steps=10;flag!=true && steps < 300;steps+=10)
      {
        int left = max(0,j-steps);
        int right= min(cols,j+steps);
        int up = max(0,i-steps);
        int down = min(rows,i+rows);
        cout<<"b"<<endl;
        for( k=up;k<down;++k)
        {
          for( l=left;l<right;++l)
          {
            //check if pixel color of (i,j) & (k,l)
            // is same
            //if yes then do following
            if(img1.at<uchar>(i,j) == img2.at<uchar>(k,l))
            {
              pair<int,int> p = make_pair(i,j);
              arr[i][j] = (int)dist(i,j,k,l);
              flag=true;
              cout<<"c"<<endl;
              break;
            }
          }
          if(flag)
          {
            break;
          }
        }
        if(flag)
        {
          break;
        }
      }
    }
  }
  //create a new image
  Mat img3(img1.rows,img1.cols,CV_8UC1,Scalar(0));
  double max =0,min =1234;
  for(int x=0;x<img1.rows;x++)
  {
    for(int y=0;y<img1.cols;y++)
    {
      if(arr[x][y] > max)
      max = arr[x][y];
      if(arr[x][y] < min)
      min = arr[x][y];
      cout<<"d"<<endl;
    }
  }
  for(x=0;x<img1.rows;x++)
  {
    for(y=0;y<img1.cols;y++)
    {
      arr[x][y] = (int)( 255 * ((double)arr[x][y] / (max-min)));
      img3.at<uchar>(x,y) = arr[x][y];
      cout<<"e"<<endl;
    }
  }
  cout<<"f"<<endl;
  namedWindow("win",WINDOW_NORMAL);
  imshow("win", img3);
  waitKey(0);
}
