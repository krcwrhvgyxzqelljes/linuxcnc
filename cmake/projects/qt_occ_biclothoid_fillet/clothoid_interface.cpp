#include "clothoid_interface.h"
#include "chrono"

clothoid_interface::clothoid_interface()
{

}

void clothoid_interface::build_clothoid(clothoid &in){

    auto start = std::chrono::high_resolution_clock::now();
    std::cout.precision(15);

    // Faster algo :
    // Clothoid::buildClothoid(in.p0.X(), in.p0.Y(), in.theta0, in.p1.X(), in.p1.Y(), in.theta1, in.k, in.dk, in.L);

    // SLower algo, but more info :
    Clothoid::buildClothoid(in.p0.X(), in.p0.Y(), in.theta0, in.p1.X(), in.p1.Y(), in.theta1,
                            in.k, in.dk, in.L, in.k_1, in.dk_1, in.L_1, in.k_2, in.dk_2, in.L_2);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // Convert duration to milliseconds with higher precision
    double duration_in_milliseconds = duration.count() / 1e6;

    std::cout << "Build duration: " << std::fixed << std::setprecision(9) << duration_in_milliseconds << " milliseconds" << std::endl;
}

void clothoid_interface::eval_clothoid(clothoid &in){

    auto start = std::chrono::high_resolution_clock::now();
    std::cout.precision(15);

    // std::cout << "k: " << k << ", dk: " << dk << ", len: " << len << "\n";
    double interval = in.L/(in.nr_of_points - 1);
    // file output will be a json array
    // std::cout << "{\"length\": "<<len<<", \"dk\": "<<dk<<", \"points\": ";
    // std::cout << "[";
    for (int i = 0; i < in.nr_of_points; i++) {
        double t = i * interval;
        double intC = 0.0;
        double intS = 0.0;
        Clothoid::GeneralizedFresnelCS(in.dk * pow(t, 2.0), in.k*t, in.theta0, intC, intS);
        gp_Pnt pi={in.p0.X() + t * intC, in.p0.Y() + t * intS, 0};

        double pointK = in.k + in.dk*t;
        // std::cout<<"distance:"<<t<<" x:"<<pi.X()<<" y:"<<pi.Y()<<" curvature:"<<pointK<<std::endl;

        in.pvec.push_back(pi);

        if (i < in.nr_of_points- 1) {
            // std::cout << ",\n";
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // Convert duration to milliseconds with higher precision
    double duration_in_milliseconds = duration.count() / 1e6;

    std::cout << "Evaluate duration: " << std::fixed << std::setprecision(9) << duration_in_milliseconds << " milliseconds" << std::endl;
}

// Hope we get this working.
gp_Pnt clothoid_interface::interpolate_clothoid_point_displacement(clothoid &in, double s){

    double intC = 0.0;
    double intS = 0.0;
    Clothoid::GeneralizedFresnelCS(in.dk * pow(s, 2.0), in.k*s, in.theta0, intC, intS);
    gp_Pnt pi={in.p0.X() + s * intC, in.p0.Y() + s * intS, 0};
    // double pointK = in.k + in.dk*s;
    // std::cout<<"progress:"<<s<<" x:"<<pi.X()<<" y:"<<pi.Y()<<" curvature:"<<pointK<<std::endl;

    return pi;
}

gp_Pnt clothoid_interface::interpolate_clothoid_point_progress(clothoid &in, double progress){

    auto start = std::chrono::high_resolution_clock::now();
    std::cout.precision(15);

    double s=progress*in.L;
    // std::cout<<"s:"<<s<<std::endl;
    // std::cout<<"L:"<<in.L<<std::endl;
    gp_Pnt pi=interpolate_clothoid_point_displacement(in,s);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    // Convert duration to milliseconds with higher precision
    double duration_in_milliseconds = duration.count() / 1e6;

    std::cout << "Interpolate duration: " << std::fixed << std::setprecision(9) << duration_in_milliseconds << " milliseconds" << std::endl;

    return pi;
}





















