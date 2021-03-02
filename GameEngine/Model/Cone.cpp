#include "Cone.h"
#include <iostream>

Cone::Cone(){
    init();
}

Cone::Cone(const Vec3 pos, const float radius, const float height) : pos(pos){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;

    init();
}

Cone::Cone(const Vec3 pos, const Vec3 scale, const Vec3 rotation): pos(pos),scale(scale),rotation(rotation){
    init();
}

Cone::Cone(const float radius, const float height, const unsigned int segment){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;
    if (segment >= 3 && segment <= 100)
        this->segment = segment;

    init();
}

Cone::~Cone(){
    delete vb;
    delete ib;
    delete ibl;
    delete program;

    vertices.clear();
    vertices.resize(0);
    vertices.shrink_to_fit();

    indices.clear();
    indices.resize(0);
    indices.shrink_to_fit();

    lineIndices.clear();
    lineIndices.resize(0);
    lineIndices.shrink_to_fit();
}

void Cone::init(){
    program = new Program("Shaders/Cone.shader");

    vb = new VertexBuffer(nullptr,sizeof(Vec3) * 602,GL_STATIC_DRAW);

    ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 600, GL_STATIC_DRAW);
    ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 600, GL_STATIC_DRAW);

    va.bind();
    vb->bind();

    va.addVertexAttribute(3, GL_FLOAT, false);
    va.addVertexAttribute(3, GL_FLOAT, false);

    setSegment(segment);
}

