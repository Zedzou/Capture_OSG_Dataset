#pragma once
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/LineWidth>
#include <osgText/Text>
#include <osg/ComputeBoundsVisitor>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>

class Coordinate
{
public:
	osg::Geode* MakeCoordinate(float p_x, float p_y, float p_z, float a_x, float a_y, float a_z, float font_size);
};

