#include "Cylinder.h"
#include <iostream>

Cylinder::Cylinder(){
    init();
}

Cylinder::Cylinder(const Vec3 pos, const float radius, const float height) : pos(pos){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;

    init();
}

Cylinder::Cylinder(const Vec3 pos, const Vec3 scale, const Vec3 rotation) : pos(pos),scale(scale),rotation(rotation){
    init();
}

Cylinder::Cylinder(const float radius, const float height, const unsigned int segment){
    if (radius > 0)
        this->radius = radius;
    if (height > 0)
        this->height = height;
    if (segment >= 3 && segment <= 100)
        this->segment = segment;
    init();
}

Cylinder::~Cylinder(){
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

void Cylinder::init(){
    program = new Program("Shaders/Cone.shader");

    vb = new VertexBuffer(nullptr, sizeof(Vec3) * 804, GL_STATIC_DRAW);

    ib = new IndexBuffer(nullptr, sizeof(unsigned int) * 1200, GL_STATIC_DRAW);
    ibl = new IndexBuffer(nullptr, sizeof(unsigned int) * 1200, GL_STATIC_DRAW);

    va.bind();
    vb->bind();

    va.addVertexAttribute(3, GL_FLOAT, false);
    va.addVertexAttribute(3, GL_FLOAT, false);

    setSegment(segment);
}

void Cylinder::draw(Camera& camera){
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

void Cylinder::setDrawMode(DrawMode mode){
    this->mode = mode;
}

void Cylinder::setSegment(const unsigned int segment){
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

    vertices.push_back({ 0,height / 2.0f,0 });
    vertices.push_back({0,0,1});
    vertices.push_back({ 0,-height / 2.0f,0 });
    vertices.push_back({0,1,0});

    for (unsigned int j = 0; j < 3; j++) {
        for (unsigned int i = 0; i < segment; i++) {
            float radian = i * 2.0f * PI / (float)segment;
            Vec3 pos;

            pos.x = cosf(radian) * radius;
            pos.z = sinf(radian) * radius;
            
            if (j == 0) {
                pos.y = height / 2.0f;
                vertices.push_back(pos);
                vertices.push_back({ 1,1,1 });
            }
            else if (j == 1) {
                pos.y = -height / 2.0f;
                vertices.push_back(pos);
                vertices.push_back({ 0,0,0 });
            }
            else {
                pos.y = height / 2.0f;
                vertices.push_back(pos);
                vertices.push_back({ 1,1,1 });

                pos.y = -height / 2.0f;
                vertices.push_back(pos);
                vertices.push_back({ 0,0,0 });
            }
        }
    }

    for (unsigned int j = 0; j < 3; j++) {
        for (unsigned int i = 0; i < segment; i++) {
            if (j == 0) {
                if (i < segment - 1) {
                    indices.push_back(0);
                    indices.push_back(2 + i + 1);
                    indices.push_back(2 + i);
                }
                else {
                    indices.push_back(0);
                    indices.push_back(2);
                    indices.push_back(2 + i);
                }
            }
            else if (j == 1) {
                if (i < segment - 1) {
                    indices.push_back(1);
                    indices.push_back(2 + segment + i);
                    indices.push_back(2 + segment + i + 1);
                }
                else {
                    indices.push_back(1);
                    indices.push_back(2 + segment + i);
                    indices.push_back(2 + segment); 
                }
            }
            else {
                if (i < segment - 1) {
                    indices.push_back(2 + 2 * segment + 2 * i);
                    indices.push_back(2 + 2 * segment + 2 * i + 3);
                    indices.push_back(2 + 2 * segment + 2 * i + 1);

                    indices.push_back(2 + 2 * segment + 2 * i);
                    indices.push_back(2 + 2 * segment + 2 * i + 2);
                    indices.push_back(2 + 2 * segment + 2 * i + 3);
                }
                else {
                    indices.push_back(2 + 2 * segment + 2 * i);
                    indices.push_back(2 + 2 * segment + 1);
                    indices.push_back(2 + 2 * segment + 2 * i + 1);

                    indices.push_back(2 + 2 * segment + 2 * i);
                    indices.push_back(2 + 2 * segment + 0);
                    indices.push_back(2 + 2 * segment + 1);
                }   
            }
        }
    }

    for (unsigned int j = 0; j < 3; j++) {
        for (unsigned int i = 0; i < segment; i++) {
            if (j == 0) {
                if (i < segment - 1) {
                    lineIndices.push_back(0);
                    lineIndices.push_back(2 + i + 1);
                    lineIndices.push_back(2 + i + 1);
                    lineIndices.push_back(2 + i);
                }
                else {
                    lineIndices.push_back(0);
                    lineIndices.push_back(2);
                    lineIndices.push_back(2);
                    lineIndices.push_back(2 + i);
                }
            }
            else if (j == 1) {
                if (i < segment - 1) {
                    lineIndices.push_back(1);
                    lineIndices.push_back(2 + segment + i);
                    lineIndices.push_back(2 + segment + i);
                    lineIndices.push_back(2 + segment + i + 1);
                }
                else {
                    lineIndices.push_back(1);
                    lineIndices.push_back(2 + segment + i);
                    lineIndices.push_back(2 + segment + i);
                    lineIndices.push_back(2 + segment);
                }
            }
            else {
                if (i < segment - 1) {
                    lineIndices.push_back(2 + 2 * segment + 2 * i);
                    lineIndices.push_back(2 + 2 * segment + 2 * i + 3);
                    lineIndices.push_back(2 + 2 * segment + 2 * i + 1);
                    lineIndices.push_back(2 + 2 * segment + 2 * i);
                }
                else {
                    lineIndices.push_back(2 + 2 * segment + 2 * i);
                    lineIndices.push_back(2 + 2 * segment + 1);
                    lineIndices.push_back(2 + 2 * segment + 2 * i + 1);
                    lineIndices.push_back(2 + 2 * segment + 2 * i);
                }
            }
        }
    }

    //std::cout << "Vertices size: " << vertices.size() << std::endl;
    //std::cout << "Indices size: " << indices.size() << std::endl;
    //std::cout << "LineIndices size: " << lineIndices.size() << std::endl;

    vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    ib->subData(0, indices.size() * sizeof(unsigned int), indices.data());
    ibl->subData(0, lineIndices.size() * sizeof(unsigned int), lineIndices.data());
}

unsigned int Cylinder::getSegment(){
    return segment;
}

void Cylinder::setRadius(const float radius){
    if (radius > 0) {
        for (unsigned int i = 4; i < vertices.size(); i += 2) {
            vertices[i].x = vertices[i].x / this->radius * radius;
            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        this->radius = radius;

        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

float Cylinder::getRadius(){
    return radius;
}

void Cylinder::setHeight(const float height){
    if (height > 0) {
        vertices[0].y = height / 2.0f;
        vertices[2].y = -height / 2.0f;

        for (unsigned int j = 0; j < 3; j++) {
            for (unsigned int i = 0; i < segment; i++) {
                if (j == 0) {
                    vertices[4 + 2 * i].y = height / 2.0f;
                }
                else if (j == 1) {
                    vertices[4 + 2 * segment + 2 * i].y = -height / 2.0f;
                }
                else {
                    vertices[4 + 4 * segment + 4 * i].y = height / 2.0f;
                    vertices[4 + 4 * segment + 4 * i + 2].y = -height / 2.0f;
                }
            }
        }

        this->height = height;
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }   
}

float Cylinder::getHeight(){
    return height;
}

void Cylinder::setSize(const float radius, const float height){
    if (radius > 0) {
        for (unsigned int i = 4; i < vertices.size(); i += 2) {
            vertices[i].x = vertices[i].x / this->radius * radius;
            vertices[i].z = vertices[i].z / this->radius * radius;
        }

        this->radius = radius;
    }

    if (height > 0) {
        vertices[0].y = height / 2.0f;
        vertices[2].y = -height / 2.0f;

        for (unsigned int j = 0; j < 3; j++) {
            for (unsigned int i = 0; i < segment; i++) {
                if (j == 0) {
                    vertices[4 + 2 * i].y = height / 2.0f;
                }
                else if (j == 1) {
                    vertices[4 + 2 * segment + 2 * i].y = -height / 2.0f;
                }
                else {
                    vertices[4 + 4 * segment + 4 * i].y = height / 2.0f;
                    vertices[4 + 4 * segment + 4 * i + 2].y = -height / 2.0f;
                }
            }
        }

        this->height = height;
    }

    if (radius > 0 || height > 0) {
        vb->subData(0, vertices.size() * sizeof(Vec3), (float*)vertices.data());
    }
}

void Cylinder::setPosition(const Vec3 pos){
    this->pos = pos;
}

Vec3 Cylinder::getPosition(){
    return pos;
}

void Cylinder::setScale(const Vec3 scale){
    this->scale = scale;
}

Vec3 Cylinder::getScale(){
    return scale;
}

void Cylinder::rotate(Vec3 rot){
    rotation = rotation + rot;
}

void Cylinder::setRotation(Vec3 rotation){
    this->rotation = rotation;
}

Vec3 Cylinder::getRotation(){
    return rotation;
}

void Cylinder::setColor(Vec3 color){
    for (unsigned int i = 0; i < vertices.size(); i += 2) {
        vertices[i + 1] = color;
    }

    this->color = color;
}

Vec3 Cylinder::getColor(){
    return color;
}
