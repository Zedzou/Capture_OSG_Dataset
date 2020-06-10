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

//�ӵƹ�
osg::Group* createLight(osg::Node* pNode);
osg::ref_ptr<osg::Image> image_c = new osg::Image();
ofstream mkdir_file;

//��ȡץͼ
struct CaptureDrawCallback : public osg::Camera::DrawCallback
{

	CaptureDrawCallback(osg::ref_ptr<osg::Image> image)
	{
		_image = image;
	}

	~CaptureDrawCallback() {}

	virtual void operator () (const osg::Camera& camera) const
	{
		//�õ�����ϵͳ�ӿ�
		osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

		unsigned int width, height;
		//��ȡ�ֱ���
		wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
		//����һ��image
		_image->allocateImage(width, height, 1, GL_RGB, GL_UNSIGNED_BYTE);
		//��ȡ������Ϣץͼ
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

	//ץͼ����
	bool CaptureImage(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
		{
			return false;
		}

		//����һ����̬����
		static int _screenCaptureSequence = 0;
		switch (ea.getEventType())
		{	
				char filename[128];
				//ȷ��һ��������ļ������Ա�֤�ܹ�����ץͼ
				sprintf(filename, "./images_H19.5/ScreenShot%04d_H%f.jpg", _screenCaptureSequence, 19.5);
				_screenCaptureSequence++;
				osgDB::writeImageFile(*(image_c.get()), filename);   //д���ļ�

				TravelManipulator Manipulate;
				//viewer->setCameraManipulator(NULL); //�ر�������


				osg::Vec3d eye, center, up;
				viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
				osg::Matrixd matrix_camera;
				matrix_camera = viewer->getCamera()->getViewMatrix();

				//matrix_camera = Manipulate.getMatrix();

				////����������ȡ��תŷ���ǲ�ת����ת����
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


	//����handler()����
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (viewer == NULL)
		{
			return false;
		}

		//����һ����̬����
		static int _screenCaptureSequence = 0;
		switch (ea.getEventType())
		{
		case osgGA::GUIEventAdapter::KEYDOWN: {

			if (ea.getKey() == 0xFFBE) {  // ����F1����

				char filename[128];
				//ȷ��һ��������ļ������Ա�֤�ܹ�����ץͼ
				sprintf(filename, "./images_OneRoad2_1/ScreenShot%04d_H%f.jpg", _screenCaptureSequence, 19.5);
				_screenCaptureSequence++;
				osgDB::writeImageFile(*(image_c.get()), filename);   //д���ļ�

				TravelManipulator Manipulate;
				//viewer->setCameraManipulator(NULL); //�ر�������


				osg::Vec3d eye, center, up;
				viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
				osg::Matrixd matrix_camera;
				matrix_camera = viewer->getCamera()->getViewMatrix();

				//matrix_camera = Manipulate.getMatrix();

				////����������ȡ��תŷ���ǲ�ת����ת����
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

	//����cowģ��
	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("Y:\\test_modual\\Panoeye_Model_5DJ\\Panoeye_Model\\Productions\\Panoeye_ENU\\s3csample.s3c");

	//����һ��������
	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator("OneRoad_2_1.path");

	//���ò�����
	viewer->setCameraManipulator(apm.get());

	root->addChild(cow.get());
	//�ƹ�
	osg::Group* Scene = new osg::Group();
	Scene = createLight(root);
	root->addChild(Scene);

	//�Ż���������
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root.get());

	//���û��ƻص�
	viewer->getCamera()->setPostDrawCallback(new CaptureDrawCallback(image_c.get()));
	viewer->setSceneData(root.get());
	//���ץͼ�¼�
	viewer->addEventHandler(new ImageHandler());

	viewer->setSceneData(root.get());
	viewer->realize();
	viewer->run();
	mkdir_file.close();

	return 0;
}

//�ӵƹ�
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