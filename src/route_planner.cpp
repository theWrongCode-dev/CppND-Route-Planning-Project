#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
  start_node=&m_Model.FindClosestNode(start_x,start_y);
  end_node=& m_Model.FindClosestNode(end_x,end_y);

}


float RoutePlanner::CalculateHValue(RouteModel::Node const *node){
  //std::cout<<node->distance(*end_node);
  return node->distance(*end_node);
}



void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
  current_node->FindNeighbors();
   for(auto v : current_node->neighbors){
     // set the parent to current node
     v->parent = current_node;
     // set the h_value from the calculate h value function
     v->h_value = CalculateHValue(v);
     v->g_value = current_node->g_value + current_node->distance(*v); 
     open_list.push_back(v);
     v->visited = true;
   }
}



bool Compare(const RouteModel::Node *a, const RouteModel::Node *b) {

float f1 = a->g_value + a->h_value;

float f2 = b->g_value + b->h_value;

return f1 > f2;

}


RouteModel::Node *RoutePlanner::NextNode() {
  std::sort(open_list.begin(), open_list.end(), Compare);

RouteModel::Node *lowest_node = open_list.back();

open_list.pop_back();

return lowest_node;
}



std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;

    // TODO: Implement your solution here.
    std::vector<RouteModel::Node> path_found;
    while (current_node != start_node) {
        distance += current_node->distance(*current_node->parent);
        path_found.push_back(*current_node);
        current_node = current_node->parent;
    }
  path_found.push_back(*current_node);
    std::reverse(path_found.begin(), path_found.end());

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}



void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
  start_node->visited = true;
    open_list.push_back(start_node);
    
    current_node = start_node;
    while (current_node != end_node) {
        AddNeighbors(current_node);
        current_node = NextNode();
    }
    m_Model.path = ConstructFinalPath(current_node);

}