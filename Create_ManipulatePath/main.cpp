#include <Windows.h>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Math>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

#include "Coordinate.h"
#include <iostream>
#include <math.h>

#include <Eigen/Dense>
#include <Eigen/Eigen>

#include "ManipulatorTravel.h"
#include "Coordinate.h"

using namespace std;

osg::Group* createLight(osg::Node* pNode);
osg::ref_ptr<osg::Image> image_c = new osg::Image();
ofstream mkdir_file;

//获取抓图
struct CaptureDrawCallback : public osg::Camera::DrawCallback
{

	CaptureDrawCallback(osg::ref_ptr<osg::Image> image)
	{
		_image = image;
	}

	~CaptureDrawCallback() {}

	virtual void operator () (const osg::Camera& camera) const
	{
		//得到窗口系统接口
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

		unsigned int width, height;
		//获取分辨率
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		//分配一个image
		_image->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		//读取像素信息抓图
		_image->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);
	}

	osg::ref_ptr<osg::Image> _image;
};

class ImageHandler : public osgGA::GUIEventHandler
{

public:
	ImageHandler()
	{
		//
	}

	~ImageHandler()
	{
		//
	}

	//抓图函数
	bool CaptureImage(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
		{
			return false;
		}

		//定义一个静态变量
		static int _screenCaptureSequence = 0;
		switch (ea.getEventType())
		{
			char filename[128];
			//确定一个合理的文件名，以保证能够连续抓图
			sprintf(filename, "./images_H19.5/ScreenShot%04d_H%f.jpg", _screenCaptureSequence, 19.5);
			_screenCaptureSequence++;
			osgDB::writeImageFile(*(image_c.get()), filename);   //写入文件

			TravelManipulator Manipulate;
			//viewer->setCameraManipulator(NULL); //关闭漫游器


			osg::Vec3d eye, center, up;
			viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
			osg::Matrixd matrix_camera;
			matrix_camera = viewer->getCamera()->getViewMatrix();




			//matrix_camera = Manipulate.getMatrix();

			////从漫游器获取旋转欧拉角并转成旋转矩阵
			//osg::Vec3 Rotation_Vec3;
			//Rotation_Vec3 = Manipulate.Get_Rotation();
			//Eigen::Vector3d ea;
			//ea << Rotation_Vec3[0], Rotation_Vec3[1], Rotation_Vec3[2];
			//Eigen::Quaterniond quaternion;
			//quaternion = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());


			cout << "matrix_camera: " << matrix_camera(0, 0) << ", " << matrix_camera(0, 1) << ", " << matrix_camera(0, 2) << ", "
				<< matrix_camera(1, 0) << ", " << matrix_camera(1, 1) << ", " << matrix_camera(1, 2) << ", "
				<< matrix_camera(2, 0) << ", " << matrix_camera(2, 1) << ", " << matrix_camera(2, 2) << endl;

			Eigen::Matrix<double, 3, 3 > Camera_Matrix;
			Camera_Matrix << matrix_camera(0, 0), matrix_camera(0, 1), matrix_camera(0, 2), matrix_camera(1, 0), matrix_camera(1, 1), matrix_camera(1, 2), matrix_camera(2, 0), matrix_camera(2, 1), matrix_camera(2, 2);
			Eigen::Quaterniond quaternion(Camera_Matrix);

			char *Write_TO_TXT = new char[sizeof(filename) + sizeof(eye._v[0]) + sizeof(eye._v[1]) + sizeof(eye._v[2]) + sizeof(quaternion.x()) + sizeof(quaternion.y()) + sizeof(quaternion.z()) + sizeof(quaternion.w()) + 100];
			sprintf(Write_TO_TXT, "./images_H19.5/ScreenShot%04d_H%f.jpg %f %f %f %f %f %f %f \n", _screenCaptureSequence, 19.5, eye._v[0], eye._v[1], eye._v[2], quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
			mkdir_file << Write_TO_TXT;

		};
		return true;
	}


	//重载handler()函数
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
		{
			return false;
		}

		//定义一个静态变量
		static int _screenCaptureSequence = 0;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN: {

			if (ea.getKey() == 0xFFBE) {  // 键盘F1按键

				char filename[128];
				//确定一个合理的文件名，以保证能够连续抓图
				sprintf(filename, "./images_H19.5/ScreenShot%04d_H%f.jpg", _screenCaptureSequence, 19.5);
				_screenCaptureSequence++;
				osgDB::writeImageFile(*(image_c.get()), filename);   //写入文件

				TravelManipulator Manipulate;
				//viewer->setCameraManipulator(NULL); //关闭漫游器


				osg::Vec3d eye, center, up;
				viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
				osg::Matrixd matrix_camera;
				matrix_camera = viewer->getCamera()->getViewMatrix();




				//matrix_camera = Manipulate.getMatrix();

				////从漫游器获取旋转欧拉角并转成旋转矩阵
				//osg::Vec3 Rotation_Vec3;
				//Rotation_Vec3 = Manipulate.Get_Rotation();
				//Eigen::Vector3d ea;
				//ea << Rotation_Vec3[0], Rotation_Vec3[1], Rotation_Vec3[2];
				//Eigen::Quaterniond quaternion;
				//quaternion = Eigen::AngleAxisd(ea[0], Eigen::Vector3d::UnitZ()) * Eigen::AngleAxisd(ea[1], Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(ea[2], Eigen::Vector3d::UnitX());


				cout << "matrix_camera: " << matrix_camera(0, 0) << ", " << matrix_camera(0, 1) << ", " << matrix_camera(0, 2) << ", "
					<< matrix_camera(1, 0) << ", " << matrix_camera(1, 1) << ", " << matrix_camera(1, 2) << ", "
					<< matrix_camera(2, 0) << ", " << matrix_camera(2, 1) << ", " << matrix_camera(2, 2) << endl;

				Eigen::Matrix<double, 3, 3 > Camera_Matrix;
				Camera_Matrix << matrix_camera(0, 0), matrix_camera(0, 1), matrix_camera(0, 2), matrix_camera(1, 0), matrix_camera(1, 1), matrix_camera(1, 2), matrix_camera(2, 0), matrix_camera(2, 1), matrix_camera(2, 2);
				Eigen::Quaterniond quaternion(Camera_Matrix);

				char *Write_TO_TXT = new char[sizeof(filename) + sizeof(eye._v[0]) + sizeof(eye._v[1]) + sizeof(eye._v[2]) + sizeof(quaternion.x()) + sizeof(quaternion.y()) + sizeof(quaternion.z()) + sizeof(quaternion.w()) + 100];
				sprintf(Write_TO_TXT, "./images_H19.5/ScreenShot%04d_H%f.jpg %f %f %f %f %f %f %f \n", _screenCaptureSequence, 19.5, eye._v[0], eye._v[1], eye._v[2], quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
				mkdir_file << Write_TO_TXT;
			}
		}
											  break;

		default:
			return false;
		}
		return true;
	}
};

