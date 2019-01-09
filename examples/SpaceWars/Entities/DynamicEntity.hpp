#pragma once

#include "Entity.hpp"
#include "DrawableEntity.hpp"

class DynamicEntity : public DrawableEntity {
    rp3d::BoxShape *__shape = nullptr;
    rp3d::ProxyShape *__proxyShape = nullptr;
    glm::vec3 __scale;

    OpenGL::IBoundingBoxDrawable *__drawable = nullptr;
    OpenGL::PositionShaderProgram *__program = nullptr;

    void setBoundingBox() {
        auto box = __drawable->getBox();
        auto t = box.second - box.first;
        auto c = __scale;
        t.x *= c.x;
        t.y *= c.y;
        t.z *= c.z;

        const rp3d::Vector3 halfExtents(t.x, t.y, t.z);
        __shape = new rp3d::BoxShape(halfExtents);
        rp3d::decimal mass = rp3d::decimal(4.0);
        __proxyShape = __body->addCollisionShape(__shape, rp3d::Transform::identity(), mass);
    }

protected:
    rp3d::RigidBody *__body = nullptr;
public:
    DynamicEntity(
        OpenGL::PositionShaderProgram *program,
        OpenGL::IBoundingBoxDrawable *drawable,
        glm::vec3 scale): DrawableEntity(program, drawable, scale){
    }

    virtual void addToWorld(rp3d::DynamicsWorld *world, glm::vec3 pos) {
        auto transform = rp3d::Transform(rp3d::Vector3(pos.x, pos.y, pos.z), rp3d::Quaternion::identity());
        __body = world->createRigidBody(transform);
        __body->setType(rp3d::BodyType::DYNAMIC);
        setBoundingBox();
    }

    void draw() const override {
        glm::mat4 matrix;
        __body->getTransform().getOpenGLMatrix(&matrix[0][0]);
        matrix = glm::scale(matrix, __scale);
        __program->setModel(matrix);

        DrawableEntity::draw();
    }

};