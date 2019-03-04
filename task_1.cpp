#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/core/core.hpp"
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
VideoCapture vid("task1_vid.mp4",1);
Mat img;
Mat img1(1080,1920,CV_8UC1,Scalar(0));
// N is just greater than maximum number of points
const int N=(int)1e6+1;
// number of steps to move at once
const double STEP = 6.0;
class point
{
  public:
  point() { }
  point(int x,int y)
  {
    this->x = x;
    this->y = y;
  }
  bool operator==(const point& p){
    return this->x == p.x && this->y == p.y;
  }
  int x, y;
};
// list of vertices
point v[N];
// 2 - D array to keep track of visited points
int visited[2001][2001];
// function to find distance between 2 points
double dist(point s,point d)
{
  // Euclidean distance
  return sqrt((d.x-s.x)*(d.x-s.x) + (d.y-s.y)*(d.y-s.y));
}
// Move from s point towards d point by 6 units
point move(point s,point d)
{
  double theta=0;
  if(s.y==d.y)
  {
    theta=90 * 3.14 / 180;
  }
  else if(s.x != d.x)
  {
    theta = atan((s.y-d.y)/(s.x-d.x));
  }
  int xx,yy;
  // go to this point only if distance is less than STEP size
  if(dist(s,d)<=STEP)
  {
    xx = d.x;
    yy = d.y;
  }
  else
  {
    xx = STEP * cos(theta);
    yy = STEP * sin(theta);
  }
  point p(xx,yy);
  // Check if the point is valid or visited before
  if(xx<0 || yy <0 ||visited[xx][yy])
  {
    p.x=-1;
    p.y=-1;
    return p;
  }
  p.x=xx;
  p.y=yy;
  return p;
}
int main()
{
  while(vid.isOpened())
  {
    vid>>img;
    // pixel size
    int pxz = img.rows, pyz = img.cols;
    int i,j;
    // st is the starting point
    point st;
    bool f = false;
    for(i = 0; i < img.rows; ++i)
    {
      for(j =0 ;j < img.cols; ++j)
      {
        if(img.at<Vec3b>(i,j)[2] >= 125)
        {
          st.x = i;
          st.y = j;
          f = true;
          break;
        }
      }
      // break if the starting point is reached
      if(f)
      {
        break;
      }
    }
    // mark starting point as visited
    visited[st.x][st.y]=1;
    v[1] = st;
    // vertex_count is the number of vertices in growing tree
    int vertex_count=1;
    // find all obstacle including red colors
    for(i = 0; i < img.rows; ++i)
    {
      for(j =0 ;j < img.cols; ++j)
      {
        if((img.at<Vec3b>(i,j)[2] >= 125) || ((img.at<Vec3b>(i,j)[0] <= 50)&&
        (img.at<Vec3b>(i,j)[1] <= 50)&&(img.at<Vec3b>(i,j)[2] <= 50))||
        (img.at<Vec3b>(i,j)[0] >= 200) )
        {
          visited[i][j] = 1;
        }
      }
    }

    while(true)
    {
      // generate a random point
      int x=rand() % pxz;
      int y=rand() % pyz;
      // make sure it is not visited yet
      while(visited[x][y])
      {
        x = rand() % pxz;
        y = rand() % pyz;
      }
      double dis=123456789;
      int closest_point;
      point random_point(x,y);
      for(int i=1;i<=vertex_count;++i)
      {
        point s = v[i];
        if(dist(s,random_point)<dis)
        {
          dis=dist(s,random_point);
          closest_point=i;
        }
      }
      point s = v[closest_point];
      point p(-1,-1);
      point res = move(s,random_point);
      // check if possible to move or not
      if(res==p)
       {
         continue;
       }
      else
       {
        ++vertex_count;
        img1.at<uchar>(res.x,res.y) = 127;
        // save the point as new vertex
        v[vertex_count] = res;
        // mark it visited
        visited[res.x][res.y]=1;
        if(img.at<Vec3b>(res.x,res.y)[1] >= 150)
         {
           cout<<"target is realsed"<<endl;
           break;
         }
        }
     }
     waitKey(1000);
  }
  namedWindow("win1", WINDOW_NORMAL);
  imshow("win", img1);
  waitKey(0);
}
