#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Edge {
    int count = 0;
    string value;
};
struct Vertex {
    string name;
    vector<Edge> in;
    vector<Edge> out;

    void setEdgeName(string str)
    {
        name = str;
    }

    void push_out(Edge e) {
        out.push_back(e);
    }
    void push_in(Edge e) {
        in.push_back(e);
    }
};
using Graph = vector<Vertex>;

Vertex setVertex(string , string, string);

int SearchVertex(Graph&, string );

int SearchEdge(vector<Edge>&, string );

void CheckMac(Graph&, string, string, string);

void PrintGraph(Graph&);

void GraphFunction(Graph&, string, string);

#endif //GRAPH_H