//路径动画控制事件
class AnimationEventHandler : public osgGA::GUIEventHandler
{
public:

	AnimationEventHandler(osgViewer::Viewer &vr) :
		viewer(vr)
	{
		//
	}

	//事件处理
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&)
	{
		//创建动画更新回调对象
		osg::ref_ptr<osg::AnimationPathCallback> animationPathCallback = new osg::AnimationPathCallback();
		osg::ref_ptr< osg::Group> group = dynamic_cast<osg::Group*>(viewer.getSceneData());

		//取得节点的动画属性
		animationPathCallback = dynamic_cast<osg::AnimationPathCallback*>(group->getChild(0)->getUpdateCallback());
		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			if (ea.getKey() == 'p')
			{
				//暂停
				animationPathCallback->setPause(true);

				return true;
			}

			if (ea.getKey() == 's')
			{
				//开始
				animationPathCallback->setPause(false);

				return true;
			}

			if (ea.getKey() == 'r')
			{
				//重新开始
				animationPathCallback->reset();

				return true;
			}

			break;
		}
		default:
			break;
		}
		return false;
	}

	osgViewer::Viewer &viewer;
};

//创建路径 （圆形路径）
osg::ref_ptr<osg::AnimationPath> createAnimationPath(osg::Vec3& center, float radius, float looptime)
{
	//创建一个Path对象
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();

	//设置动画模式为循环(LOOP)(LOOP:循环，SWING:单摆,NO_LOOPING:不循环)
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	//关键点数
	int numPoint = 6000;

	//每次偏移角度(偏航角)
	float yaw = 0.0f;
	float yaw_delta = 2.0f*osg::PI / ((float)(numPoint - 1.0f));

	//倾斜角度
	float roll = osg::inDegrees(45.0f);

	//pitch
	float pitch = osg::inDegrees(-45.0f);

	//时间偏移 （每一个点的消耗时间）
	float time = 0.0f;
	float time_delta = looptime / ((float)numPoint);

	for (int i = 0; i < numPoint; i++)
	{
		//关键点位置 （计算出下一个位置坐标）
		osg::Vec3 position(center + osg::Vec3(sinf(yaw)*radius, cosf(yaw)*radius, 0.0f));
		//关键点角度 （计算出下一个角度）  构造四元数方法osg::Quat quat(float radians, const Vec3f& axis),其中radians是旋转弧度, 后面的axis是旋转轴向量;
		osg::Quat rotation(osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(-(yaw + osg::inDegrees(90.0f)), osg::Vec3(0.0, 0.0, 1.0)));
		//插入Path，把关键点与时间压入形成Path
		animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));

		yaw += yaw_delta;
		time += time_delta;
	}

	//返回Path
	return animationPath.get();
}

