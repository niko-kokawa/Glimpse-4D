#include "Space.h"

#include <cmath>

Rot4 Rot4::Identity()
{
    Rot4 r{};
    for (int i = 0; i < 4; i++)
        r.m[i][i] = 1.f;
    return r;
}

Rot4 Rot4::Plane(int a, int b, float angle)
{
    Rot4 r = Identity();
    float c = cos(angle);
    float s = sin(angle);

    r.m[a][a] = c;
    r.m[a][b] = -s;
    r.m[b][a] = s;
    r.m[b][b] = c;

    return r;
}

Vec4 Rot4::rotate(const Vec4& v) const
{
    return {
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
        m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
    };
}

Rot4 Rot4::operator*(const Rot4& r) const
{
    Rot4 o{};
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                o.m[i][j] += m[i][k] * r.m[k][j];
    return o;
}


void rotateYaw(Vec3& d, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float x = d.x * c - d.z * s;
    float z = d.x * s + d.z * c;

    d.x = x;
    d.z = z;
}

void rotatePitch(Vec3& d, float angle) {
    float c = cos(angle);
    float s = sin(angle);

    float y = d.y * c - d.z * s;
    float z = d.y * s + d.z * c;

    d.y = y;
    d.z = z;
}

void normalize(Vec3& v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    v.x /= len;
    v.y /= len;
    v.z /= len;
}