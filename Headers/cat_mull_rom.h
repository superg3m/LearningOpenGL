#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

#include <glm/glm.hpp>

class CMRSpline {
public:
    std::vector<glm::vec3> splinePoints;
    std::vector<glm::vec3> distanceVec;
    std::vector<glm::vec3> controlPoints;
    std::vector<int> meshAmount;
    void addControlPoints(glm::vec3 point)
    {
   
        controlPoints.push_back(point);
        
    }
    void drawSpline()
    {
        if (controlPoints.empty())
        {
            addControlPoints(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z - 2));
        }
        else
        {
            float length = abs(glm::length(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z - 2)) - glm::length(controlPoints[controlPoints.size() - 1]));
            std::cout << "length: " << length << std::endl;
            if (length > 0.2)
            {
                addControlPoints(glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z - 2));
            }
        }
    }
    void calculate_number_of_meshes_per_spline(Model modelObject)
    {
        this->meshAmount.clear();
        this->number_of_splines = controlPoints.size() - 1;
        for (int i = 0; i < this->number_of_splines; i++)
        {
            this->meshAmount.push_back(15 + (2 * (glm::length(controlPoints[i] - controlPoints[i + 1]) / (modelObject.height))));
        }
    }
    void calculateSplinePoints(bool pause)
    {
        if (!pause)
        {
            this->splinePoints.clear();
            this->distanceVec.clear();
        }
        this->number_of_splines = controlPoints.size() - 1;
        if (this->number_of_splines > 0 && !pause)
        {
            glm::vec3 ghostStart = 2.0f * controlPoints[0] - controlPoints[1];
            glm::vec3 ghostEnd = 2.0f * controlPoints.back() - controlPoints[controlPoints.size() - 2];
            for (int i = 0; i < number_of_splines; i++)
            {
                for (int j = 0; j < this->meshAmount[i]; j++)
                {
                    this->splinePoints.push_back(CatmullRom(ghostStart, controlPoints[i], controlPoints[i + 1], ghostEnd, j / (float)this->meshAmount[i]));
                    //splinePoints.push_back(splineObject.CatmullRom(points[0], points[i], points[i + 1], points[points.size() - 1], j / (float)amount));
                }
            }
            for (unsigned int i = 0; i < this->splinePoints.size() - 1; i++)
            {
                this->distanceVec.push_back(this->splinePoints[i + 1] - this->splinePoints[i]);
            }
            this->distanceVec.push_back(-1.0f * this->distanceVec[this->distanceVec.size() - 1]);
        }
    }
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
    
    int number_of_splines;
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