//创建路径 （方形路径）
osg::ref_ptr<osg::AnimationPath> createAnimationPath(osg::Vec3& StartPoint, float Long, float width, float looptime)
{
	//创建一个path对象
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();

	//设置动画模式 （LOOP, NO_LOOPING, SWING）
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	//关键点数
	int numPoint = 1200;

	//偏航角
	float yaw_0 = -90.0f;
	float yaw_1 = -90.0f;
	float yaw_2 = -180.0f;
	float yaw_3 = -270.0f;
	//翻滚角
	float roll = 0.0f;
	//pitch
	float pitch = 0.0f;

	//时间偏移
	float time = 0.0f;
	float time_delta = looptime / ((float)numPoint);
	//点与点之间的距离
	float distancePoint = ((float)((Long + width) * 2)) / (numPoint - 1);
	//长、宽方向点的个数
	int numPointLong = ((Long / distancePoint) + 1);
	int numPointWidth = ((width / distancePoint) + 1);

	for (int i = 0; i < numPoint; i++)
	{
		if (i + 1 < numPointLong)
		{
			osg::Vec3 position(StartPoint + osg::Vec3(0.0f, distancePoint * (i + 1), 0.0f));
			osg::Quat rotation(osg::Quat(roll, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(pitch, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(yaw_0, osg::Vec3(0.0, 0.0, 1.0)));
			animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
			cout << "第一段： " << i << ":" << position[0] << ", " << position[1] << ", " << position[2] << endl;
		}
		else if ((i + 1) >= numPointLong && (i + 1) <= (numPointLong + numPointWidth))
		{
			osg::Vec3 position(StartPoint + osg::Vec3(((i - numPointLong) * distancePoint), Long, 0.0f));
			osg::Quat rotation(osg::Quat(roll, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(pitch, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(yaw_1, osg::Vec3(0.0, 0.0, 1.0)));
			animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
			cout << "第二段： " << i << ": " << position[0] << ", " << position[1] << ", " << position[2] << endl;
		}
		else if ((i + 1) >= (numPointLong + numPointWidth) && (i + 1) < (numPointLong * 2 + numPointWidth))
		{
			osg::Vec3 position(StartPoint + osg::Vec3(width, Long - (i - numPointLong - numPointWidth)*distancePoint, 0.0f));
			osg::Quat rotation(osg::Quat(roll, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(pitch, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(yaw_2, osg::Vec3(0.0, 0.0, 1.0)));
			animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
			cout << "第三段： " << i << ":" << position[0] << ", " << position[1] << ", " << position[2] << endl;
		}
		else if ((i + 1) >= (numPointLong * 2 + numPointWidth) && (i + 1) < (numPointLong * 2 + numPointWidth * 2))
		{
			osg::Vec3 position(StartPoint + osg::Vec3(width - ((i - numPointLong * 2 - numPointWidth) - 1) * distancePoint, 0.0f, 0.0f));
			osg::Quat rotation(osg::Quat(roll, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(pitch, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(yaw_3, osg::Vec3(0.0, 0.0, 1.0)));
			animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
			cout << "第四段： " << i << ": " << position[0] << ", " << position[1] << ", " << position[2] << endl;
		}
		time += time_delta;
	}
	return animationPath.get();
}

//创建路径 （直线路径）
osg::ref_ptr<osg::AnimationPath> createAnimationPath(osg::Vec3& StartPoint, osg::Vec3& EndPoint, float looptime)
{
	//创建一个path对象
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();

	//设置动画模式 （LOOP, NO_LOOPING, SWING）
	animationPath->setLoopMode(osg::AnimationPath::LOOP);
	//关键点数
	int numPoint = 5000;
	//直线段的长度
	float Long_Line_2 = pow((EndPoint[0] - StartPoint[0]), 2) + pow((EndPoint[1] - StartPoint[1]), 2);
	float Long_Line = sqrt(Long_Line_2);
	//时间偏移
	float time = 0.0f;
	float time_delta = looptime / ((float)numPoint);
	//float time_delta_2 = (looptime - 10) / ((float)numPoint);

	//点之间的距离
	float DistancePoint = Long_Line / (numPoint - 1);
	//直线的单位向量
	Eigen::Vector2f LineVec;
	LineVec << ((EndPoint[0] - StartPoint[0]) / Long_Line), ((EndPoint[1] - StartPoint[1]) / Long_Line);

	//角度
	float yaw = osg::inDegrees(90.0f); //偏航角 Z
	float roll = osg::inDegrees(70.0f); //翻滚角 Y 
	float pitch = osg::inDegrees(20.0f); //pitch X
	float roll_2 = osg::inDegrees(-50.0f);
	float pitch_2 = osg::inDegrees(10.0f);
	float roll_3 = osg::inDegrees(-40.0f);
	float pitch_3 = osg::inDegrees(15.0f);

	float yaw_one = osg::inDegrees(90.0f); //偏航角 Z
	float roll_one = osg::inDegrees(90.0f); //翻滚角 Y 
	float pitch_one = osg::inDegrees(0.0f); //pitch X

	time += 5.0;
	for (int i = 0; i < numPoint; i++)
	{
		Eigen::Vector2f MotionVec;
		MotionVec = -((i + 1)*DistancePoint)*LineVec; //运动特征点的向量
		osg::Vec2 point_vector(MotionVec[0], MotionVec[1]); //特征点的向量
		osg::Vec3 position(StartPoint[0] - point_vector[0], StartPoint[1] - point_vector[1], StartPoint[2]); //特征点的三位坐标

		//************MainRoad1

		//osg::Quat rotation(osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));  //从右往左依次进行旋转
		//osg::Quat rotation(osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		//osg::Quat rotation(osg::Quat(pitch_2, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		//osg::Quat rotation(osg::Quat(roll_3, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		
		//**********MainRoad2

		/* MainRoad2_1 */
		//osg::Quat rotation(osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		
		/* MainRoad2_2 */ //roll_2 = -50.0f
		//osg::Quat rotation(osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		
		/* MainRoad2_3 */ //pitch_2 = 15.0f
		//osg::Quat rotation(osg::Quat(pitch_2, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		
		/* MainRoad2_4 */
		//osg::Quat rotation(osg::Quat(pitch_3, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(roll_3, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));
		
		//***********MainROad3

		//MainRoad3_1 
		//osg::Quat rotation(osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));

		//MainRoad3_2
		//osg::Quat rotation(osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));

		/* MainRoad3_3 */ //pitch_2 = 15.0f
		//osg::Quat rotation(osg::Quat(pitch_2, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(roll_2, osg::Vec3(0.0, 1.0, 0.0)) * osg::Quat(pitch, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll, osg::Vec3(0.0, 1.0, 0.0)));


		//**********oneRoad1
		//osg::Quat rotation(osg::Quat(pitch_one, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw_one, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll_one, osg::Vec3(0.0, 1.0, 0.0))); //1_1  pitch = 0.0f
		//osg::Quat rotation(osg::Quat(pitch_one, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw_one, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll_one, osg::Vec3(0.0, 1.0, 0.0))); //1_2  pitch = 20.0f
		//osg::Quat rotation(osg::Quat(pitch_one, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw_one, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll_one, osg::Vec3(0.0, 1.0, 0.0))); //1_3  pitch = 10.0f

		//*********OneRoad2
		osg::Quat rotation(osg::Quat(pitch_one, osg::Vec3(1.0, 0.0, 0.0)) * osg::Quat(yaw_one, osg::Vec3(0.0, 0.0, 1.0)) * osg::Quat(roll_one, osg::Vec3(0.0, 1.0, 0.0))); //2_1

		animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
		time += time_delta;
		if (i%50 == 0)
		{
			//停留
			animationPath->insert(time, osg::AnimationPath::ControlPoint(position, rotation));
			time += 2.0;
		}

	}
	return animationPath.get();
}

int main()
{
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读入卡车模型
	osg::ref_ptr<osg::Node> cessna = osgDB::readNodeFile("H:\\vcpkg\\packages\\OpenSceneGraph-Data-3.0.0\\cessna.osg");

	//读入地形模型
	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile("Y:\\test_modual\\Panoeye_Model_5DJ\\Panoeye_Model\\Productions\\Panoeye_ENU\\s3csample.s3c");

	//卡车模型坐标系
	osg::ref_ptr<osg::Group> cessna_group = new osg::Group;
	cessna_group->addChild(cessna.get());
	Coordinate coordinate;
	cessna_group->addChild(coordinate.MakeCoordinate(0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 2.0));

	//地形模型坐标系
//	Coordinate coordinate;
	root->addChild(coordinate.MakeCoordinate(0.0, 0.0, 0.0, 500.0, 500.0, 200.0, 50.0));

	//得到包围盒，来确定动画旋转中心
	const osg::BoundingSphere& bs = cessna->getBound();
	osg::Vec3 position = bs.center() + osg::Vec3(0.0f, 0.0f, 200.0f);
	//缩放比例，如果比例不当的话，模型会看不见
	float size = 100.0f / bs.radius()*0.1f;

	////PanoEye中的马路漫游起点与终点  门口马路第一条
	//osg::Vec3 StartPoint(61.646042f, -76.476547f, 4.5f);
	//osg::Vec3 EndPoint(15.623094f, -106.57222f, 4.5f);

	////PanoEye中的马路漫游起点与终点  门口马路第二条
	//osg::Vec3 StartPoint(70.910423f, -65.340996f, 4.5f);
	//osg::Vec3 EndPoint(-7.495761f, -117.153206, 4.5f);

	////PanoEye中的马路漫游起点和终点  门口马路第三条
	//osg::Vec3 StartPoint(95.409943f, -40.393982f, 4.5f);
	//osg::Vec3 EndPoint(-37.514317f, -127.687645f, 4.5f);

	////PanoEye靠近公司马OneRoad1  靠近公司马路第一条
	//osg::Vec3 StartPoint(20.750908f, -21.059109f, 4.5f);
	//osg::Vec3 EndPoint(48.429039f, -58.061600f, 4.5f);

	//OneRoad2
	osg::Vec3 StartPoint(14.143380f, -20.974194f, 4.5f);
	osg::Vec3 EndPoint(40.741459f, -60.860077f, 4.5f);

	//创建路径
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();
	//animationPath = createAnimationPath(position, 100.0f, 10.0f); //圆形
	//animationPath = createAnimationPath(position, 200.0f, 200.0f, 10.0f); //方形
	animationPath = createAnimationPath(StartPoint, EndPoint, 10.0f); //直线

	//保存路径
	std::string fileName("OneRoad_2_1.path");
	std::ofstream out(fileName.c_str());
	animationPath->write(out);

	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform();
	//OSG确保只有STATIC数据可以进行图形渲染
	mt->setDataVariance(osg::Object::STATIC);
	//进行适当的变换(平移，缩放以及旋转)
	mt->setMatrix(osg::Matrix::translate(-bs.center())*
		osg::Matrix::scale(size, size, size)*
		osg::Matrix::rotate(osg::inDegrees(180.0f), 0.0f, 0.0f, 1.0f));
	mt->addChild(cessna_group.get());

	osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform();
	//设置更新回调
	pat->setUpdateCallback(new osg::AnimationPathCallback(animationPath.get(), 0.0f, 1.0f));
	pat->addChild(mt.get());

	root->addChild(pat.get());
	root->addChild(node.get());
	//灯光
	osg::Group* Scene = new osg::Group();
	Scene = createLight(root);
	root->addChild(Scene);

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	viewer->setSceneData(root.get());

	//开启深度测试
	root->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

	//添加路径动画控制事件
	viewer->addEventHandler(new AnimationEventHandler(*(viewer.get())));

	viewer->realize();

	viewer->run();

	return 0;
}

//加灯光
osg::Group* createLight(osg::Node* pNode)
{
	osg::Group* lightGroup = new osg::Group;
	osg::BoundingSphere boundSphere = pNode->getBound();
	lightGroup->setName("light root");

	// create a spot light.
	osg::Light* myLight1 = new osg::Light;
	myLight1->setLightNum(0);
	myLight1->setPosition(osg::Vec4(boundSphere.center().x(), boundSphere.center().y(), boundSphere.center().z() + boundSphere.radius(), 0.1f));
	myLight1->setAmbient(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	myLight1->setDiffuse(osg::Vec4(0.5f, 0.5f, 0.5f, 0.5f));
	myLight1->setSpotCutoff(20.0f);
	myLight1->setSpotExponent(50.0f);
	myLight1->setDirection(osg::Vec3(0.0f, 0.0f, -1.0f));

	osg::LightSource* lightS1 = new osg::LightSource;
	lightS1->setLight(myLight1);
	lightS1->setLocalStateSetModes(osg::StateAttribute::ON);


	osg::StateSet* rootStateSet = pNode->getOrCreateStateSet();
	lightS1->setStateSetModes(*rootStateSet, osg::StateAttribute::ON);

	lightGroup->addChild(lightS1);

	return lightGroup;
}