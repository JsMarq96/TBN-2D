#include "collisions.h"


CollisionsManager::CollisionsManager() {
	event_calls = std::unordered_map<std::string, std::function<void(int,int)> >();
}

// Calculate the collisions and return an event stack, to be executed by the Scene
// TODO: Refactor and reduce complexity of this method, and posible paralelization for speedup
void CollisionsManager::calculate_collisions(std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> &object_areas,
											 std::array<Labels, MAX_ELEMENTS_PER_SCENE> &object_labels,
											 int obj_count,
											 std::stack<Collision> *col_stack) {
	for (int i = 0; i <= obj_count; i++) {
		for (int j = i+1; j <= obj_count; j++) {
			if (object_areas[i].is_overlaping(object_areas[j])) {
				// If they colide or overlap, check all the labels for the collisions
				for (std::string label1 : object_labels[i].labels) {
					if (label1.empty()) {
						continue;
					}
					for (std::string label2 : object_labels[j].labels) {
						if (label2.empty()) {
							continue;
						}
						// Add to the stack the requerired function
						std::string key = label1 + "-" + label2;
						if (event_calls.count(key)) {
							// Store the collision event with the objects's ids
							col_stack->push(Collision {event_calls[key], i, j});
						}
						// Repit the process with the inverted keys
						key = label2 + "-" + label2;
						if (event_calls.count(key)) {
							col_stack->push(Collision {event_calls[key], j, i});
						}
					}
				}
			}
		}
	}
}

// Calculate collision from a vector of object to the rest
void CollisionsManager::calculate_collisions_to(std::array<SceneSprite, MAX_ELEMENTS_PER_SCENE> &object_areas,
											 std::array<Labels, MAX_ELEMENTS_PER_SCENE> &object_labels,
											 std::vector<int> &from_objs,
											 int obj_count,
											 std::stack<Collision> *col_stack) {
	if (obj_count == 0)
		return;
	for (auto it=from_objs.begin(); it!=from_objs.end(); ++it) {
		int curr_index = *it;
		for (int j = 0; j <= obj_count; j++) {
			// If its the current object, or the collision label is empty,
			// skip ahead
			if (j == curr_index || object_labels[j].labels[0].empty()) {
				continue;
			}

			if (object_areas[curr_index].is_overlaping(object_areas[j])) {
				std::string label1 = object_labels[curr_index].labels[0];
				std::string label2 = object_labels[j].labels[0];
				// If they colide or overlap, check all the labels for the collisions
				// Add to the stack the requerired function
				std::string key = label1 + "-" + label2;
				if (event_calls.count(key)) {
					// Store the collision event with the objects's ids
					col_stack->push(Collision {event_calls[key], curr_index, j});
				}
				// Repit the process with the inverted keys
				key = label2 + "-" + label2;
				if (event_calls.count(key)) {
					col_stack->push(Collision {event_calls[key], j, curr_index});
				}
				for (std::string label1 : object_labels[curr_index].labels) {
					if (label1.empty()) {
						continue;
					}
					for (std::string label2 : object_labels[j].labels) {
						if (label2.empty()) {
							continue;
						}
						
					}
				}
			}
		}
	}
}

// Store an event with for two labels
void CollisionsManager::add_collision_event(std::function<void(int,int)> event,
											std::string label_1,
											std::string label_2) {
	std::string key = label_1 + "-" + label_2;
	event_calls[key] = event;
}
