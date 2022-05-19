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
    string ssid = "";
    string status = "";
    vector<Edge> in;
    vector<Edge> out;

    void setEdgeName(string str){name = str;}
    string getEdgeName() { return name; }

    void setSSID(string str){ssid = str;}
    string getSSID() { return ssid; }

    void setStatus(string str){status = str;}
    string getStatus(){return status;}

    void push_out(Edge e) {
        out.push_back(e);
    }
    void push_in(Edge e) {
        in.push_back(e);
    }
};
using Graph = vector<Vertex>;

Vertex setVertex(string, string="", string="", string="");

int SearchVertex(Graph&, string );

int SearchEdge(vector<Edge>&, string );

void CheckMac(Graph&, string, string, string="out");

void PrintGraph(Graph&);

void GraphFunction(Graph&, string, string, string="");

void PrintNetworkGraph(Graph&);

void PrintEdgeParameters(Graph&, vector<Edge>&, Edge&, int, string);

#endif //GRAPH_H

