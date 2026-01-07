#pragma once

struct Vec3 {
    float x, y, z;
};

struct Vec4 {
    float x, y, z, w;

    Vec4 operator-(const Vec4& other) const {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
            w - other.w
        };
    }
};


// A Camera Controller
struct CameraController {
    float radius = 4.0f;
    Vec3 direction = { 0.f, 0.f, 1.f }; // looking from +Z
    float w = -4.0f;
};

// A rotor in geometric algebra
struct Rot4 {
    float m[4][4];

    static Rot4 Identity();
    static Rot4 Plane(int a, int b, float angle);

    Vec4 rotate(const Vec4& v) const;
    Rot4 operator*(const Rot4& r) const;
};


void rotateYaw(Vec3& d, float angle);
void rotatePitch(Vec3& d, float angle);
void normalize(Vec3& v);