#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "FormatFunctions.h"

using namespace std;

struct Edge {
    int count = 0;
    string value;

    /*bool operator==(string a) {
        return this->value == a;
    }

    bool operator==(Edge& a) {
        return this->value == a.value;
    }*/
};

struct Vertex {
private:
    string name;
    string ssid = "";
    string status = "";
    string extendedStatus = "";
public:
    Vertex() {

    }
    Vertex(string edgeName, string out="", string in="", string ssid="") {
        Edge e;
        e.count = 1;

        setEdgeName(edgeName);

        if (edgeName == "ff:ff:ff:ff:ff:ff")
        {
            setStatus("Broadcast");
        }

        if (ssid != "")
        {
            setSSID(ssid);
            setStatus("AccessPoint");
        }

        if (out != "")
        {
            e.value = out;
            push_out(e);
        }

        if (in != "")
        {
            e.value = in;
            push_in(e);
        }
    }

    vector<Edge> in;
    vector<Edge> out;

    void setEdgeName(string str){name = str;}
    string getEdgeName() { return name; }

    void setSSID(string str){ssid = str;}
    string getSSID() { return ssid; }

    void setStatus(string str){status = str;}
    string getStatus(){return status;}

    void setExtStatus(string str) { extendedStatus = str; }
    string getExtStatus() { return extendedStatus; }

    void push_out(Edge e) {
        out.push_back(e);
    }
    void push_in(Edge e) {
        in.push_back(e);
    }
};

using Graph = vector<Vertex>;

struct Network {
private:
    string name = "";
    string macAP = "";
public:
    Graph g;

    void setName(string str) { name = str; }
    string getName() { return name; }

    void setMac(string str) { macAP = str; }
    string getMac() { return macAP; }
};

int Search(Graph&, string);

int Search(vector<Edge>&, string);

void CheckMac(Graph&, string, string, string="out");

void PrintGraph(Graph&);

void GraphFunction(Graph&, string, string, string="");

void PrintNetworkGraph(Graph&, vector<Network>&);

void PrintEdgeParameters(Graph&, vector<Edge>&, Edge&, int, string);

void PrintEdgeParameters(Graph&, Vertex& v);

void CheckDevice(Vertex&, string);

void UpdateStatus(Vertex&);

string SearchInDB(map<string, string>&, string);

vector<Network> CreateNetworks(Graph&);

#endif //GRAPH_H

