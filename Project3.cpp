/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include<iostream>
#include <fstream>
#include<string>
#include<sstream>
#include<vector>
#include <utility>
#include <map>
#include <algorithm>
#include <fstream>
#include <tuple>
#include <cmath>
#include <queue>
#include <set>
using namespace std;


bool validateLocation(string location)
{
	return false;
}
class Graph
{
private:
	map<string, vector<pair<string, double>>> obj;
public:
	void insertEdge(string from, string to, double weight);
	void insertFromNeighbors(vector<tuple<string, double, double>>& neighbors);
	map<string, vector<pair<string, double>>>& getMap() {
		return obj;
	}
	void bellmanFord(Graph* graph, vector<tuple<string, double, double>>& V, int& E, string& src, string& destination);
};
void Graph::insertEdge(string from, string to, double weight)
{
	pair<string, double> toNode;
	toNode = make_pair(to, weight);
	obj[from].push_back(toNode);
}
void Graph::insertFromNeighbors(vector<tuple<string, double, double>>& neighbors) {
	for (int i = 0; i < neighbors.size(); i++) {
		//just make the neighbors the next one and previous one in the list
		//use pythagorean theorem to get distance as the crow flies
		double tokenWeight;
		if (i - 1 < 0) { //for first element in the file, only has one neighbor
			double a = pow(get<1>(neighbors[i]) - get<1>(neighbors[i + 1]), 2); //X-coordinates
			double b = pow(get<2>(neighbors[i]) - get<2>(neighbors[i + 1]), 2); //Y-coordinates
			tokenWeight = sqrt(a + b);
			this->insertEdge(get<0>(neighbors[i]), get<0>(neighbors[i + 1]), tokenWeight);
		}
		else if (i + 1 > 0) { //for last element in the file, only has one neighbor
			double a = pow(get<1>(neighbors[i]) - get<1>(neighbors[i - 1]), 2); //X-coordinates
			double b = pow(get<2>(neighbors[i]) - get<2>(neighbors[i - 1]), 2); //Y-coordinates
			tokenWeight = sqrt(a + b);
			this->insertEdge(get<0>(neighbors[i]), get<0>(neighbors[i - 1]), tokenWeight);
		}
		else { //every other station has 2 neighbors
			double a = pow(get<1>(neighbors[i]) - get<1>(neighbors[i + 1]), 2); //X-coordinates
			double b = pow(get<2>(neighbors[i]) - get<2>(neighbors[i + 1]), 2); //Y-coordinates
			tokenWeight = sqrt(a + b);
			this->insertEdge(get<0>(neighbors[i]), get<0>(neighbors[i + 1]), tokenWeight);


			a = pow(get<1>(neighbors[i]) - get<1>(neighbors[i - 1]), 2); //X-coordinates
			b = pow(get<2>(neighbors[i]) - get<2>(neighbors[i - 1]), 2); //Y-coordinates
			tokenWeight = sqrt(a + b);
			this->insertEdge(get<0>(neighbors[i]), get<0>(neighbors[i - 1]), tokenWeight);
		}
	}
}

void dijkstra(Graph& graph, string src, string destination, vector<tuple<string, double, double>> neigh)
{
	int vertices = neigh.size();
	double minWeight = 0;
	string minKey;
	map<string, double> distance(vertices, INT_MAX);
	set<string> visited(vertices, 0);
	distance[src] = 0;
	priority_queue<pair<string, double>, vector<pair<string, double>>, greater<pair<string, double>>> pq;
	pq.push(make_pair(src, 0));

	while (!pq.empty())
	{
		pair<string, double> p = pq.top();
		pq.pop();
		string curr = p.first;
		if (visited.find(curr) == visited.end())
			continue;
		visited.insert(curr);
		map<string, vector<pair<string, double>>>  adj = graph.getMap();


		for (int i = 0; i < adj[curr].size(); i++)
		{
			string dest = adj[curr][i].first;
			double weight = adj[curr][i].second;
			if (visited.find(dest) == visited.end())
				continue;
			if (distance[dest] > distance[curr] + weight)
			{
				distance[dest] = distance[curr] + weight;
				pq.push(make_pair(dest, distance[dest]));
			}
		}
	}

	cout << "Shortest path is: " << distance[destination] << endl;

}

