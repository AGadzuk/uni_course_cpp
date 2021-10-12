#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <algorithm>

using namespace std;

class Vertex {
	public:
	int id;
	vector<int> edge_ids;
	int depth;

	Vertex(int x, vector<int> y, int d) {
		id = x;
		edge_ids = y;
		depth = d;
	}

};

class Edge {
	public:
	int id;
	vector<int> vertex_ids;
	string color;

	Edge(int x, vector<int> y, string c) {
		id = x;
		vertex_ids = y;
		color = c;
	}

};

vector<Vertex> graph_vertexs;
vector<Edge> graph_edges;
vector<Vertex> current_depth_vertexs;
vector<Vertex> temp_vec;
int depth, new_vertices_num;

bool isDenerated(double prob) {
	return (rand() % 100) <= prob;
}

void log() {
	for (int i = 0; i < graph_vertexs.size(); i++) {
		cout << "vertex: " << graph_vertexs[i].id << endl;
		cout << "depth: " << graph_vertexs[i].depth << endl;
		cout << "edge_ids: " << endl;
		for (int j = 0; j < graph_vertexs[i].edge_ids.size(); j++) {
			cout << graph_vertexs[i].edge_ids[j] << ' ';
		}
		cout << endl;
	}

	for (int i = 0; i < graph_edges.size(); i++) {
		cout << "edge: " << graph_edges[i].id << endl;
		cout << "color: " << graph_edges[i].color << endl;
		cout << "vertex_ids: " << endl;
		for (int j = 0; j < graph_edges[i].vertex_ids.size(); j++) {
			cout << graph_edges[i].vertex_ids[j] << ' ';
		}
		cout << endl;
	}

}

void Vertex_output(Vertex v, ofstream& file) {
	file << "{\n\t\t\t\"id\": " << v.id << ",\n\t\t\t\"edge_ids\": [";
	for (int i = 0; i < v.edge_ids.size(); i++) {
		if (v.edge_ids[i] > -1) {
			file << v.edge_ids[i] << ",";
		}
		else {
			break;
		}
	}
	file << "],\n\t\t\t\"depth\": " << v.depth << "\n\t\t";
}
void Edge_output(Edge e, ofstream& file) {
	file << "{\n\t\t\t\"id\": " << e.id << ",\n\t\t\t\"vertex_ids\": [";
	for (int i = 0; i < e.vertex_ids.size(); i++) {
		if (e.vertex_ids[i] > -1) {
			file << e.vertex_ids[i] << ",";
		}
		else {
			break;
		}
	}
	file << "],\n\t\t\t\"color\": " << e.color << "\n\t\t";
}

void Graph_output(vector<Vertex> v, vector<Edge> e, ofstream& file) {
	file << "{\n\t\"depth\" :" << depth;
	file << "\n\t\"vertices\": [\n\t\t";
	for (int i = 0; i < v.size(); i++) {
		if (v[i].id > -1) {
			Vertex_output(v[i], file);
			if (i != (v.size() - 1)) {
				file << ",\n\t\t";
			}
		}
		else {
			break;
		}
	}
	file << "],\n\t\"edges\": [\n\t\t";
	for (int i = 0; i < e.size(); i++) {
		if (e[i].id > -1) {
			Edge_output(e[i], file);
			if (i != (e.size() - 1)) {
				file << ",\n\t\t";
			}
		}
		else {
			break;
		}
	}
	file << "]\n}";
}

