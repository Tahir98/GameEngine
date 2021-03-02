#include "Capsule.h"
#include <iostream>

Capsule::Capsule(){
    init();
}

Capsule::Capsule(const Vec3 pos, const float radius, const float height) : pos(pos){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;

    init();
}

Capsule::Capsule(const Vec3 pos, const Vec3 scale, const Vec3 rotation) : pos(pos),scale(scale),rotation(rotation){
    init();
}

Capsule::Capsule(const float radius, const float height, const unsigned int segment){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;
    if (segment >= 3 && segment <= 50)
        this->segment = segment;

    init();
}

Capsule::~Capsule(){
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

void Capsule::init(){
    program = new Program("Shaders/Cone.shader");

    vb = new VertexBuffer(nullptr, sizeof(Vec3) * 10200, GL_STATIC_DRAW);

    ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 29700, GL_STATIC_DRAW);
    ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 29600, GL_STATIC_DRAW);

    va.bind();
    vb->bind();

    va.addVertexAttribute(3, GL_FLOAT, false);
    va.addVertexAttribute(3, GL_FLOAT, false);

    setSegment(segment);
}

void Capsule::draw(Camera& camera){
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

void Capsule::setDrawMode(DrawMode mode){
    this->mode = mode;
}

void Capsule::setSegment(const unsigned int segment){
    if (segment >= 3 && segment <= 50) {
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

    float degZ,degY;
    Vec3 pos;

    /*       Vertices      */
    for (unsigned int k = 0; k < 2; k++) {
        for (unsigned int i = 0; i < segment; i++) {
            degY = 2.0f * PI / (float)segment * i;

            for (unsigned int j = 0; j < segment; j++) {
                degZ = PI / (2.0f * (segment - 1)) * (float)j;

                pos.y = (k==0 ? height / 2.0f + sinf(degZ) * radius : -height / 2.0f - sinf(degZ) * radius) ;
                pos.x = cosf(degZ) * cosf(degY) * radius;
                pos.z = cosf(degZ) * sinf(degY) * radius;

                vertices.push_back(pos);
                vertices.push_back({color});
            }
        }
    }
    
    for (unsigned int i = 0; i < segment; i++) {
        float radian = 2.0f * PI / (float)segment * i;

        pos.x = cosf(radian) * radius;
        pos.z = sinf(radian) * radius;
        pos.y = height / 2.0f;

        vertices.push_back(pos);
        vertices.push_back(color);

        pos.y = -height / 2.0f;
        vertices.push_back(pos);
        vertices.push_back(color);
    }

    /*     Indices      */
    for (unsigned int i = 0; i < segment; i++) {
        for (unsigned int j = 0; j < segment - 1; j++) {
            if (i < segment - 1) {
                indices.push_back(i * segment + j);
                indices.push_back(i * segment + j + 1);
                indices.push_back((i + 1) * segment + j + 1);

                indices.push_back(i * segment + j);
                indices.push_back((i + 1) * segment + j + 1);
                indices.push_back((i + 1) * segment + j);
            }
            else {
                indices.push_back(i * segment + j);
                indices.push_back(i * segment + j + 1);
                indices.push_back(j + 1);

                indices.push_back(i * segment + j);
                indices.push_back(j + 1);
                indices.push_back(j);
            }
        }
    }

    unsigned int temp = segment * segment;
    for (unsigned int i = 0; i < segment; i++) {
        for (unsigned int j = 0; j < segment - 1; j++) {
            if (i < segment - 1) {
                indices.push_back(temp + i * segment + j);
                indices.push_back(temp + (i + 1) * segment + j + 1);
                indices.push_back(temp + i * segment + j + 1);

                indices.push_back(temp + i * segment + j);
                indices.push_back(temp + (i + 1) * segment + j);
                indices.push_back(temp + (i + 1) * segment + j + 1);
            }
            else {
                indices.push_back(temp + i * segment + j);
                indices.push_back(temp + j + 1);
                indices.push_back(temp + i * segment + j + 1);

                indices.push_back(temp + i * segment + j);
                indices.push_back(temp + j);
                indices.push_back(temp + j + 1);
            }
        }
    }

    temp *= 2;
    for (unsigned int i = 0; i < segment; i++) {
        if (i < segment - 1) {
            indices.push_back(temp + 2 * i );
            indices.push_back(temp + 2 * i + 3);
            indices.push_back(temp + 2 * i + 1);

            indices.push_back(temp + 2 * i);
            indices.push_back(temp + 2 * i + 2);
            indices.push_back(temp + 2 * i + 3);
        }
        else {
            indices.push_back(temp + 2 * i);
            indices.push_back(temp + 1);
            indices.push_back(temp + 2 * i + 1);

            indices.push_back(temp + 2 * i);
            indices.push_back(temp + 0);
            indices.push_back(temp + 1);
        }
    }

    /*      Line Indices     */
    for (unsigned int i = 0; i < segment; i++) {
        for (unsigned int j = 0; j < segment - 1; j++) {
            if (i < segment - 1) {
                lineIndices.push_back(i * segment + j);
                lineIndices.push_back(i * segment + j + 1);
                
                lineIndices.push_back(i* segment + j);
                lineIndices.push_back((i + 1) * segment + j);
                
                lineIndices.push_back(i * segment + j);
                lineIndices.push_back((i + 1) * segment + j + 1);
            }
            else {
                lineIndices.push_back(i * segment + j);
                lineIndices.push_back(i * segment + j + 1);
                
                lineIndices.push_back(i* segment + j);
                lineIndices.push_back(j);
               
                lineIndices.push_back(i * segment + j);
                lineIndices.push_back(j + 1);
            }
        }
    }

    temp = segment * segment;
    for (unsigned int i = 0; i < segment; i++) {
        for (unsigned int j = 0; j < segment - 1; j++) {
            if (i < segment - 1) {
                lineIndices.push_back(temp + i * segment + j);
                lineIndices.push_back(temp + (i + 1) * segment + j + 1);
                
                lineIndices.push_back(temp + (i + 1) * segment + j);
                lineIndices.push_back(temp + i * segment + j);
                
                lineIndices.push_back(temp + i * segment + j);
                lineIndices.push_back(temp + i * segment + j + 1);
            }
            else {
                lineIndices.push_back(temp + i * segment + j);
                lineIndices.push_back(temp + j + 1);
                
                lineIndices.push_back(temp + j);
                lineIndices.push_back(temp + i * segment + j);
              
                lineIndices.push_back(temp + i * segment + j);
                lineIndices.push_back(temp + i * segment + j + 1);
            }
        }
    }

    temp *= 2;
    for (unsigned int i = 0; i < segment; i++) {
        if (i < segment - 1) {
            lineIndices.push_back(temp + 2 * i);
            lineIndices.push_back(temp + 2 * i + 3);
            lineIndices.push_back(temp + 2 * i + 1);
            lineIndices.push_back(temp + 2 * i);
        }
        else {
            lineIndices.push_back(temp + 2 * i);
            lineIndices.push_back(temp + 1);
            lineIndices.push_back(temp + 2 * i + 1);
            lineIndices.push_back(temp + 2 * i);
        }
    }

    //std::cout << "Vertices size: " << vertices.size() << std::endl;
    //std::cout << "Indices size: " << indices.size() << std::endl;
    //std::cout << "LineIndices size: " << lineIndices.size() << std::endl;

    vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    ib->subData(0, indices.size() * sizeof(unsigned int), indices.data());
    ibl->subData(0, lineIndices.size() * sizeof(unsigned int), lineIndices.data());
}

unsigned int Capsule::getSegment(){
    return segment;
}

void Capsule::setRadius(const float radius){
    if (radius > 0) {
        for (unsigned int i = 0; i < 4 * segment * segment; i += 2) {
            vertices[i].x = (vertices[i].x / this->radius) * radius;
            if (i < 2 * segment * segment) {
                vertices[i].y -= height / 2.0f;
                vertices[i].y = (vertices[i].y / this->radius) * radius + height / 2.0f;
            }
            else {
                vertices[i].y += height / 2.0f;
                vertices[i].y = (vertices[i].y / this->radius) * radius - height / 2.0f;
            }

            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        for (unsigned int i = 4 * segment * segment; i < vertices.size(); i += 2) {
            vertices[i].x = vertices[i].x / this->radius * radius;
            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        this->radius = radius;
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

float Capsule::getRadius(){
    return radius;
}

void Capsule::setHeight(const float height){
    if (height >= 0) {
        for (unsigned int i = 0; i < 4 * segment * segment; i += 2) {
            if (i < 2 * segment * segment) 
                vertices[i].y = vertices[i].y - this->height / 2.0f + height / 2.0f;
            else 
                vertices[i].y = vertices[i].y + this->height / 2.0f - height / 2.0f;
        }

        for (unsigned int i = 4 * segment * segment; i < vertices.size(); i += 2) {
            if (i % 4 == 0) 
                vertices[i].y = vertices[i].y - this->height / 2.0f + height / 2.0f;
            else 
                vertices[i].y = vertices[i].y + this->height / 2.0f - height / 2.0f;
        }

        this->height = height;
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
    
}

float Capsule::getHeight(){
    return height;
}

void Capsule::setSize(const float radius, const float height){
    if (radius > 0) {
        for (unsigned int i = 0; i < 4 * segment * segment; i += 2) {
            vertices[i].x = (vertices[i].x / this->radius) * radius;
            if (i < 2 * segment * segment) {
                vertices[i].y -= height / 2.0f;
                vertices[i].y = (vertices[i].y / this->radius) * radius + height / 2.0f;
            }
            else {
                vertices[i].y += height / 2.0f;
                vertices[i].y = (vertices[i].y / this->radius) * radius - height / 2.0f;
            }

            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        for (unsigned int i = 4 * segment * segment; i < vertices.size(); i += 2) {
            vertices[i].x = vertices[i].x / this->radius * radius;
            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        this->radius = radius;
    }

    if (height >= 0) {
        for (unsigned int i = 0; i < 4 * segment * segment; i += 2) {
            if (i < 2 * segment * segment)
                vertices[i].y = vertices[i].y - this->height / 2.0f + height / 2.0f;
            else
                vertices[i].y = vertices[i].y + this->height / 2.0f - height / 2.0f;
        }

        for (unsigned int i = 4 * segment * segment; i < vertices.size(); i += 2) {
            if (i % 4 == 0)
                vertices[i].y = vertices[i].y - this->height / 2.0f + height / 2.0f;
            else
                vertices[i].y = vertices[i].y + this->height / 2.0f - height / 2.0f;
        }

        this->height = height;
    }

    if(radius > 0 || height >= 0)
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
}

void Capsule::setPosition(const Vec3 pos){
    this->pos = pos;
}

Vec3 Capsule::getPosition(){
    return pos;
}

void Capsule::setScale(const Vec3 scale){
    this->scale = scale;
}

Vec3 Capsule::getScale(){
    return scale;
}

void Capsule::rotate(Vec3 rot){
    rotation = rotation + rot;
}

void Capsule::setRotation(Vec3 rotation){
    this->rotation = rotation;
}

Vec3 Capsule::getRotation(){
    return rotation;
}

void Capsule::setColor(Vec3 color){
    this->color = color;
}

Vec3 Capsule::getColor(){
    return color;
}
