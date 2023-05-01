#ifndef CURVE_HPP
#define CURVE_HPP

#include <vecmath.h>

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "object3d.hpp"
// #define DEBUG

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
    int NCP, degree; // Numbe of control points, and degree of curve
};

class BezierCurve : public Curve {
   public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        NCP = points.size(); //number of control points = NCP
        degree = NCP-1; // k = n = NCP-1
        // Knots.resize(NCP+degree+1);
        // for(int i=0;i <= degree;i++){
            // Knots[i] = 0;
        // }
        // for(int i=NCP; i< Knots.size();i++){
            // Knots[i] = 1;
        // }

    }
    void discretize(int resolution, std::vector<CurvePoint>& data) override{
        int num_data = resolution; //most of the situation, it would be resolution * (4-3)
        data.resize(num_data);
        for(int i=0; i<resolution;i++){
            data[i].V = Vector3f::ZERO;
            data[i].T = Vector3f::ZERO;
            double step = double(i)/resolution;
            
            #ifdef DEBUG
            std::cout<<i+1<<"th step is "<<step<<std::endl;
            #endif

            //Calculate the Bernsteins
            std::vector<double> Bernsteins(NCP,0);
            std::vector<double> DBernsteins(NCP,0);
            for(int j=0; j<=NCP-1;j++){
                Bernsteins[j] = get_Bernstein(j,step);
                #ifdef DEBUG
                std::cout<<"Bernsteins["<<j<<"] is "<<Bernsteins[j]<<std::endl;
                #endif
            } 
            // Calculate the Derivatives
            for(int j=0; j<=NCP-1;j++){
                // DBernsteins[j] = ;
            }
            // ∑
            for(int j=0; j<=NCP-1;j++){
                data[i].V += controls[j] * Bernsteins[j];
                data[i].T += controls[j] * DBernsteins[j];
            }
        }
    }
protected:
    double get_Bernstein(int i, double t){
        int n = NCP-1;
        return C(n,i) * pow(t,i) * pow(1-t,n-i);
    }

    double C(int n,int i){
        if(n==i || i==0)
            return 1;
        return factorial(n)/(factorial(i)*factorial(n-i));
    }

    double factorial(int n){
        if(n<=1) return 1;

        double res = 1;
        while(n>1){
            res *= n;
            n--;
        }
        return res;
    }
};

class BsplineCurve : public Curve {
public:
    std::vector<double> Knots;

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
    }
    void discretize(int resolution, std::vector<CurvePoint>& data) override{
        int num_data = resolution * (NCP - degree); //most of the situation, it would be resolution * (4-3)

        //debug
        #ifdef DEBUG
            std::cout<<"Number of points in this discretization is "<<num_data<<std::endl;
        #endif

        data.resize(num_data);

        //debug
        #ifdef DEBUG
        for(int i=0; i<NCP+degree+1;i++){
            std::cout<<"knots["<<i<<"] is "<<Knots[i]<<std::endl;
        }
        #endif

        for (int i=degree;i<NCP;i++) {
            // get the index of Control points, that are effective at this iteration.
            // int Valid[4];
            int * Valid = new int[degree+1];
            
            for(int j = 0; j<=degree;j++){
                Valid[j] = i-degree+j;
                #ifdef DEBUG
                    cout<<"Valid["<<j<<"] is "<<Valid[j]<<endl;
                #endif
            }
            
            //debug
            #ifdef DEBUG
                cout<<"Knots[i] is "<<Knots[i]<<endl;
                cout<<"Knots[i+1] is "<<Knots[i+1]<<endl;
            #endif
            for(int j=0;j<resolution;j++){
                data[(i-degree)*resolution + j].V = Vector3f::ZERO;
                // Calculate t
                double step = (Knots[i+1] - Knots[i]) * ((double)j/(resolution)) + Knots[i];

                // debug
                #ifdef DEBUG
                    std::cout<<"step is "<<step<<endl;
                #endif
                // Dynamic programming, get Bernsteins
                //  - for t in t_i to t_{i+1}, Bernsteins are only determined by the B_i,0
                std::vector<double> Bernsteins(degree+1,0);
                for(int l = 0; l < Bernsteins.size();l++){
                    Bernsteins[l] = get_Bernstein(Valid[l],degree,step);

                    //debug
                    #ifdef DEBUG
                        std::cout<<"Params are  "<<Valid[l]<<" "<<degree<<" "<<step<<endl;
                        std::cout<<Bernsteins[l]<<endl;
                    #endif
                }

                // ∑ 

                //debug
                #ifdef DEBUG
                    std::cout<<"Point for this iteration is..(in data index)";
                    std::cout<<(i-degree)*resolution + j<<endl;
                #endif
                for (int l = 0; l < Bernsteins.size(); ++l) {
                    data[(i-degree)*resolution + j].V += controls[Valid[l]] * Bernsteins[l];
                    // data[i].T += controls[-lsk + j] * ds[j];
                }

                //debug
                #ifdef DEBUG
                    std::cout<<data[(i-degree)*resolution + j].V<<endl;
                #endif
            }
            delete [] Valid;
        }
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