int main() {
	srand(time(0));
	cin >> depth >> new_vertices_num;

	Vertex root(0, {}, 0);
	graph_vertexs.push_back(root);
	current_depth_vertexs.push_back(root);

	for (int i = 0; i < depth; i++) {
		double step = 100 / depth;
		double prob = 100 - step * i;
		temp_vec.clear();
		for (int j = 0; j < current_depth_vertexs.size(); j++) {
			for (int k = 0; k < new_vertices_num; k++) {
				if (isDenerated(prob)) {
					int new_edge_id = graph_edges.size();
					int new_vertex_id = graph_vertexs.size();
					int current_vertex_id = current_depth_vertexs[j].id;
					graph_edges.push_back(Edge(new_edge_id, { current_vertex_id, new_vertex_id }, "gray"));
					Vertex newVertex(new_vertex_id, { new_edge_id }, i + 1);
					graph_vertexs.push_back(newVertex);
					temp_vec.push_back(newVertex);
					graph_vertexs[current_vertex_id].edge_ids.push_back(new_edge_id);
				}
			}
		}
		current_depth_vertexs = temp_vec;
	}

	for (int i = 0; i < graph_vertexs.size(); i++) {
		Vertex v = graph_vertexs[i];

		if (isDenerated(10)) {
			int new_edge_id = graph_edges.size();
			graph_edges.push_back(Edge(new_edge_id, {v.id}, "green"));
			graph_vertexs[i].edge_ids.push_back(new_edge_id);
		}
		if (isDenerated(25) && i != graph_vertexs.size()-1) {
			int new_edge_id = graph_edges.size();
			if (graph_vertexs[i + 1].depth == v.depth) {
				graph_edges.push_back(Edge(new_edge_id, { v.id, v.id + 1 }, "blue"));
				graph_vertexs[i].edge_ids.push_back(new_edge_id);
				graph_vertexs[i+1].edge_ids.push_back(new_edge_id);
			}
			else if(i != 0 && graph_vertexs[i - 1].depth == v.depth
				&& graph_vertexs[i - 1].edge_ids.back() != v.edge_ids.back()) {
				graph_edges.push_back(Edge(new_edge_id, { v.id - 1 , v.id }, "blue"));
				graph_vertexs[i - 1].edge_ids.push_back(new_edge_id);
				graph_vertexs[i].edge_ids.push_back(new_edge_id);
			}
		}
		if (v.depth < depth) {
			double prob = v.depth * (100 / (depth-1));

			if (isDenerated(prob)) {
				vector<Vertex> temp_deep_depth_vec;
				for (int j = i + 1; j < graph_vertexs.size(); j++) {
					Vertex current_v = graph_vertexs[j];
					if (current_v.depth == v.depth + 1 && find(v.edge_ids.begin(), v.edge_ids.end(), current_v.edge_ids[0]) == v.edge_ids.end()) {
						temp_deep_depth_vec.push_back(current_v);
					}
					if (current_v.depth == v.depth + 2) {
						break;
					}

				}
				if (!temp_deep_depth_vec.empty()) {
					int random_idx = rand() % temp_deep_depth_vec.size();
					int new_edge_id = graph_edges.size();
					graph_edges.push_back(Edge(new_edge_id, { graph_vertexs[i].id, temp_deep_depth_vec[random_idx].id }, "yellow"));
					graph_vertexs[i].edge_ids.push_back(new_edge_id);
					graph_vertexs[temp_deep_depth_vec[random_idx].id].edge_ids.push_back(new_edge_id);
				}
			}
		}
		if (v.depth < depth - 1 && isDenerated(33)) {
			vector<Vertex> temp_deep_depth_vec;
			for (int j = i + 1; j < graph_vertexs.size(); j++) {
				if (graph_vertexs[j].depth == v.depth + 2) {
					temp_deep_depth_vec.push_back(graph_vertexs[j]);
				}
				if (graph_vertexs[j].depth == v.depth + 3) {
					break;
				}

			}
			if (!temp_deep_depth_vec.empty()) {
				int random_idx = rand() % temp_deep_depth_vec.size();
				int new_edge_id = graph_edges.size();
				graph_edges.push_back(Edge(new_edge_id, { graph_vertexs[i].id, temp_deep_depth_vec[random_idx].id }, "red"));
				graph_vertexs[i].edge_ids.push_back(new_edge_id);
				graph_vertexs[temp_deep_depth_vec[random_idx].id].edge_ids.push_back(new_edge_id);
			}
		}

	}


	//log();

	ofstream gra;
	gra.open("graph.json");
	Graph_output(graph_vertexs, graph_edges, gra);
}
