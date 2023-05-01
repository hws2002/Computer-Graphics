#ifndef CURVE_HPP
#define CURVE_HPP

#include <vecmath.h>

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "object3d.hpp"

// TODO (PA3): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in
// all the data.
using namespace std;
struct CurvePoint {
    Vector3f V;  // Vertex
    Vector3f T;  // Tangent  (unit)
};

class Curve : public Object3D {
   protected:
    std::vector<Vector3f> controls;

   public:
    explicit Curve(std::vector<Vector3f> points)
        : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override { return false; }

    std::vector<Vector3f> &getControls() { return controls; }

    virtual void discretize(int resolution, std::vector<CurvePoint> &data)  = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto &control : controls) {
            glVertex3fv(control);
        }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto &control : controls) {
            glVertex3fv(control);
        }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto &cp : sampledPoints) {
            glVertex3fv(cp.V);
        }
        glEnd();
        glPopAttrib();
    }
    void pad() {
        int tSize = t.size();
        tpad.resize(tSize + k);
        for (int i = 0; i < tSize; ++i) tpad[i] = t[i];
        for (int i = 0; i < k; ++i) tpad[i + tSize] = t.back();
    }
    int n, k;
    std::vector<double> t;
    std::vector<double> tpad;
    int NCP, degree;
    std::vector<double> Knots;
    double range[2];
};

class BezierCurve : public Curve {
   public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        n = controls.size();
        k = n - 1;
        range[0] = 0;
        range[1] = 1;
        t.resize(2 * n);
        for (int i = 0; i < n; ++i) {
            t[i] = 0;
            t[i + n] = 1;
        }
        pad();
    }
    void discretize(int resolution, std::vector<CurvePoint>& data) override{
    }
};

class BsplineCurve : public Curve {
   public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf(
                "Number of control points of BspineCurve must be more than "
                "4!\n");
            exit(0);
        }
                    NCP = points.size(); //number of control points
        degree = 3; // pre-determined
        Knots.resize(NCP+degree+1);
        for(int i=0;i <NCP+degree+1;i++){
            Knots[i] = (double)i/(NCP+degree);
        }

        n = controls.size();
        k = 3;
        t.resize(n + k + 1);
        for (int i = 0; i < n + k + 1; ++i) t[i] = (double)i / (n + k);
        pad();
        range[0] = t[k];
        range[1] = t[n];
    }
    void discretize(int resolution, std::vector<CurvePoint>& data) override{
        int num_data = resolution * (NCP - degree); //most of the situation, it would be resolution * (4-3)

        //debug
        // std::cout<<"Number of points in this discretization is "<<num_data<<std::endl;

        data.resize(num_data);

        //debug
        // for(int i=0; i<NCP+degree+1;i++){
        //     std::cout<<"knots["<<i<<"] is "<<Knots[i]<<std::endl;
        // }

        for (int i=degree;i<NCP;i++) {
            // get the index of Control points, that are effective at this iteration.
            int Valid[4];
            
            for(int j = 0; j<=degree;j++){
                Valid[j] = i-degree+j;
                //debug
                // cout<<"Valid["<<j<<"] is "<<Valid[j]<<endl;
            }
            
            //debug
            // cout<<"Knots[i] is "<<Knots[i]<<endl;
            // cout<<"Knots[i+1] is "<<Knots[i+1]<<endl;

            for(int j=0;j<resolution;j++){
                data[(i-degree)*resolution + j].V = Vector3f::ZERO;
                // Calculate t
                double step = (Knots[i+1] - Knots[i]) * ((double)j/(resolution)) + Knots[i];

                // debug
                std::cout<<"step is "<<step<<endl;
                // Dynamic programming, get Bernsteins
                //  - for t in t_i to t_{i+1}, Bernsteins are only determined by the B_i,0
                std::vector<double> Bernsteins(degree+1,0);
                for(int l = 0; l < Bernsteins.size();l++){
                    Bernsteins[l] = get_Bernstein(Valid[l],degree,step);

                    //debug
                    std::cout<<"Params are  "<<Valid[l]<<" "<<degree<<" "<<step<<endl;
                    std::cout<<Bernsteins[l]<<endl;
                }

                // ∑ 
                std::cout<<"Point for this iteration is..(in data index)";
                std::cout<<(i-degree)*resolution + j<<endl;
                for (int l = 0; l < Bernsteins.size(); ++l) {
                    data[(i-degree)*resolution + j].V += controls[Valid[l]] * Bernsteins[l];
                    // data[i].T += controls[-lsk + j] * ds[j];
                }

                //debug
                std::cout<<data[(i-degree)*resolution + j].V<<endl;
            }
        }
        // std::cout<<"Points are..."<<endl;
        // for(int i=0;i<num_data;i++){
        //     data[i].V = Vector3f(3+i,0,0);
        //     std::cout<<data[i].V<<endl;
        // }
        // std::cout<<endl;
    }
protected:
    double get_Bernstein(int i,int p,double t){
        if(p <= 0){
            if(t >= Knots[i] && t <Knots[i+1])
                return 1;
            else 
                return 0;
        } else {
            double a = (t - Knots[i])/(Knots[i+p] - Knots[i]);
            double b = (Knots[i+p+1] - t)/(Knots[i+p+1] - Knots[i+1]);
            return a*get_Bernstein(i,p-1,t) + b*get_Bernstein(i+1,p-1,t);
        }
    }
};

#endif  // CURVE_HPP
