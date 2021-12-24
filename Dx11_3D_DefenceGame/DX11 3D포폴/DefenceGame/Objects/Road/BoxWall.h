#pragma once

class BoxWall
{
private:    //������ ����
	typedef VertexUVNormalTangentColor VertexType;
    
    Material* material;

    VertexBuffer* vertexBuffer;
    VertexBuffer* instanceBuffer;
    IndexBuffer* indexBuffer;
    
    vector<VertexType> vertices;
    vector<UINT> indices;
private:    //������ ����
    

public:
	BoxWall();
    ~BoxWall();

    void Render(VertexBuffer*, UINT);

private:
    void CreateData();
    void CreateNormal();
    void CreateTangent();
};