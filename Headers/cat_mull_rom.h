#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

#include <glm/glm.hpp>

class CMRSpline {
public:
    static glm::vec3 CatmullRom(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t /* between 0 and 1 */, float alpha = .5f /* between 0 and 1 */) {
        float t0 = 0.0f;
        float t1 = GetT(t0, alpha, p0, p1);
        float t2 = GetT(t1, alpha, p1, p2);
        float t3 = GetT(t2, alpha, p2, p3);

        t = lerp(t1, t2, t);

        glm::vec3 A1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
        glm::vec3 A2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
        glm::vec3 A3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;
        glm::vec3 B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2;
        glm::vec3 B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3;

        glm::vec3 C = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2;
        return C;
    }
private:
    static float lerp(float a, float b, float t) {
        return a + ((b - a) * t);
    }
    static float GetT(float t, float alpha, const glm::vec3& p0, const glm::vec3& p1)
    {
        auto d = p1 - p0;
        float a = glm::dot(d, d); // Dot product
        float b = pow(a, alpha * .5f);
        return (b + t);
    }
};