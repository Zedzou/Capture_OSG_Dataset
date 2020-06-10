#include <Windows.h>
#include "Coordinate.h"

//模型坐标系 坐标系原点p_x p_y p_z, x y z font_size
osg::Geode* Coordinate::MakeCoordinate(float p_x, float p_y, float p_z, float a_x, float a_y, float a_z, float font_size)
{
	osg::ref_ptr<osg::Sphere> pSphereShape = new osg::Sphere(osg::Vec3(0, 0, 0), 1.0f);
	osg::ref_ptr<osg::ShapeDrawable> pShapeDrawable = new osg::ShapeDrawable(pSphereShape.get());
	pShapeDrawable->setColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	//创建保存几何信息的对象
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//创建四个顶点 X Y Z轴的坐标
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();
	v->push_back(osg::Vec3(p_x, p_y, p_z));
	v->push_back(osg::Vec3(p_x + a_x, p_y, p_z)); //x轴

	v->push_back(osg::Vec3(p_x, p_y, p_z));
	v->push_back(osg::Vec3(p_x, p_y + a_y, p_z)); //y轴

	v->push_back(osg::Vec3(p_x, p_y, p_z));
	v->push_back(osg::Vec3(p_x, p_y, p_z + a_z)); //z轴
	geom->setVertexArray(v.get());

	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();
	c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	c->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));

	c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	c->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));

	c->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	geom->setColorArray(c.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	//xyz
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 2, 2));
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 4, 2));

	osg::ref_ptr<osgText::Text> pTextXAuxis1 = new osgText::Text;
	pTextXAuxis1->setText(L"X");
	pTextXAuxis1->setFont("H:/vcpkg/packages/OpenSceneGraph-Data-3.0.0/fonts/times.ttf");

	pTextXAuxis1->setAxisAlignment(osgText::Text::SCREEN);
	pTextXAuxis1->setCharacterSize(font_size);
	pTextXAuxis1->setPosition(osg::Vec3(p_x + a_x, p_y, p_z));

	osg::ref_ptr<osgText::Text> pTextYAuxis1 = new osgText::Text;
	pTextYAuxis1->setText(L"Y");
	pTextYAuxis1->setFont("H:/vcpkg/packages/OpenSceneGraph-Data-3.0.0/fonts/times.ttf");

	pTextYAuxis1->setAxisAlignment(osgText::Text::SCREEN);
	pTextYAuxis1->setCharacterSize(font_size);
	pTextYAuxis1->setPosition(osg::Vec3(p_x, p_y + a_y, p_z));

	osg::ref_ptr<osgText::Text> pTextZAuxis1 = new osgText::Text;
	pTextZAuxis1->setText(L"Z");
	pTextZAuxis1->setFont("H:/vcpkg/packages/OpenSceneGraph-Data-3.0.0/fonts/times.ttf");

	pTextZAuxis1->setAxisAlignment(osgText::Text::SCREEN);
	pTextZAuxis1->setCharacterSize(font_size);
	pTextZAuxis1->setPosition(osg::Vec3(p_x, p_y, p_z + a_z));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(3.0), osg::StateAttribute::ON);

	geode->addDrawable(pShapeDrawable.get());
	geode->addDrawable(geom.get());
	geode->addDrawable(pTextXAuxis1.get());

	geode->addDrawable(pTextYAuxis1.get());
	geode->addDrawable(pTextZAuxis1.get());

	return geode.release();
}