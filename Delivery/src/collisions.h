#ifndef _collisions_h_
#define _collisions_h_

#include <unordered_map>
#include <array>
#include <stack>
#include <vector>
#include <functional>
#include "framework.h"
#include "scenesprite.h"

// Label object, for managing the collisions
class Labels {
public:
	std::array<std::string, 4> labels;
	bool has_label(std::string label);
};
/*
	Not needed
// Struct that wrapps the pointers to the scene elmetns to pass to the collision functions
struct SceneStateWrapper {
    std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> *object;
    std::array<bool, MAX_ELEMENTS_PER_SCENE> *enabled;
    std::array<Labels, MAX_ELEMENTS_PER_SCENE> *labels;
};
*/
struct Collision {
	std::function<void(int,int)> event;
	int id_1;
	int id_2;
};

class CollisionsManager {
private:
	std::unordered_map<std::string, std::function<void(int,int)>> event_calls;
	
public:
	CollisionsManager();
	
	void calculate_collisions(std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> &object_areas,
							  std::array<Labels, MAX_ELEMENTS_PER_SCENE> &object_labels,
							  int count,
							  std::stack<Collision> *col_stack);

	void calculate_collisions_to(std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> &object_areas,
											 std::array<Labels, MAX_ELEMENTS_PER_SCENE> &object_labels,
											 std::vector<int> &from_objs,
											 int obj_count,
											 std::stack<Collision> *col_stack);
	
	void add_collision_event(std::function<void(int,int)> event,
							 std::string label_1,
							 std::string label_2);
};



#endif