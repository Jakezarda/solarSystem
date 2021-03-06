#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "include/planet.hpp"

std::vector<planet> getPlanets(std::string file) {
    double G = 5.3039E-11;
    double M_Sun = 334875;
    std::vector<planet> planets;
    std::ifstream fin(file);
    while (!fin.eof()) {
        std::string name;
        std::vector<double> pos(3);
        std::vector<double> vel(3);
        double mass;
        fin >> name;
        for (int i = 0; i < 3; ++i)
            fin >> pos[i];
        for (int i = 0; i < 3; ++i)
            fin >> vel[i];
        fin >> mass;
        if (!fin.eof()) {
            planet p(name, pos, vel, mass, M_Sun, G);
            planets.push_back(p);
        }
    }
    return planets;
}

void checkPlanets(std::vector<planet> &planets) {
    for (size_t i = 0; i < planets.size(); ++i) {
        std::vector<double> pos = planets[i].getPos();
        std::vector<double> vel = planets[i].getVel();
        std::cout << planets[i].getName() << " " << pos[0] << " " << pos[1] << " " << pos[2] << " " << vel[0] << " " << vel[1] << " " << vel[2] << " " << planets[i].getMass() << "\n";
    }
}

int main(int argc, char *argv[]) {
    double h = 0.5;                               //back of the envelope calculation, each timestep roughly 6 hours
    int N = 584000;                               //started at 3250; 584000 time steps should be 200 years then
    for (int i = 1; i < argc; ++i) {
        std::string temp(argv[i]);
        if (temp == "-h") {
            h = std::stod(argv[i+1]);
            ++i;
        } else if (temp == "-N") {
            N = std::stoi(argv[i+1]);
            ++i;
        }
    }
    
    std::vector<planet> planets = getPlanets("planetData2.dat");
    checkPlanets(planets);
    
    std::ofstream fout("simulationTest.dat");
    fout.precision(15);
    fout << "0 ";
    for (size_t p = 0; p < planets.size(); ++p) {
        std::vector<double> pos = planets[p].getPos();
        std::vector<double> vel = planets[p].getVel();
        fout << pos[0] << " " << pos[1] << " " << pos[2] << " " << vel[0] << " " << vel[1] << " " << vel[2] << " ";
    }
    fout << "\n";
    for (int i = 0; i < N; ++i) {
        double t = h + h*i;
        fout << t << " ";
        for (size_t p = 0; p < planets.size(); ++p) {
            planets[p].update(t, h, planets);
            std::vector<double> pos = planets[p].getPos();
            std::vector<double> vel = planets[p].getVel();
            fout << pos[0] << " " << pos[1] << " " << pos[2] << " " << vel[0] << " " << vel[1] << " " << vel[2] << " ";
        }
        fout << "\n";
    }
    fout.close();
    
    return 0;
}
