#include <Windows.h>
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/AnimationPathManipulator>

#include <osgUtil/Optimizer>
#include "ManipulatorTravel.h"

#include <iostream>
#include <fstream>
#include <Eigen/Eigen>
#include <Eigen/Dense>

using namespace std;

//加灯光
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
				sprintf(Write_TO_TXT, "./images_MainRoad/ScreenShot%04d_H%f.jpg %f %f %f %f %f %f %f \n", _screenCaptureSequence, 19.5, eye._v[0], eye._v[1], eye._v[2], quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
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
				sprintf(filename, "./images_OneRoad2_1/ScreenShot%04d_H%f.jpg", _screenCaptureSequence, 19.5);
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
				sprintf(Write_TO_TXT, "./images_OneRoad2_1/ScreenShot%04d.jpg %f %f %f %f %f %f %f \n", _screenCaptureSequence, eye._v[0], eye._v[1], eye._v[2], quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
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

int main()
{
	mkdir_file.open("./images_OneRoad2_1/dataset.txt");

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

	osg::ref_ptr<osg::Group> root = new osg::Group();

	//读入cow模型
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("Y:\\test_modual\\Panoeye_Model_5DJ\\Panoeye_Model\\Productions\\Panoeye_ENU\\s3csample.s3c");

	//申请一个操作器
	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator("OneRoad_2_1.path");

	//启用操作器
	viewer->setCameraManipulator(apm.get());

	root->addChild(cow.get());
	//灯光
	osg::Group* Scene = new osg::Group();
	Scene = createLight(root);
	root->addChild(Scene);

	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//设置绘制回调
	viewer->getCamera()->setPostDrawCallback(new CaptureDrawCallback(image_c.get()));
	viewer->setSceneData(root.get());
	//添加抓图事件
	viewer->addEventHandler(new ImageHandler());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();
	mkdir_file.close();

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