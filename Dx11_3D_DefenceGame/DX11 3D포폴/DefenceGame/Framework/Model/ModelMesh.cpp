#include "Framework.h"

ModelMesh::ModelMesh()
{
}

ModelMesh::~ModelMesh()
{
	for (ModelMeshPart* part : meshParts)
		delete part;
}

void ModelMesh::Binding()
{
	for (ModelMeshPart* part : meshParts)
		part->Binding();
}

void ModelMesh::Render()
{
	for (ModelMeshPart* part : meshParts)
		part->Render();
}