void Cone::draw(Camera& camera){
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    model = GLMath::translate(pos) * GLMath::scale(scale) * GLMath::rotate(rotation);

    program->setUniformMatrix4fv("model", 1, false, model.m[0]);
    program->setUniformMatrix4fv("view", 1, false, camera.getViewMatrix());
    program->setUniformMatrix4fv("projection", 1, false, camera.combine());

    va.bind();

    if (mode == DrawMode::POINT) {
        program->setUniform1f("chosen", 0);
        glDrawArrays(GL_POINTS, 0, vertices.size() / 2);
    }
    else if (mode == DrawMode::LINE) {
        ibl->bind();
        //glDisable(GL_DEPTH_TEST);
        program->setUniform1f("chosen", 1);
        glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);
    }
    else if (mode == DrawMode::TRIANGLE) {
        ib->bind();
        program->setUniform1f("chosen", 2);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void Cone::setDrawMode(DrawMode mode){
    this->mode = mode;
}

void Cone::setSegment(const unsigned int segment){
   
    if (segment >= 3 && segment <= 100) {
        this->segment = segment;
    }
    else {
        return;
    }
   
    vertices.clear();
    vertices.resize(0);
    vertices.shrink_to_fit();

    indices.clear();
    indices.resize(0);
    indices.shrink_to_fit();

    lineIndices.clear();
    lineIndices.resize(0);
    lineIndices.shrink_to_fit();

    
    for (unsigned int i = 0; i < segment; i++) {
        float radian = i * 2.0f * PI / (float)segment;
        Vec3 pos;

        pos.y = -height / 2.0f;
        pos.x = cosf(radian) * radius;
        pos.z = sinf(radian) * radius;

        vertices.push_back({ 0, height / 2.0f,0 });
        vertices.push_back(color);

        vertices.push_back(pos);
        vertices.push_back(color);
    }
    
    vertices.push_back({ 0, -height / 2.0f,0 });
    vertices.push_back(color);

    for (unsigned int i = 0; i < segment; i++) {
        float radian = i * 2.0f * PI / (float)segment;
        Vec3 pos;

        pos.y = -height / 2.0f;
        pos.x = cosf(radian) * radius;
        pos.z = sinf(radian) * radius;

        vertices.push_back(pos);
        vertices.push_back(color);
    }
    
    //Triangle indices
    for (unsigned int i = 0; i < segment * 2; i += 2) {
        if (i < 2 * segment - 2) {
            indices.push_back(i);
            indices.push_back(i + 3);
            indices.push_back(i + 1);
        }
        else {
            indices.push_back(i);
            indices.push_back(1);
            indices.push_back(i + 1);
        }
    }

    for (unsigned int i = 2 * segment + 1; i < 3 * segment + 1 ; i++) {
        if (i < 3 * segment) {
            indices.push_back(2 * segment);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        else {
            indices.push_back(2 * segment);
            indices.push_back(i);
            indices.push_back(2 * segment + 1);
        }
    }

    //Line indices
    for (unsigned int i = 0; i < segment * 2; i += 2) {
        if (i < 2 * segment - 2) {
            lineIndices.push_back(i);
            lineIndices.push_back(i + 3);
            lineIndices.push_back(i + 3);
            lineIndices.push_back(i + 1);
        }
        else {
            lineIndices.push_back(i);
            lineIndices.push_back(1);
            lineIndices.push_back(1);
            lineIndices.push_back(i + 1);
        }
    }

    for (unsigned int i = 2 * segment + 1; i < 3 * segment + 1; i++) {
        if (i < 3 * segment) {
            lineIndices.push_back(2 * segment);
            lineIndices.push_back(i);
            //lineIndices.push_back(i + 1);
        }
        else {
            lineIndices.push_back(2 * segment);
            lineIndices.push_back(i);
            //lineIndices.push_back(2 * segment + 1);
        }
    }
    
    //std::cout << "Vertices size: " << vertices.size() << std::endl;
    //std::cout << "Indices size: " << indices.size() << std::endl;
    //std::cout << "LineIndices size: " << lineIndices.size() << std::endl;

    vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    ib->subData(0, indices.size() * sizeof(unsigned int), indices.data());
    ibl->subData(0, lineIndices.size() * sizeof(unsigned int), lineIndices.data());

}

unsigned int Cone::getSegment(){
    return segment;
}

void Cone::setRadius(const float radius){
    if (radius > 0) {
        for (unsigned int i = 0; i < vertices.size();) {
            if (i < 4 * segment) {
                vertices[i + 2].x = vertices[i + 2].x / this->radius * radius;
                vertices[i + 2].z = vertices[i + 2].z / this->radius * radius;

                i += 4;
            }
            else if(i >= 4 *segment + 2) {
                vertices[i].x = vertices[i].x / this->radius * radius;
                vertices[i].z = vertices[i].z / this->radius * radius;

                i += 2;
            }
            else {
                i += 2;
            }
        }

        this->radius = radius;
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

float Cone::getRadius() {
    return radius;
}

void Cone::setHeight(const float height){
    if (height > 0) {
        for (unsigned int i = 0; i < vertices.size(); ){
            if (i < 4 * segment) {
                vertices[i].y = height / 2.0f;
                vertices[i + 2].y = -height / 2.0f;
                i += 4;
            }
            else {
                vertices[i].y = -height / 2.0f;
                i += 2;
            }
        }

        this->height = height;
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

float Cone::getHeight(){
    return height;
}

void Cone::setSize(const float radius, const float height){
    if (radius > 0) {
         for (unsigned int i = 0; i < vertices.size();) {
            if (i < 4 * segment) {
                vertices[i + 2].x = vertices[i + 2].x / this->radius * radius;
                vertices[i + 2].z = vertices[i + 2].z / this->radius * radius;

                i += 4;
            }
            else if(i >= 4 *segment + 2) {
                vertices[i].x = vertices[i].x / this->radius * radius;
                vertices[i].z = vertices[i].z / this->radius * radius;

                i += 2;
            }
            else {
                i += 2;
            }
        }

        this->radius = radius;
    }

    if (height > 0) {
        for (unsigned int i = 0; i < vertices.size(); ) {
            if (i < 4 * segment) {
                vertices[i].y = height / 2.0f;
                vertices[i + 2].y = -height / 2.0f;
                i += 4;
            }
            else {
                vertices[i].y = -height / 2.0f;
                i += 2;
            }
        }

        this->height = height;
    }

    if (radius > 0 || height > 0) {
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

void Cone::setPosition(const Vec3 pos){
    this->pos = pos;
}

Vec3 Cone::getPosition(){
    return pos;
}

void Cone::setScale(const Vec3 scale){
    this->scale = scale;
}

Vec3 Cone::getScale(){
    return scale;
}

void Cone::rotate(Vec3 rot){
    rotation = rotation + rot;
}

void Cone::setRotation(Vec3 rotation){
    this->rotation = rotation;
}

Vec3 Cone::getRotation(){
    return rotation;
}

void Cone::setColor(Vec3 color){
    this->color = color;

    for (unsigned int i = 1; i < vertices.size(); i += 2) {
        vertices[i] = color;
    }

    vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
}

Vec3 Cone::getColor(){
    return color;
}
