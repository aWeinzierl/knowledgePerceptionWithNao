#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>
#include <json_prolog/prolog.h>
using namespace std;
using namespace cv;
using namespace aruco;
using namespace json_prolog;

class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
	CameraParameters TheCameraParameters;
	Prolog pl;

public:
	ImageConverter()
	: it_(nh_)
	{
		// Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/nao_robot/camera/top/camera/image_raw", 1, &ImageConverter::imageCb, this);

		Mat dist(1,5,CV_32FC1);
		dist.at<float>(0,0)=-0.066494;
		dist.at<float>(0,1)=0.095481;
		dist.at<float>(0,2)=-0.000279;
		dist.at<float>(0,3)=0.002292;
		dist.at<float>(0,4)=0.000000;
		Mat cameraP(3,3,CV_32FC1);

		cameraP.at<float>(0,0)=551.543059;
		cameraP.at<float>(0,1)=0.000000;
		cameraP.at<float>(0,2)=327.382898;
		cameraP.at<float>(1,0)=0.000000;
		cameraP.at<float>(1,1)=553.736023;
		cameraP.at<float>(1,2)=225.026380;
		cameraP.at<float>(2,0)=0.000000;
		cameraP.at<float>(2,1)=0.000000;
		cameraP.at<float>(2,2)=1.000000;

		TheCameraParameters.setParams(cameraP,dist,Size(640,480));
		TheCameraParameters.resize( Size(640,480));
	}

	~ImageConverter()
	{
	}

	string IntToStr(int a)
	{
		stringstream ss;
		ss << a;
		return ss.str();
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg)
	{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
			cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
			ROS_ERROR("cv_bridge exception: %s", e.what());
			return;
		}

		Mat InImage=cv_ptr->image;

        MarkerDetector mDetector;
        vector<Marker> markers;
        mDetector.detect(InImage, markers, TheCameraParameters);

        for(auto const& marker : markers){
            marker.draw(InImage, Scalar(0,0,255), 2);
        }


		imshow("markers",InImage);
		waitKey(10);
	}
};

int main(int argc, char** argv)
{
	ros::init(argc, argv, "tutorial_vision");
	ImageConverter ic;

	ros::spin();
	return 0;
}
