#include "Graph.h"
#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;


Vertex setVertex(string edgeName, string out, string in, string ssid)
{
    Vertex v;
    Edge e;
    e.count = 1;

    v.setEdgeName(edgeName);

    
    if (edgeName == "ff:ff:ff:ff:ff:ff")
    {
        v.setStatus("broadcast");
    }

    if (ssid != "")
    {
        v.setSSID(ssid);
        v.setStatus("access_point");
    }

    if (out != "")
    {
        e.value = out;
        v.push_out(e);
    }

    if (in != "")
    {
        e.value = in;
        v.push_in(e);
    }


    return v;
}

int SearchVertex(Graph& g, string edgeName)
{
    int iter = -1;

    for (int i = 0; i < g.size(); i++) //check for vertex exists
    {
        if (g[i].name == edgeName)
        {
            iter = i;
            break;
        }
    }

    return iter;
}

int SearchEdge(vector<Edge>& v, string mac)
{
    int iter = -1;

    for (int i = 0; i < v.size(); i++) //check for edge exists
    {
        if (v[i].value == mac)
        {
            iter = i;
            break;
        }
    }
    //auto iter = find(v.begin(), v.end(), mac);
    return iter;
}

void CheckMac(Graph& g, string edgeName, string mac, string mode)
{
    if (mac != "")
    {
        Edge e;
        e.value = edgeName;
        e.count++;

        int iter = SearchVertex(g, mac); // search vertex in graph
        if (iter != -1) // found
        {
            if (e.value == "")
                return;

            if (mode == "in")
                g[iter].push_out(e); // set out edge for incoming vertex
            else if (mode == "out")
                g[iter].push_in(e);

        }
        else // not found
        {
            if (mode == "in")
                g.push_back(setVertex(mac, edgeName)); // add incoming vertex in graph
            else if (mode == "out")
                g.push_back(setVertex(mac, "", edgeName));
        }
    }
}

void PrintGraph(Graph& g)
{
    //cout << "Остальные адреса:\n";
    for (Vertex& v : g)
    {
        //cout << "Name : \t" << v.name << endl;
        /*cout << "in : ";
        for (auto& t : v.in)
        {
            cout << "\t" << t.value << " " << t.count << endl;
        }
        cout << endl;
        cout << "out : ";*/
        for (auto& t : v.out)
        {
            cout << v.name << " -> " << t.value << " : " << t.count <<  endl;
        }
        //cout << "\n\n";
    }
}

void PrintEdgeParameters(Graph& g, vector<Edge>& v, Edge& e, int iterator, string mode)
{

    if (e.value != "ff:ff:ff:ff:ff:ff")
        g[iterator].setStatus("Client");
    else
        g[iterator].setStatus("Broadcast");

    cout << "\t" << e.value << "\t";

    int iter = SearchEdge(v, e.value);
        
    if (mode == "out")
    {
        cout << e.count << "\t\t";

        if (iter != -1)
            cout << v[iter].count;
        else
            cout << 0;
    }     
    else if (mode == "in")
    {
        if (iter == -1)
            cout << 0 << "\t\t";
        else
            cout << v[iter].count << "\t\t";

        cout << e.count;
    }

    cout << "\t\t" << g[iterator].getStatus() << endl;
}

void PrintNetworkGraph(Graph& g)
{
    vector<Vertex> others;

    for (Vertex& v : g)
    {
        if (v.getSSID() != "")
        {
            v.setStatus("AccessPoint");

            cout << "Сеть: " << v.getSSID() << " MAC-адрес: " << v.getEdgeName() << endl;
            cout << "\tMAC-адрес\t\tВх. фреймов\tИсх. фреймов\tСтатус" << endl;
            for (Edge& t : v.out)
            {
                int iterOut = SearchVertex(g, t.value);

                PrintEdgeParameters(g, v.in, t, iterOut, "out");
            }

            for (Edge& t : v.in)
            {
                int iterIn = SearchVertex(g, t.value);

                if (g[iterIn].getStatus() == "")
                {
                    PrintEdgeParameters(g, v.out, t, iterIn, "in");
                }
                           
            }
        } 
    }

    for (Vertex& v : g)
    {
        if (v.getStatus() == "")// || v.getStatus() == "Broadcast")
        {
            others.push_back(v);
        }
    }

    if (!others.empty())
    {
        cout << "\nАдреса без сети:\n";
        PrintGraph(others);
    }

    
    
}

void GraphFunction(Graph& g, string edgeName, string outMac, string ssid)
{
    //Graph g;
    //string edgeName = "";
    //string outMac = "";
    //string inMac = "";

    int iter = SearchVertex(g, edgeName); // search current vertex

    if (iter != -1) // current vertex found
    {
        if (ssid != "" && g[iter].ssid == "")
        {
            g[iter].ssid = ssid;
        }

        if (outMac != "")
        {
            Edge e;
            int vOut = SearchEdge(g[iter].out, outMac);

            if (vOut == -1) // not found
            {
                e.value = outMac;
                e.count++;
                g[iter].push_out(e);
                CheckMac(g, edgeName, outMac, "out");
            }
            else // found
            {
                g[iter].out[vOut].count++;

                auto iter1 = SearchVertex(g, outMac);

                auto vIn = SearchEdge(g[iter1].in, edgeName);
                if (vIn == -1) // not found
                {
                    e.value = edgeName;
                    e.count++;
                    g[iter1].push_in(e);
                }
                else
                {
                    g[iter1].in[vIn].count++;
                }

            }
        }
    }
    else // current vertex not found 
    {
        if (edgeName != "")
            g.push_back(setVertex(edgeName, outMac, "", ssid));

        CheckMac(g, edgeName, outMac, "out");

    }


    //return g;
    //PrintGraph(g);
}

