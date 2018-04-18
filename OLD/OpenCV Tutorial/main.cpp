//
//  main.cpp
//  OpenCV Tutorial
//
//  Created by Didi Chi on 28/09/2016.
//  Copyright © 2016 dako. All rights reserved.
//

#include "findCircleParameter.hpp"
using namespace cv;
using namespace std;



typedef struct correctParameters
{
    Mat imgOrg;
    Point2i center;
    int radius;
    double w_longtitude;
    double w_latitude;
    distMapMode distMap ;
    double theta_left ;
    double phi_up ;
    double camerFieldAngle ;
    camMode camProjMode;
    CorrectType typeOfCorrect;
}correctParams;


Mat latitudeCorrection(Mat imgOrg, Point2i center, int radius, double camerFiledAngle, CorrectType type){
    //Mat resultImg;
    if(!(camerFiledAngle > 0 && camerFiledAngle <= PI)){
        cout << "The parameter \"camerFieldAngle\" must be in the interval (0,PI]." << endl;
        return Mat();
    }
    double rateOfWindow = 0.9;
    int width = imgOrg.size().width*rateOfWindow;
    int height = width;
    Size imgSize(width,height);
    Mat retImg(imgSize, CV_8UC3, Scalar(0,0,0));
    
    double dx  = camerFiledAngle/imgSize.width;
    double dy = dx;
    
    double latitude;
    double longitude;
    
    double x,y,z,r;
    double Theta_sphere;
    double Phi_sphere;
    double p;
    double theta;
    double x_cart, y_cart;
    int u,v;
    int u_latitude,v_latitude;
    
    double longitude_offset, latitude_offset;
    longitude_offset = (PI - camerFiledAngle)/2;
    latitude_offset = (PI - camerFiledAngle)/2;
    Mat_<Vec3b> _retImg = retImg;
    Mat_<Vec3b> _imgOrg = imgOrg;
    switch(type){
        case Forward:
            int left, top;
            left = center.x - radius;
            top =  center.y - radius;
            for(int j = top; j < top + 2 * radius;j++){
                for(int i = left; i < left + 2 * radius; i++){
                    if(pow(i-center.x,2) + pow(j-center.y,2) > pow(radius,2)){
                        continue;
                    }
                    u = i;
                    v = j;
                    double R = radius/sin(camerFiledAngle/2);
                    
                    //convert to catiesian coordinate
                    
                    x_cart = (u-center.x)/R;
                    y_cart = -(v - center.y)/R;
                    theta = cvFastArctan(y_cart, x_cart)*PI/180;
                    p = sqrt(pow(x_cart ,2) + pow(y_cart,2));
                    
                    Theta_sphere = asin(p);
                    Phi_sphere = theta;
                    
                    x = sin(Theta_sphere) * cos(Phi_sphere);
                    y = sin(Theta_sphere) * sin(Phi_sphere);
                    z = cos(Theta_sphere);
                    
                    latitude = acos(y);
                    longitude = cvFastArctan(z,-x)*PI /180;
                    
                    u_latitude = ((longitude -longitude_offset)/dx);
                    v_latitude = ((latitude-latitude_offset)/dy);
                    if (u_latitude < 0 || u_latitude >= imgSize.height || v_latitude < 0 || v_latitude >= imgSize.width)
                        continue;
                    
                    //perform the map from the origin image to the latitude map image
                    _retImg(v_latitude, u_latitude)[0] = _imgOrg(j, i)[0];
                    _retImg(v_latitude, u_latitude)[1] = _imgOrg(j, i)[1];
                    _retImg(v_latitude, u_latitude)[2] = _imgOrg(j, i)[2];
                }
            }
            break;
        case Reverse:
            for(int j = 0; j < imgSize.height;j++){
                latitude = latitude_offset + j*dy;
                for(int i = 0; i < imgSize.width;i++){
                    longitude = longitude_offset + i * dx;
                    x = -sin(latitude)*cos(longitude);
                    y = cos(latitude);
                    z = sin(latitude)*sin(longitude);
                    
                    Theta_sphere = acos(z);
                    Phi_sphere = cvFastArctan(y,x);
                    Phi_sphere = Phi_sphere*PI/180;
                    p = sin(Theta_sphere);
                    theta = Phi_sphere;
                    
                    x_cart = p*cos(theta);
                    y_cart = p*sin(theta);
                    double R = radius;
                    u = x_cart * R + center.x;
                    v = -y_cart * R + center.y;
                    _retImg.at<Vec3b>(j,i) = _imgOrg.at<Vec3b>(v,u);
                    
                }
            }
            break;
    }
   imshow("Corrected Image", retImg);
    return retImg;
}
Mat latitudeCorrection2(Mat imgOrg, Point2i center, int radius, distMapMode distMap, double camerFieldAngle, camMode camProjMode)
{
    if (!(camerFieldAngle > 0 && camerFieldAngle <= PI))
    {
        cout << "The parameter \"camerFieldAngle\" must be in the interval (0,PI]." << endl;
        return Mat();
    }
    double rateOfWindow = 0.9;
    //int width = imgOrg.size().width*rateOfWindow;
    //int height = width;
    
    int width = max(imgOrg.cols, imgOrg.rows);
    int height = width;
    
    
    Size imgSize(width, height);
    int center_x = imgSize.width / 2;
    int center_y = imgSize.height / 2;
    
    Mat retImg(imgSize, CV_8UC3, Scalar(0, 0, 0));
    
    double dx = camerFieldAngle / imgSize.width;
    double dy = camerFieldAngle / imgSize.height;
    
    //coordinate for latitude map
    double latitude;
    double longitude;
    
    //unity sphere coordinate
    double x, y, z, r;
    
    //parameter cooradinate of sphere coordinate
    double Theta_sphere;
    double Phi_sphere;
    
    //polar cooradinate for fish-eye Image
    double p;
    double theta;
    
    //cartesian coordinate
    double x_cart, y_cart;
    
    //Image cooradinate of imgOrg
    double u, v;
    Point pt, pt1, pt2, pt3, pt4;
    
    //Image cooradinate of imgRet
    int u_latitude, v_latitude;
    Rect imgArea(0, 0, imgOrg.cols, imgOrg.rows);
    
    //offset of imgRet Origin
    double longitude_offset, latitude_offset;
    longitude_offset = (PI - camerFieldAngle) / 2;
    latitude_offset = (PI - camerFieldAngle) / 2;
    
    double foval = 0.0;//焦距
    
    
    Mat_<Vec3b> _retImg = retImg;
    Mat_<Vec3b> _imgOrg = imgOrg;
    
    //according to the camera type to do the calibration
    for (int j = 0; j < imgSize.height; j++)
    {
        
        for (int i = 0; i < imgSize.width; i++)
        {
            Point3f tmpPt(i - center_x, center_y - j, 100);//最后一个参数用来修改成像面的焦距
            double normPt = norm(tmpPt);
            
            switch (distMap)
            {
                case PERSPECTIVE:
                    
                    tmpPt.x /= normPt;
                    tmpPt.y /= normPt;
                    tmpPt.z /= normPt;
                    x = tmpPt.x;
                    y = tmpPt.y;
                    z = tmpPt.z;
                    
                    break;
                case LATITUDE_LONGTITUDE:
                    
                    //latitude = latitude_offset + atan((double)j/(double)(imgSize.height-j));
                    //longitude = longitude_offset + i*dx;
                    
                    latitude = latitude_offset + j*dy;
                    longitude = longitude_offset + i*dx;
                    //Convert from latitude cooradinate to the sphere cooradinate
                    x = -sin(latitude)*cos(longitude);
                    y = cos(latitude);
                    z = sin(latitude)*sin(longitude);
                    
                    break;
                default:
                    break;
            }
            
            //Convert from unit sphere cooradinate to the parameter sphere cooradinate
            Theta_sphere = acos(z);
            Phi_sphere = cvFastArctan(y, x);//return value in Angle
            Phi_sphere = Phi_sphere*PI / 180;//Convert from Angle to Radian
            
            
            switch (camProjMode)
            {
                case STEREOGRAPHIC:
                    foval = radius / (2 * tan(camerFieldAngle / 4));
                    p = 2 * foval*tan(Theta_sphere / 2);
                    break;
                case EQUIDISTANCE:
                    foval = radius / (camerFieldAngle / 2);
                    p = foval*Theta_sphere;
                    break;
                case EQUISOLID:
                    foval = radius / (2 * sin(camerFieldAngle / 4));
                    p = 2 * foval*sin(Theta_sphere / 2);
                    break;
                case ORTHOGONAL:
                    foval = radius / sin(camerFieldAngle / 2);
                    p = foval*sin(Theta_sphere);
                    break;
                default:
                    cout << "The camera mode hasn't been choose!" << endl;
            }
            //Convert from parameter sphere cooradinate to fish-eye polar cooradinate
            //p = sin(Theta_sphere);
            theta = Phi_sphere;
            
            //Convert from fish-eye polar cooradinate to cartesian cooradinate
            x_cart = p*cos(theta);
            y_cart = p*sin(theta);
            
            //double R = radius / sin(camerFieldAngle / 2);
            
            //Convert from cartesian cooradinate to image cooradinate
            u = x_cart + center.x;
            v = -y_cart + center.y;
            
            pt = Point(u, v);
            
            if (!pt.inside(imgArea))
            {
                continue;
            }
            
            _retImg.at<Vec3b>(j, i) = _imgOrg.at<Vec3b>(pt);
            
        }
    }
    
    //imshow("org", _imgOrg);
    //imshow("ret", _retImg);
    //waitKey();
#ifdef _DEBUG_
    namedWindow("Corrected Image", CV_WINDOW_AUTOSIZE);
    imshow("Corrected Image", retImg);
    waitKey();
#endif
    //imwrite("ret.jpg", retImg);
    return retImg;
}
double func(double l, double phi)
{
    double result = (l / 2 - 1)*cos(phi) - sin(phi) + 1;
    return result;
}
double getPhi(double l)
{
    double head = 0;
    double tail = 0;
    double mid = 0;
    double result = 0;
    if (l >= 0 && l < 2)
    {
        head = 0;
        tail = PI / 2;
        mid = head;
        result = func(l, mid);
        while (abs(result)>LIMIT)
        {
            mid = (tail + head) / 2;
            result = func(l, mid);
            
            if (result > 0)
            {
                head = mid;
            }
            else
            {
                tail = mid;
            }
        }
    }
    else
    {
        head = PI / 2;
        tail = PI;
        mid = tail;
        result = func(l, mid);
        while (abs(result) > LIMIT)
        {
            mid = (tail + head) / 2;
            result = func(l, mid);
            if (result < 0)
            {
                head = mid;
            }
            else
            {
                tail = mid;
            }
        }
    }
    return mid;
}

