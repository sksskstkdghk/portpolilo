#pragma once

class BoxWall
{
private:    //랜더링 관련
	typedef VertexUVNormalTangentColor VertexType;
    
    Material* material;

    VertexBuffer* vertexBuffer;
    VertexBuffer* instanceBuffer;
    IndexBuffer* indexBuffer;
    
    vector<VertexType> vertices;
    vector<UINT> indices;
private:    //데이터 관련
    

public:
	BoxWall();
    ~BoxWall();

    void Render(VertexBuffer*, UINT);

private:
    void CreateData();
    void CreateNormal();
    void CreateTangent();
};