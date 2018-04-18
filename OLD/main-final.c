#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

int main()
{
    IplImage* pFrame1 = NULL;
    IplImage* pFrame2 = NULL;
    IplImage* pFrame3 = NULL;
    IplImage* pFrame4 = NULL;
    IplImage* final=cvCreateImage(cvSize(1800,610), IPL_DEPTH_8U,3);

    int R=340,r=600;
    int i,j;

    double I1r=0,I1g=0,I1b=0,I2r=0,I2g=0,I2b=0,I12r=0,I12g=0,I12b=0,I23r=0,I23g=0,I23b=0,I34r=0,I34g=0,I34b=0;
    double I11r,I11g,I11b,I22r,I22g,I22b,I33r,I33g,I33b,I44r,I44g,I44b;

    int begin=50;
    int min1=409;
    int max1=439;
    int min2=48;
    int max2=78;
    int min3=423;
    int max3=453;
    int min4=47;
    int max4=77;
    int min5=409;
    int max5=439;
    int min6=23;
    int max6=53;
    int end=434;

    CvScalar s;
    CvScalar s1;
    CvScalar s2;

    CvCapture* pCapture1 = cvCreateCameraCapture(4);
    CvCapture* pCapture2 = cvCreateCameraCapture(2);
    CvCapture* pCapture3 = cvCreateCameraCapture(1);
    CvCapture* pCapture4 = cvCreateCameraCapture(0);

    cvSetCaptureProperty( pCapture1, CV_CAP_PROP_FRAME_WIDTH , 640 );
    cvSetCaptureProperty( pCapture1, CV_CAP_PROP_FRAME_HEIGHT , 480 );
    cvSetCaptureProperty( pCapture1, CV_CAP_PROP_FPS , 15 );

    cvSetCaptureProperty( pCapture2, CV_CAP_PROP_FRAME_WIDTH , 640 );
    cvSetCaptureProperty( pCapture2, CV_CAP_PROP_FRAME_HEIGHT , 480 );
    cvSetCaptureProperty( pCapture2, CV_CAP_PROP_FPS , 15 );

    cvSetCaptureProperty( pCapture3, CV_CAP_PROP_FRAME_WIDTH , 640 );
    cvSetCaptureProperty( pCapture3, CV_CAP_PROP_FRAME_HEIGHT , 480 );
    cvSetCaptureProperty( pCapture3, CV_CAP_PROP_FPS , 15 );

    cvSetCaptureProperty( pCapture4, CV_CAP_PROP_FRAME_WIDTH , 640 );
    cvSetCaptureProperty( pCapture4, CV_CAP_PROP_FRAME_HEIGHT , 480 );
    cvSetCaptureProperty( pCapture4, CV_CAP_PROP_FPS , 15 );

    cvNamedWindow("Panoramic Video", 0);

    while(1)
    {
        pFrame1=cvQueryFrame( pCapture1 );
        pFrame2=cvQueryFrame( pCapture2 );
        pFrame3=cvQueryFrame( pCapture3 );
        pFrame4=cvQueryFrame( pCapture4 );

            for (i=0;i<610;i++){
                for (j=min1+10;j<max1-10;j++){
                    int j1=639-i-11;
                    int i1=(j-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int j2=639-i-20;
                    int i2=((j-min1+min2)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                        s=cvGet2D(pFrame1,i1,j1);
                        s1=cvGet2D(pFrame2,i2,j2);
                        I1r=I1r+s.val[0];
                        I1g=I1g+s.val[1];
                        I1b=I1b+s.val[2];
                        I2r=I2r+s1.val[0];
                        I2g=I2g+s1.val[1];
                        I2b=I2b+s1.val[2];
                    }
                }
                I12r=I1r/I2r;
                I12g=I1g/I2g;
                I12b=I1b/I2b;
                I1r=0;
                I1g=0;
                I1b=0;
                I2r=0;
                I2g=0;
                I2b=0;
            for (i=0;i<610;i++){
                for (j=min3+10;j<max3-10;j++){
                    int j1=639-i-20;
                    int j2=639-i-16;
                    int i1=(j-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int i2=((j-min3+min4)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                        s=cvGet2D(pFrame2,i1,j1);
                        s1=cvGet2D(pFrame3,i2,j2);
                        I1r=I1r+s.val[0];
                        I1g=I1g+s.val[1];
                        I1b=I1b+s.val[2];
                        I2r=I2r+s1.val[0];
                        I2g=I2g+s1.val[1];
                        I2b=I2b+s1.val[2];
                    }
                }
                I23r=I1r/I2r;
                I23g=I1g/I2g;
                I23b=I1b/I2b;

                I1r=0;
                I1g=0;
                I1b=0;
                I2r=0;
                I2g=0;
                I2b=0;
            for (i=0;i<610;i++){
                for (j=min5+10;j<max5-10;j++){
                    int j1=639-i-16;
                    int j2=639-i;
                    int i1=(j-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int i2=((j-min5+min6)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                        s=cvGet2D(pFrame3,i1,j1);
                        s1=cvGet2D(pFrame4,i2,j2);
                        I1r=I1r+s.val[0];
                        I1g=I1g+s.val[1];
                        I1b=I1b+s.val[2];
                        I2r=I2r+s1.val[0];
                        I2g=I2g+s1.val[1];
                        I2b=I2b+s1.val[2];
                    }
                }
                I34r=I1r/I2r;
                I34g=I1g/I2g;
                I34b=I1b/I2b;

                if (I12r>=1 && I12r*I23r>=1 && I12r*I23r*I34r>=1){
                    I11r=1;
                    I11g=1;
                    I11b=1;
                    I22r=I12r;
                    I22g=I12g;
                    I22b=I12b;
                    I33r=I12r*I23r;
                    I33g=I12g*I23g;
                    I33b=I12b*I23b;
                    I44r=I12r*I23r*I34r;
                    I44g=I12g*I23g*I34g;
                    I44b=I12b*I23b*I34b;
                }
                else if (I12r<1 && I23r>=1 && I23r*I34r>=1){
                    I11r=1/I12r;
                    I11g=1/I12g;
                    I11b=1/I12b;
                    I22r=1;
                    I22g=1;
                    I22b=1;
                    I33r=I23r;
                    I33g=I23g;
                    I33b=I23b;
                    I44r=I23r*I34r;
                    I44g=I23g*I34g;
                    I44b=I23b*I34b;
                }
                else if (I34r>=1 && I23r<1 && I12r*I23r<1){
                    I11r=1/I12r/I23r;
                    I11g=1/I12g/I23g;
                    I11b=1/I12b/I23b;
                    I22r=1/I23r;
                    I22g=1/I23g;
                    I22b=1/I23b;
                    I33r=1;
                    I33g=1;
                    I33b=1;
                    I44r=I34r;
                    I44g=I34g;
                    I44b=I34b;
                }
                else {
                    I11r=1/I12r/I34r/I23r;
                    I11g=1/I12g/I34g/I23g;
                    I11b=1/I12b/I34b/I23b;
                    I22r=1/I34r/I23r;
                    I22g=1/I34g/I23g;
                    I22b=1/I34b/I23b;
                    I33r=1/I34r;
                    I33g=1/I34g;
                    I33b=1/I34b;
                    I44r=1;
                    I44g=1;
                    I44b=1;
                }

            if(!pFrame1)break;

            for(i=0;i<610;i++){
                for(j=begin;j<min1;j++){
                    int j1=639-i-11;
                    int i1=(j-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    s=cvGet2D(pFrame1,i1,j1);
                    s.val[0]=I11r*s.val[0];
                    s.val[1]=I11g*s.val[1];
                    s.val[2]=I11b*s.val[2];
                    cvSet2D(final,i,j,s);
                }
            }

            for(i=0;i<610;i++){
                for(j=min1;j<max1;j++){
                    int j1=639-i-11;
                    int j2=639-i-20;
                    int i1=(j-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int i2=((j-min1+min2)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                    double a=j-min1;
                    double b=max1-j;
                    double c=max1-min1;
                    s=cvGet2D(pFrame1,i1,j1);
                    s1=cvGet2D(pFrame2,i2,j2);
                    s2.val[0]=a/c*I22r*s1.val[0]+b/c*I11r*s.val[0];
                    s2.val[1]=a/c*I22g*s1.val[1]+b/c*I11g*s.val[1];
                    s2.val[2]=a/c*I22b*s1.val[2]+b/c*I11b*s.val[2];

                    cvSet2D(final,i,j,s2);
                }
            }
            for(i=0;i<610;i++){
                for(j=max1;j<min3+max1-max2;j++){
                    int j1=639-i-20;
                    int i1=((j-max1+max2)-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    s=cvGet2D(pFrame2,i1,j1);
                    s.val[0]=I22r*s.val[0];
                    s.val[1]=I22g*s.val[1];
                    s.val[2]=I22b*s.val[2];
                    cvSet2D(final,i,j,s);
                }
            }
            for(i=0;i<610;i++){
                for(j=min3+max1-max2;j<max3+max1-max2;j++){
                    int j1=639-i-20;
                    int j2=639-i-16;
                    int i1=((j-max1+max2)-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int i2=((j-max1+max2-min3+min4)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                    double a=j-min3-max1+max2;
                    double b=max3+max1-max2-j;
                    double c=max3-min3;
                    s=cvGet2D(pFrame2,i1,j1);
                    s1=cvGet2D(pFrame3,i2,j2);
                    s2.val[0]=a/c*I33r*s1.val[0]+b/c*I22r*s.val[0];
                    s2.val[1]=a/c*I33g*s1.val[1]+b/c*I22g*s.val[1];
                    s2.val[2]=a/c*I33b*s1.val[2]+b/c*I22b*s.val[2];
                    s2.val[3]=a/c*s1.val[3]+b/c*s.val[3];
                    cvSet2D(final,i,j,s2);
                }
            }
            for(i=0;i<610;i++){
                for(j=max3+max1-max2;j<min5+max3+max1-max2-max4;j++){
                    int j1=639-i-16;
                    int i1=((j-max1+max2-max3+max4)-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    s=cvGet2D(pFrame3,i1,j1);
                    s.val[0]=I33r*s.val[0];
                    s.val[1]=I33g*s.val[1];
                    s.val[2]=I33b*s.val[2];
                    cvSet2D(final,i,j,s);
                }
            }
            for(i=0;i<610;i++){
                for(j=min5+max3+max1-max2-max4;j<max5+max3+max1-max2-max4;j++){
                    int j1=639-i-16;
                    int j2=639-i;
                    int i1=((j-max1-max3+max2+max4)-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    int i2=((j-max1-max3+max2+max4-min5+min6)-240)*sqrt(R*R-(j2-320)*(j2-320))/(R+r-sqrt(r*r-(j2-320)*(j2-320)))+240;
                    double a=j-max3-max1+max2+max4-min5;
                    double b=max5+max3+max1-max2-max4-j;
                    double c=max5-min5;
                    s=cvGet2D(pFrame3,i1,j1);
                    s1=cvGet2D(pFrame4,i2,j2);
                    s2.val[0]=a/c*I44r*s1.val[0]+b/c*I33r*s.val[0];
                    s2.val[1]=a/c*I44g*s1.val[1]+b/c*I33g*s.val[1];
                    s2.val[2]=a/c*I44b*s1.val[2]+b/c*I33b*s.val[2];
                    cvSet2D(final,i,j,s2);
                }
            }
            for(i=0;i<610;i++){
                for(j=max5+max3+max1-max2-max4;j<end-max6+max5-max4+max3-max2+max1;j++){
                    int j1=639-i;
                    int i1=((j-max1-max3+max2+max4-max5+max6)-240)*sqrt(R*R-(j1-320)*(j1-320))/(R+r-sqrt(r*r-(j1-320)*(j1-320)))+240;
                    s=cvGet2D(pFrame4,i1,j1);
                    s.val[0]=I44r*s.val[0];
                    s.val[1]=I44g*s.val[1];
                    s.val[2]=I44b*s.val[2];
                    cvSet2D(final,i,j,s);
                }
            }
            cvShowImage("Panoramic Video",final);

            char c=cvWaitKey(5);
            if(c==27)break;
        }
    cvReleaseCapture(&pCapture1);
    cvReleaseCapture(&pCapture2);
    cvReleaseCapture(&pCapture3);
    cvReleaseCapture(&pCapture4);
    cvDestroyWindow("Panoramic Video");

}
