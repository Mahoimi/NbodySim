#include <NBodySim.hpp>

int main(){
    const int WIDTH = 1600;
    const int HEIGHT = 900;

    NBodySim simulation;
    simulation.init(WIDTH,HEIGHT);
    simulation.run();

	return EXIT_SUCCESS;
}
