#include "Box.hpp"
#include "MathHelper.hpp"
#include "StringHelper.hpp"

using namespace Pocket;

Vector2 Box::corners[4];

Box::Box()
:
left(0), top(0), right(0), bottom(0)
{}

Box::~Box() {
}

Box::Box(const Box &source)
:
left(source.left), top(source.top), right(source.right), bottom(source.bottom)
{}

Box::Box(float left, float top, float right, float bottom)
:
left(left), top(top), right(right), bottom(bottom)
{}

bool Box::Intersect(const Box& other) const{
	return !(left > other.right
                || right < other.left
                || top > other.bottom
                || bottom < other.top);
}

bool Box::Inside(const Vector2& position) const{
 return !(position.x < left ||
			position.x > right ||
			position.y > top ||
			position.y < bottom);
}

void Box::FromMatrix(const Pocket::Matrix3x3 &matrix) {

	corners[0].x = left;
	corners[0].y = top;

	corners[1].x = right;
	corners[1].y = top;

	corners[2].x = right;
	corners[2].y = bottom;

	corners[3].x = left;
	corners[3].y = bottom;

		matrix.TransformPointRef(corners[0]);
		matrix.TransformPointRef(corners[1]);
		matrix.TransformPointRef(corners[2]);
		matrix.TransformPointRef(corners[3]);

		left = right = corners[0].x;
		top = bottom = corners[0].y;


		for (int i = 1; i < 4; i++) {
			left = Min(left, corners[i].x);
			right = Max(right, corners[i].x);
			top = Min(top, corners[i].y);
			bottom = Max(bottom, corners[i].y);
		}
}

void Box::FromMatrix(const Pocket::Vector2 &topLeft, 
					 const Pocket::Vector2 &bottomRight, 
					 const Pocket::Matrix3x3 &matrix) {

	//Vector2 corners[4];
		
	corners[0] = topLeft;
	corners[1].x = bottomRight.x;
    corners[1].y = topLeft.y;

    corners[2] = bottomRight;

    corners[3].x = topLeft.x;
    corners[3].y = bottomRight.y;
	/*
	corners[0]=matrix.TransformPoint(corners[0]);
	corners[1]=matrix.TransformPoint(corners[1]);
	corners[2]=matrix.TransformPoint(corners[2]);
	corners[3]=matrix.TransformPoint(corners[3]);
	*/
	
	matrix.TransformPointRef(corners[0]);
	matrix.TransformPointRef(corners[1]);
	matrix.TransformPointRef(corners[2]);
	matrix.TransformPointRef(corners[3]);
	

	left = right = corners[0].x;
    top = bottom = corners[0].y;

	
    for (int i = 1; i < 4; i++) {
        left = Min(left, corners[i].x);
        right = Max(right, corners[i].x);
        top = Min(top, corners[i].y);
        bottom = Max(bottom, corners[i].y);
    }
}

bool Box::operator ==(const Box &other) const {
	return !(left!=other.left || right!=other.right  || top!=other.top || bottom!=other.bottom);
}

bool Box::operator !=(const Box &other) const {
	return(left!=other.left || right!=other.right  || top!=other.top || bottom!=other.bottom);;
}

Vector2 Box::Interpolate(const Vector2& p) const {
	return Vector2(left + (right - left) * p.x, top + (bottom - top) * p.y);
}

/*

public static void CalculateRectangle(
            Vector2 topLeft, 
            Vector2 bottomRight, 
            ref Matrix matrix,
            ref Vector4 boundingRectangle) {

            corners[0] = topLeft;
            
            corners[1].X = bottomRight.X;
            corners[1].Y = topLeft.Y;

            corners[2] = bottomRight;

            corners[3].X = topLeft.X;
            corners[3].Y = bottomRight.Y;

            Vector2.Transform(ref corners[0], ref matrix, out corners[0]);
            Vector2.Transform(ref corners[1], ref matrix, out corners[1]);
            Vector2.Transform(ref corners[2], ref matrix, out corners[2]);
            Vector2.Transform(ref corners[3], ref matrix, out corners[3]);

            boundingRectangle.X = boundingRectangle.Z = corners[0].X;
            boundingRectangle.Y = boundingRectangle.W = corners[0].Y;

            for (int i = 1; i < 4; i++) {

                boundingRectangle.X = System.Math.Min(boundingRectangle.X, corners[i].X);
                boundingRectangle.Z = System.Math.Max(boundingRectangle.Z, corners[i].X);
                boundingRectangle.Y = System.Math.Min(boundingRectangle.Y, corners[i].Y);
                boundingRectangle.W = System.Math.Max(boundingRectangle.W, corners[i].Y);
            }
        }


		*/

Vector2 Pocket::Box::Position() const {
	return Vector2(left, bottom);
}

Vector2 Pocket::Box::Size() const {
	return Vector2(right - left, top - bottom);
}

Vector2 Pocket::Box::Center() const {
    return Vector2((left + right) * 0.5f,(bottom + top) * 0.5f);
}

float Pocket::Box::Aspect() const {
    return (right - left) / (top - bottom);
}

Box Pocket::Box::Deserialize(const std::string &data) {
    std::vector<float> v;
    StringHelper::splitToFloats(data, ',', v);
    return Box(v.size()>0 ? v[0] : 0,
               v.size()>1 ? v[1] : 0,
               v.size()>2 ? v[2] : 0,
               v.size()>3 ? v[3] : 0);
}

std::ostream& operator<<(std::ostream& stream, const Pocket::Box& v) {
    stream << v.left <<","<<v.top<<","<<v.right<<","<<v.bottom;
    return stream;
}