Mat latitudeCorrection3(Mat imgOrg, Point2i center, int radius, distMapMode distMap, double theta_left, double phi_up, double camerFieldAngle, camMode camProjMode)
{
    if (!(camerFieldAngle > 0 && camerFieldAngle <= PI))
    {
        cout << "The parameter \"camerFieldAngle\" must be in the interval (0,PI]." << endl;
        return Mat();
    }
    double rateOfWindow = 0.9;
    
    //int width = imgOrg.size().width*rateOfWindow;
    //int height = width;
    
    int width = max(imgOrg.cols, imgOrg.rows);
    int height = width;
    //int height = imgOrg.rows;
    
    
    Size imgSize(width, height);
    int center_x = imgSize.width / 2;
    int center_y = imgSize.height / 2;
    
    Mat retImg(imgSize, CV_8UC3, Scalar(0, 0, 0));
    
    double dx = camerFieldAngle / imgSize.width;
    double dy = camerFieldAngle / imgSize.height;
    
    //coordinate for latitude map
    double latitude;
    double longitude;
    
    //unity sphere coordinate
    double x, y, z, r;
    
    //parameter cooradinate of sphere coordinate
    double Theta_sphere;
    double Phi_sphere;
    
    //polar cooradinate for fish-eye Image
    double p;
    double theta;
    
    //cartesian coordinate
    double x_cart, y_cart;
    
    //Image cooradinate of imgOrg
    double u, v;
    Point pt, pt1, pt2, pt3, pt4;
    
    //Image cooradinate of imgRet
    int u_latitude, v_latitude;
    Rect imgArea(0, 0, imgOrg.cols, imgOrg.rows);
    
    //offset of imgRet Origin
    double longitude_offset, latitude_offset;
    longitude_offset = (PI - camerFieldAngle) / 2;
    latitude_offset = (PI - camerFieldAngle) / 2;
    
    double foval = 0.0;//焦距
    
    
    Mat_<Vec3b> _retImg = retImg;
    Mat_<Vec3b> _imgOrg = imgOrg;
    
    //according to the camera type to do the calibration
    for (int j = 0; j < imgSize.height; j++)
    {
        
        for (int i = 0; i < imgSize.width; i++)
        {
            Point3f tmpPt(i - center_x, center_y - j, 600);//最后一个参数用来修改成像面的焦距
            double normPt = norm(tmpPt);
            
            switch (distMap)
            {
                case PERSPECTIVE:
                    
                    tmpPt.x /= normPt;
                    tmpPt.y /= normPt;
                    tmpPt.z /= normPt;
                    
                    x = tmpPt.x;
                    y = tmpPt.y;
                    z = tmpPt.z;
                    
                    break;
                case LATITUDE_LONGTITUDE:
                    
                    //latitude = latitude_offset + j*dy;
                    
                    latitude = getPhi((double)j*4.0 / imgSize.height);
                    
                    longitude = getPhi((double)i * 4 / imgSize.width);
                    
                    //latitude = latitude_offset + j*dy;
                    //longitude = longitude_offset + i*dx;
                    //Convert from latitude cooradinate to the sphere cooradinate
                    x = -sin(latitude)*cos(longitude);
                    y = cos(latitude);
                    z = sin(latitude)*sin(longitude);
                    
                    break;
                default:
                    break;
            }
            
            if (distMap == PERSPECTIVE)
            {
                //double theta = PI/4;
                //double phi = -PI/2;
                cv::Mat curPt(cv::Point3f(x, y, z));
                std::vector<cv::Point3f> pts;
                
                //向东旋转地球
                //pts.push_back(cv::Point3f(cos(theta), 0, -sin(theta)));
                //pts.push_back(cv::Point3f(0, 1, 0));
                //pts.push_back(cv::Point3f(sin(theta), 0, cos(theta)));
                
                //向南旋转地球
                //pts.push_back(cv::Point3f(1, 0, 0));
                //pts.push_back(cv::Point3f(0, cos(phi), sin(phi)));
                //pts.push_back(cv::Point3f(0, -sin(phi), cos(phi)));
                
                //两个方向旋转
                pts.push_back(cv::Point3f(cos(theta_left), 0, sin(theta_left)));
                pts.push_back(cv::Point3f(sin(phi_up)*sin(theta_left), cos(phi_up), -sin(phi_up)*cos(theta_left)));
                pts.push_back(cv::Point3f(-cos(phi_up)*sin(theta_left), sin(phi_up), cos(phi_up)*cos(theta_left)));
                
                
                cv::Mat revert = cv::Mat(pts).reshape(1).t();
                
                cv::Mat changed(revert*curPt);
                
                Mat_<double> changed_double;
                changed.convertTo(changed_double, CV_64F);
                
                x = changed_double.at<double>(0, 0);
                y = changed_double.at<double>(1, 0);
                z = changed_double.at<double>(2, 0);
                
                //std::cout << curPt << std::endl
                //	<<revert<<std::endl;
            }
            
            //Convert from unit sphere cooradinate to the parameter sphere cooradinate
            Theta_sphere = acos(z);
            Phi_sphere = cvFastArctan(y, x);//return value in Angle
            Phi_sphere = Phi_sphere*PI / 180;//Convert from Angle to Radian
            
            
            switch (camProjMode)
            {
                case STEREOGRAPHIC:
                    foval = radius / (2 * tan(camerFieldAngle / 4));
                    p = 2 * foval*tan(Theta_sphere / 2);
                    break;
                case EQUIDISTANCE:
                    foval = radius / (camerFieldAngle / 2);
                    p = foval*Theta_sphere;
                    break;
                case EQUISOLID:
                    foval = radius / (2 * sin(camerFieldAngle / 4));
                    p = 2 * foval*sin(Theta_sphere / 2);
                    break;
                case ORTHOGONAL:
                    foval = radius / sin(camerFieldAngle / 2);
                    p = foval*sin(Theta_sphere);
                    break;
                default:
                    cout << "The camera mode hasn't been choose!" << endl;
            }
            //Convert from parameter sphere cooradinate to fish-eye polar cooradinate
            //p = sin(Theta_sphere);
            theta = Phi_sphere;
            
            //Convert from fish-eye polar cooradinate to cartesian cooradinate
            x_cart = p*cos(theta);
            y_cart = p*sin(theta);
            
            //double R = radius / sin(camerFieldAngle / 2);
            
            //Convert from cartesian cooradinate to image cooradinate
            u = x_cart + center.x;
            v = -y_cart + center.y;
            
            pt = Point(u, v);
            
            if (!pt.inside(imgArea))
            {
                continue;
            }
            
            _retImg.at<Vec3b>(j, i) = _imgOrg.at<Vec3b>(pt);
            
        }
    }
    
    //imshow("org", _imgOrg);
    //imshow("ret", _retImg);
    //waitKey();
#ifdef _DEBUG_
    namedWindow("Corrected Image", CV_WINDOW_AUTOSIZE);
    imshow("Corrected Image", retImg);
    waitKey();
#endif
    //imwrite("ret.jpg", retImg);
    return retImg;
}
/** @function main */
int main( int argc, char** argv )
{
   
    
    // Load the images
   Mat img1= imread( "my_photo-2.jpg" );
   Mat img2= imread( "my_photo-1.jpg");
   
    //uncomment the following lines if you want to find the parameters of the photo by the findCircleParameter function
  /*  bool correctionstarts = findCircleParameter::init(img1);
    if(correctionstarts){
       findCircleParameter::findCircle();
    }*/
    // setting up the correction parameters
    correctParams param;
    param.imgOrg = img1;
     //uncomment the following line if you want to find the parameters of the photo by the findCircleParameter function
   // findCircleParameter::getCircleParatemer(param.center, param.radius);
    param.center.x = 240;
    param.center.y = 320;
    param.radius = 340;
    param.w_longtitude = PI/2;
    param.w_latitude = PI/2;
    param.distMap = LATITUDE_LONGTITUDE;
    param.theta_left = 0;
    param.phi_up = 0;
    param.camerFieldAngle = findCircleParameter::FOV;
    param.camProjMode = EQUIDISTANCE;
    param.typeOfCorrect = Reverse;
   
    cout << endl
				<< "Correct Image(s) with the same circular region params: " << endl
				<< "radius = " << param.radius << ", center = " << param.center << endl << endl;
    Mat image2 = latitudeCorrection(param.imgOrg, param.center, param.radius, param.camerFieldAngle, param.typeOfCorrect);
       param.imgOrg = img2;
    //uncomment the following lines if you want to find the parameters of the photo by the findCircleParameter function
   //findCircleParameter::findCircle();
   //findCircleParameter::getCircleParatemer(param.center, param.radius);
    Mat image1 = latitudeCorrection(param.imgOrg, param.center, param.radius, param.camerFieldAngle, param.typeOfCorrect);
   
    Rect myRoi1(108,62,215,320);

    
    image1 = image1(myRoi1);
    image2 = image2(myRoi1);
    imwrite("result_test1.jpg",image1);
    imwrite("result_test2.jpg",image2);

    Mat gray_image1;
    Mat gray_image2;
    // Convert to Grayscale
    cvtColor( image1, gray_image1, CV_RGB2GRAY );
   cvtColor( image2, gray_image2, CV_RGB2GRAY );
    
    if( !gray_image1.data || !gray_image2.data )
    { std::cout<< " --(!) Error reading images " << std::endl; return -1; }
    
    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;
    
    SurfFeatureDetector detector( minHessian );
    
    std::vector< KeyPoint > keypoints_object, keypoints_scene;
    
    detector.detect( gray_image1, keypoints_object );
    detector.detect( gray_image2, keypoints_scene );
    
    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;
    
    Mat descriptors_object, descriptors_scene;
    
    extractor.compute( gray_image1, keypoints_object, descriptors_object );
    extractor.compute( gray_image2, keypoints_scene, descriptors_scene );
    
    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_object, descriptors_scene, matches );
    
    double max_dist = 0; double min_dist = 100;
    
    //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < descriptors_object.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }
    
    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );
    double largest1 = 0;
    double largest2 = 10000;
    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;
       for( int i = 0; i < descriptors_object.rows; i++ ){
        
        if( matches[i].distance < 3*min_dist  ){
            
            float slope = (keypoints_object[matches[i].queryIdx].pt.y - keypoints_scene[matches[i].trainIdx].pt.y)/(image1.cols - keypoints_object[matches[i].queryIdx].pt.x + keypoints_scene[matches[i].trainIdx].pt.x);
            //finding the keypoint with largest x value on pic 1 and finding the one with smallest x value on pic 2
           
            if(std::abs(slope) < 0.015){
                if(keypoints_object[matches[i].queryIdx].pt.x < largest2){
                    largest2 = keypoints_object[matches[i].queryIdx].pt.x;
                   // largest1 = i;
                }
                if(keypoints_scene[matches[i].trainIdx].pt.x > largest1){
                    largest1 = keypoints_scene[matches[i].trainIdx].pt.x;
                    //largest2 = i;
                }
                
                // the coordinates of the keypoints
                cout << "points on the first picture:x,y ";
                cout << keypoints_object[matches[i].queryIdx].pt.x;
                cout << " ";
                cout << keypoints_object[matches[i].queryIdx].pt.y << endl;
                
                cout << "points on the second picture: x y ";
                cout << keypoints_scene[matches[i].trainIdx].pt.x ;
                cout << " ";
                cout << keypoints_scene[matches[i].trainIdx].pt.y << endl;
               
                good_matches.push_back( matches[i]);
            }
        }
        //good_matches_keypoints.push_back(keypoints_object[matches[i].queryIdx]);
    }
    printf("image size height %d, width %d\n",image1.rows,image1.cols);
    printf("the largest on image2 is %f\n",largest1);
    printf("the largest on image1 is %f\n",largest2);
    Mat img_key;
    string output1 = "with_keys.jpg";
    drawMatches(image1, keypoints_object, image2, keypoints_scene, good_matches, img_key);
    
    imwrite(output1, img_key);

    std::vector< Point2f > obj;
    std::vector< Point2f > scene;
    
    
    for( int i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }
    int width = largest1 + image2.cols - largest2;
    printf("the width of the final image is %d\n",width);
    Mat combine(max(image1.size().height,image2.size().height),width,CV_8UC3);
    Mat left_roi(combine,Rect(0,0,largest1,image2.size().height));
    Mat croppedImage1;
    Rect Roi1(0,0,largest1,image2.rows);
    croppedImage1 = image2(Roi1);
    croppedImage1.copyTo(left_roi);
    Mat right_roi(combine,Rect(largest1,0,image1.cols-largest2,image1.rows));
    Mat croppedImage2;
    Rect Roi2(largest2,0,image1.cols-largest2,image1.rows);
    croppedImage2 = image1(Roi2);
    croppedImage2.copyTo(right_roi);
    imshow("combine",combine);
    imwrite("combined.jpg",combine);
    // Find the Homography Matrix
    Mat H = findHomography( obj, scene, CV_RANSAC );
    // Use the Homography Matrix to warp the images
    cv::Mat result;
    warpPerspective(image1,result,H,cv::Size(image1.cols+image2.cols,image1.rows));
    //warpPerspective(resultImage,result,H,cv::Size(resultImage.cols+resultImage2.cols,resultImage.rows));
    cv::Mat half(result,cv::Rect(0,0,image2.cols,image2.rows));
    
    //cv::Mat half(result,cv::Rect(0,0,resultImage2.cols,resultImage2.rows));
    image2.copyTo(half);
    string result_name = "result.jpg";
    imshow( "Result", result );
    imwrite(result_name, result);
        waitKey(0);
    return 0;
}

/** @function readme */
