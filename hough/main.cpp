#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;





/** @function main */
int main(int argc, char** argv)
{
  Mat src, src_HSV, src_gray;

  /// Read the image
  VideoCapture cap("vid.mp4");

  //if( !cap.data )
    //{ return -1; }

  int i = 0;

  for (i = 0; i < 1300; i++) {
      cap >> src;
  }

  while(1) {
      cap >> src;

  /// Convert it to HSV
  cvtColor(src, src_HSV, CV_BGR2HSV);

  /// Contrast
  ///for( int y = 0; y < src_HSV.rows; y++ )
  ///    { for( int x = 0; x < src_HSV.cols; x++ )
  ///         { for( int c = 0; c < 3; c++ )
  ///              {
  ///      src_HSV.at<Vec3b>(y,x)[c] =
  ///         saturate_cast<uchar>( 1.85*( src_HSV.at<Vec3b>(y,x)[c] ) + 50 );
  ///             }
  ///    }
  ///    }


  /// Filter for red
  Mat lower_red_hue_range;
  Mat upper_red_hue_range;
  inRange(src_HSV, Scalar(0, 100, 110), Scalar(50, 255, 255), lower_red_hue_range);
  inRange(src_HSV, Scalar(100, 8, 0), Scalar(180, 255, 255), upper_red_hue_range);

  /// Combine red images
  Mat red_hue_image;
  addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

  /// Filter for green
  Mat green_hue_image;
  inRange(src_HSV, Scalar(79, 199, 60), Scalar(82, 205, 70), green_hue_image);

  /// Filter for yellow
  Mat yellow_hue_image;
  inRange(src_HSV, Scalar(70, 150, 90), Scalar(80, 190, 170), yellow_hue_image);

  /// Combine all three
  Mat all_colour_image;
  Mat green_yellow;
  addWeighted(green_hue_image, 1.0, yellow_hue_image, 1.0, 0.0, green_yellow);
  addWeighted(red_hue_image, 1.0, green_yellow, 1.0, 0.0, all_colour_image);


  /// Convert it to gray
  /// cvtColor( src_HSV, src_gray, CV_BGR2GRAY );


  /// Reduce the noise so we avoid false circle detection
  /// GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
  /// GaussianBlur( all_colour_image, all_colour_image, Size(9, 9), 2, 2 );


  int dilation_type;
  int erosion_type;
  int erosion_elem = 0;
  int erosion_size = 10;
  int dilation_elem = 0;
  int dilation_size = 10;
  int const max_elem = 2;
  int const max_kernel_size = 21;


  Mat eroded;
  if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
  else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
  else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

  Mat element1 = getStructuringElement( erosion_type,
                                         Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                         Point( erosion_size, erosion_size ) );

  /// Apply the erosion operation
  erode( all_colour_image, eroded, element1 );


  Mat dilated;
  if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
  else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
  else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

  Mat element = getStructuringElement( dilation_type,
                                         Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
  /// Apply the dilation operation
  dilate( eroded, dilated, element );


  std::vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  /// HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/15, 200, 30, 0, 0);
  HoughCircles( dilated, circles, CV_HOUGH_GRADIENT, 1, dilated
                .rows/15, 200, 30, 0, 0);

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      // circle center
           circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
           // circle outline
           circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }

       /// Show your results
       namedWindow( "Hough Circle Transform Demo", CV_WINDOW_NORMAL );
       imshow( "Hough Circle Transform Demo", src);
       namedWindow("HSV Filter", CV_WINDOW_NORMAL );
       imshow( "HSV Filter", all_colour_image );
       namedWindow("Red", CV_WINDOW_NORMAL );
       imshow( "Red", red_hue_image );
       namedWindow("Green", CV_WINDOW_NORMAL );
       imshow( "Green", green_hue_image );
       namedWindow("Yellow", CV_WINDOW_NORMAL );
       imshow( "Yellow", yellow_hue_image );
       namedWindow("HSV", CV_WINDOW_NORMAL);
       imshow("HSV", src_HSV);
       namedWindow("dilate", CV_WINDOW_NORMAL);
       imshow("dilate", dilated);


       waitKey(1000/24);


  }
       return 0;

     }
