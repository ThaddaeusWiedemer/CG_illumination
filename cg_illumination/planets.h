#include <map>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

/*Material material_sun(glm::vec3(1.f), glm::vec3(0.f), glm::vec3(0.f), 0, 1);
Material material_planet(glm::vec3(0.3f), glm::vec3(1.f), glm::vec3(0.f), 0, 1);

struct Planet{
	float r;
	float r_orbit;
	float tilt;
	float inclination;
	float spin;
	float v_orbit;
	Material material;
	Texture texture;
};*/

std::map<std::string, float> sun = {
	{"r_orbit", 0.},
	{"inclination", 0.},
	{"r", 695500 / 2. / 5.}, //5 times smaller than in reality
	{"tilt", 0.},
	{"spin", 0.},
	{"v_orbit", 360. / 576.}
};

std::map<std::string, float> mercury = {
	{"r_orbit", 57909050.},
	{"inclination", 7},
	{"r", 2439.7},
	{"tilt", 0.1},
	{"spin", 360 / 4222.6},
	{"v_orbit", 360 / 2111.28}
};

std::map<std::string, float> venus = {
	{"r_orbit", 108208000.},
	{"inclination", 3.39},
	{"r", 6051.8},
	{"tilt", 177},
	{"spin", -360 / 2802.},
	{"v_orbit", 360 / 5392.8}
};

std::map<std::string, float> earth = {
	{"r_orbit", 149598261.},
	{"inclination", 0.},
	{"r", 6378.},
	{"tilt", 23},
	{"spin", 360 / 24.},
	{"v_orbit", 360 / 8782.08}
};

std::map<std::string, float> mars = {
	{"r_orbit", 227939100.},
	{"inclination", 1.85},
	{"r", 3393.50},
	{"tilt", 25},
	{"spin", 360 / 24.66},
	{"v_orbit", 360 / 16487.52}
};

std::map<std::string, float> jupiter = {
	{"r_orbit", 778547200.},
	{"inclination", 1.31},
	{"r", 71400.},
	{"tilt", 3},
	{"spin", 360 / 9.93},
	{"v_orbit", 360 / 103987.68}
};

std::map<std::string, float> saturn = {
	{"r_orbit", 1433449370.},
	{"inclination", 2.49},
	{"r", 60000.},
	{"tilt", 27},
	{"spin", 360 / 10.66},
	{"v_orbit", 360 / 258136.8}
};

std::map<std::string, float> uranus = {
	{"r_orbit", 2876679082.},
	{"inclination", 0.77},
	{"r", 25600.},
	{"tilt", 98},
	{"spin", -360 / 17.24},
	{"v_orbit", 360 / 736491.6}
};

std::map<std::string, float> neptune = {
	{"r_orbit", 4503443661.},
	{"inclination", 1.77},
	{"r", 24300.},
	{"tilt", 30},
	{"spin", 360 / 16.11},
	{"v_orbit", 360 / 1444560.72}
};

std::vector<std::map<std::string, float>> planets = {sun, mercury, venus, earth, mars, jupiter, saturn, uranus, neptune};