void Graph::bellmanFord(Graph* graph, vector<tuple<string, double, double>>& V, int& E, string& src, string& destination) {
	// Step 1: Initialize the number of edges and vertices and create an array with the size of vertices
	unsigned int verticesCount = V.size();
	int edgesCount = E;
	map<string, double> distance;
	map<string, vector<pair<string, double>>> adjList = getMap();

	// Step 2: Initialize distances from source node to all other vertices as infinite
	for (int i = 0; i < verticesCount; i++) {
		distance[get<0>(V[i])] = INT_MAX;
	}

	distance[src] = 0;

	// Step 3: Find the simplest shortest path from source to any other vertex with at most |V| - 1 edges
	for (int i = 1; i <= verticesCount - 1; i++) {
		for (int j = 0; j < edgesCount; j++) {

			string u, v;
			double weight;

			// set source node
			for (auto& it : adjList) {
				u = it.first;
			}

			// set destination node
			v = destination;

			// set weight
			for (auto& it : adjList) {
				for (auto& k : it.second) {
					weight = k.second;
				}
			}

			if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
				distance[v] = distance[u] + weight;
			}
		}
	}

	// Step 4: Find negative-weight cycles: If there is a shorter path then there is a cycle
	for (int i = 0; i < edgesCount; i++) {
		string u, v;
		double weight;

		// set source node
		for (auto& it : adjList) {
			u = it.first;
		}

		// set destination node
		v = destination;

		// set weight
		for (auto& it : adjList) {
			for (auto& k : it.second) {
				weight = k.second;
			}
		}

		// If graph has a negative weight
		// Should not reach this point for this project, but in case of an error
		if (distance[u] != INT_MAX && distance[u] + weight < distance[destination]) {
			cout << "Error: Graph contains a negative weight cycle!";
			return;
		}
	}

	// Step 5: Print all the distances
	cout << "The shortest paths from the current location to electric car charging stations: ";
	cout << distance[destination] << endl;
	
}

int main()
{
	Graph stations = Graph();
	vector<tuple<string, double, double>> neighbors;

	ifstream myFile("FULL_Alternative_Excel.csv");
	string line;
	bool title = false;
	int counter = 0;
	while (getline(myFile, line))
	{
		istringstream ss(line);

		string axis;
		getline(ss, axis, ',');
		string x;
		getline(ss, x, ',');
		string y;
		getline(ss, y, ',');
		string id;
		getline(ss, id, ',');
		string fuel;
		getline(ss, fuel, ',');
		string streetaddress;
		getline(ss, streetaddress, ',');
		string city;
		getline(ss, city, ',');
		string state;
		getline(ss, state, ',');
		string zip;
		getline(ss, zip, ',');

		string addy = streetaddress + ", " + city + ", " + state + " " + zip;
		try {
			tuple<string, double, double>station = make_tuple(addy, stod(x), stod(y)); //make a tuple with address, X-coordinate, Y-coordinate
			neighbors.push_back(station);
		}
		catch (...) {
			continue;
		}

		//create a vector containing all the addresses and their coordinates
		title = true; //used to make sure the headings are not passed in as a parameter
	}

	//for (int i = 0; i < neighbors.size(); i++) {
	//	cout << get<0>(neighbors[i]) << ": " << get<1>(neighbors[i]) << ": " << get<2>(neighbors[i]) << endl;
	//}

	stations.insertFromNeighbors(neighbors);

	/*for (auto it = stations.getMap().begin(); it != stations.getMap().end(); it++) {
		cout << it->first << endl;
		for (int i = 0; i < it->second.size(); i++) {
			cout << it->second[i].first << " weight: " << it->second[i].second << endl;
		}

	}
	*/

	auto it = stations.getMap().begin();
	auto itt = stations.getMap().begin();
	itt++;
	itt++;

	dijkstra(stations, it->first, itt->first, neighbors);

	return 0;
}

















