#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue>  // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"

using namespace std;

double INF = numeric_limits<double>::max();
using json = nlohmann::json;

class prioritize {
 public:
  bool operator()(const pair<double,long long>& p1,
                  const pair<double, long long>& p2) const {
    return p1.first > p2.first;
  }
};
void buildGraph(istream& input, graph<long long, double>& g,
                vector<BuildingInfo>& buildings) {
  json inputJson;

  input >> inputJson;

  const double THRESHOLD = 0.036;

  for (const auto& building : inputJson["buildings"]) {
    long long getID = building["id"];

    double getLat = building["lat"];

    double getLon = building["lon"];

    string getAbbr = building["abbr"];

    string getName = building["name"];

    Coordinates location(getLat, getLon);

    BuildingInfo build(getID, location, getName, getAbbr);

    buildings.push_back(build);

    g.addVertex(getID);
  }

  unordered_map<long long, Coordinates> wayPointsCheck;

  for (const auto& wayPoints : inputJson["waypoints"]) {
    long long getID = wayPoints["id"];

    double getLat = wayPoints["lat"];

    double getLon = wayPoints["lon"];

    Coordinates location(getLat, getLon);

    wayPointsCheck[getID] = location;

    g.addVertex(getID);
  }

  for (const auto& building : buildings) {
    for (const auto& [waypointId, waypointCoord] : wayPointsCheck) {
      double dist = distBetween2Points(building.location, waypointCoord);

      if (dist <= THRESHOLD) {
        g.addEdge(building.id, waypointId, dist);

        g.addEdge(waypointId, building.id, dist);
      }
    }
  }

  for (const auto& footway : inputJson["footways"]) {
    if (footway.size() < 2) {
      continue;
    }

    for (size_t i = 0; i + 1 < footway.size(); ++i) {
      long long from = footway[i];

      long long to = footway[i + 1];

      Coordinates CoordOne;

      if (wayPointsCheck.count(from)) {
        CoordOne = wayPointsCheck[from];
      }

      else {
        for (const auto& building : buildings) {
          if (building.id == from) {
            CoordOne = building.location;

            break;
          }
        }
      }

      Coordinates CoordTwo;

      if (wayPointsCheck.count(to)) {
        CoordTwo = wayPointsCheck[to];

      } else {
        for (const auto& building : buildings) {
          if (building.id == to) {
            CoordTwo = building.location;

            break;
          }
        }
      }

      if (CoordOne.lat == 0 && CoordOne.lon == 0) {
        throw std::runtime_error("Node ID not found: " + std::to_string(from));
      }

      if (CoordTwo.lat == 0 && CoordTwo.lon == 0) {
        throw std::runtime_error("Node ID not found: " + std::to_string(to));
      }

      double calcWeight = distBetween2Points(CoordOne, CoordTwo);

      g.addEdge(from, to, calcWeight);

      g.addEdge(to, from, calcWeight);
    }
  }
}

BuildingInfo getBuildingInfo(const vector<BuildingInfo>& buildings,
                             const string& query) {
  for (const BuildingInfo& building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo>& buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo& building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double>& G, long long start,
                           long long target,
                           const set<long long>& ignoreNodes) {
  const double INF = numeric_limits<double>::max();
  unordered_map<long long, double> distances;
  unordered_map<long long, long long> prev;
  unordered_set<long long> visited;
  for (const auto& vertex : G.getVertices()) {
    distances[vertex] = INF;
  }
  distances[start] = 0; 
 
  priority_queue<pair<double, long long>, vector<pair<double, long long>>,
                 prioritize>list;

  list.push({0.0, start});
  while (!list.empty()) {
    auto [currentDistance, currentVertex] = list.top();
    list.pop();
    if (visited.count(currentVertex)) {
      continue;
    }
    visited.insert(currentVertex);
    if (currentVertex == target) {
      break;
    }
    if (ignoreNodes.count(currentVertex) && currentVertex != start &&
        currentVertex != target) {
      continue;
    }
    for (const auto& neighbor : G.neighbors(currentVertex)) {
      double weight;
      if (!G.getWeight(currentVertex, neighbor, weight)) {
        continue;
      }
      double newDistance = currentDistance + weight;
      if (newDistance < distances[neighbor]) {
        distances[neighbor] = newDistance;
        prev[neighbor] = currentVertex;
        list.push({newDistance, neighbor});
      }
    }
  }
  vector<long long> path;
  if (distances[target] == INF) {
    return path;
  }
  for (long long at = target; at != start; at = prev[at]) {
    path.push_back(at);
  }
  path.push_back(start);
  reverse(path.begin(), path.end());
  return path;
}

double pathLength(const graph<long long, double>& G,
                  const vector<long long>& path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long>& path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

void application(const vector<BuildingInfo>& buildings,
                 const graph<long long, double>& G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto& building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
