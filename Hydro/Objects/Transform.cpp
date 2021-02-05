#include "Transform.h"

using namespace Hydro;

Matrix4 Transform::GetTransformation() const{
	//Translation Matrix
	Matrix4 positionMatrix = Matrix4::Translate(position);

	//Rotation Matrix
	Matrix4 rotationMatrix = rotation.ToMatrix4();

	//Scale Matrix
	Matrix4 scaleMatrix = Matrix4::Scale(scale);

	//if(parent == nullptr){
		return (positionMatrix * (rotationMatrix * scaleMatrix));
	//}else{
	//	return parent->GetTransformation() * (positionMatrix * (rotationMatrix * scaleMatrix));
	//}
}