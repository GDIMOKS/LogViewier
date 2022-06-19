#include "Graph.h"
#pragma once

Vertex setVertex(string edgeName, string out, string in, string ssid)
{
    Vertex v;
    Edge e;
    e.count = 1;

    v.setEdgeName(edgeName);
    
    if (edgeName == "ff:ff:ff:ff:ff:ff")
    {
        v.setStatus("Broadcast");
    }

    if (ssid != "")
    {
        v.setSSID(ssid);
        v.setStatus("AccessPoint");
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
        if (g[i].getEdgeName() == edgeName)
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
    if (mac == "")
        return;

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

void PrintGraph(Graph& g)
{
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
            int iter = SearchVertex(g, t.value);
            //if (iter != 0)
            cout << v.getEdgeName() << " -> " << t.value << " : " << t.count << "\t" << v.getExtStatus() << "\t" /* << g[iter].getExtStatus() */ << endl;
        }
        //cout << "\n\n";
    }
}

void GraphFunction(Graph& g, string edgeName, string outMac, string ssid)
{
    int iter = SearchVertex(g, edgeName); // search current vertex

    if (iter == -1) // current vertex not found
    {
        if (edgeName != "")
            g.push_back(setVertex(edgeName, outMac, "", ssid));

        CheckMac(g, edgeName, outMac, "out");

        return;  
    }

    // current vertex found 
    Vertex& v = g[iter];
    
    if (ssid != "" && v.getSSID() == "")
    {
        v.setSSID(ssid);
        v.setStatus("AccessPoint");
    }

    if (outMac == "")
        return;
   
    Edge e;
    int vOut = SearchEdge(v.out, outMac);

    if (vOut == -1) // not found
    {
        e.value = outMac;
        e.count++;
        v.push_out(e);
        CheckMac(g, edgeName, outMac, "out");
    }
    else // found
    {
        v.out[vOut].count++;

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

void PrintEdgeParameters(Graph& g, vector<Edge>& v, Edge& e, int iterator, string mode)
{
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

    cout << "\t\t" << g[iterator].getStatus() << "  \t" << g[iterator].getExtStatus() << endl;
}

string SearchInDB(map<string, string>& db, string mac)
{
    string str;

    if (db.find(mac.substr(0, 8)) != db.end())
    {
        str = db.at(mac.substr(0, 8));
    }
    else if (db.find(mac.substr(0, 10)) != db.end())
    {
        str = db.at(mac.substr(0, 10));
    }
    else if (db.find(mac.substr(0, 13)) != db.end())
    {
        str = db.at(mac.substr(0, 13));
    }
    else
    {
        str = "-1";
    }

    return str;
}

void CheckDevice(Vertex& v, string networkSSID)
{
    if (v.getStatus() == "Broadcast")
    {
        v.setExtStatus("Broadcast");
        return;
    }
        
    map<string, string> drones {
        {"6C:DF:FB:E", "Fimi"},
        {"98:AA:FC:7", "Hubsan"},
        {"90:3A:E6", "Parrot"},
        {"00:12:1C", "Parrot"},
        {"00:26:7E", "Parrot"},
        {"90:03:B7", "Parrot"},
        {"A0:14:3D", "Parrot"},
        {"38:1D:14", "Skydio"},
        {"60:60:1F", "DJI"},
        {"10:A4:BE", "Syma"},
        {"20:F4:1B", "Wltoys"},//"Shenzhen Bilian electronic"}
        {"94:87:E0", "Xiaomi Communications Co Ltd"},
        {"B0:41:1D", "ITTIM Technologies"},
    };

    map<string, string> otherDevices{
        {"70:65:82", "Suzhou Hanming Technologies Co., Ltd."},
        {"7C:DD:76", "Suzhou Hanming Technologies Co., Ltd."},
        {"4C:5E:0C", "Routerboard.com"},
        {"30:05:3F", "Routerboard.com"},
        {"00:0C:42", "Routerboard.com"},
        {"08:55:31", "Routerboard.com"},
        {"18:FD:74", "Routerboard.com"},
        {"2C:C8:1B", "Routerboard.com"},
        {"48:8F:5A", "Routerboard.com"},
        {"64:D1:54", "Routerboard.com"},
        {"6C:3B:6B", "Routerboard.com"},
        {"74:4D:28", "Routerboard.com"},
        {"B8:69:F4", "Routerboard.com"},
        {"C4:AD:34", "Routerboard.com"},
        {"CC:2D:E0", "Routerboard.com"},
        {"D4:CA:6D", "Routerboard.com"}, 
        {"DC:2C:6E", "Routerboard.com"},
        {"E4:8D:8C", "Routerboard.com"},
        {"D8:FE:E3", "D-Link International"},
        {"C4:6E:1F", "TP-LINK TECHNOLOGIES CO.,LTD."},
        {"EC:56:23", "HUAWEI TECHNOLOGIES CO.,LTD"},
        {"88:DC:96", "EnGenius Technologies, Inc."},
        {"5C:E0:C5", "Intel Corporate"},
        {"98:AF:65", "Intel Corporate"},
        {"7C:7A:91", "Intel Corporate"},
        {"94:65:9C", "Intel Corporate"},
        {"60:67:20", "Intel Corporate"},
        {"48:F1:7F", "Intel Corporate"},
        {"48:A4:72", "Intel Corporate"},
        {"7C:B2:7D", "Intel Corporate"},
        {"88:A7:3C", "Ragentek Technology Group"},
        {"74:C6:3B", "AzureWave Technology Inc"},
        {"F0:7D:68", "D-Link Corporation"},
        {"10:BD:18", "Cisco Systems, Inc"},
        {"DA:A1:19", "Google, Inc"},
        {"88:A7:3C", "Ragentek Technology Group"},
        {"98:22:EF", "Liteon Technology Corporation"},
        /*{"08:EA:40","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"0C:8C:24","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"0C:CF:89","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"10:A4:BE","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"14:6B:9C","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"20:32:33","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"20:F4:1B","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"28:F3:66","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"0C:8C:24","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"0C:8C:24","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},
        {"0C:8C:24","SHENZHEN BILIAN ELECTRONIC CO.，LTD"},*/

    };
    
    string mac = ffunc::ToUpper(v.getEdgeName());
    string ssid = "";

    string venderName = SearchInDB(drones, mac);

    for (pair<string, string> drone : drones)
    {
        drone.second = ffunc::ToLower(drone.second);
        if (ffunc::ToLower(networkSSID).find(drone.second) != -1)
        {
            ssid = ffunc::ToLower(networkSSID);
            break;
        }
    }
    
    if (venderName != "-1" && ssid != "") // vender's name exists in list of drones and ssid mentioned in list
    {
        if (v.getStatus() == "AccessPoint")
        {
            v.setExtStatus("Drone " + venderName);
        }
        else if (v.getStatus() == "Client")
        {
            v.setExtStatus("Controller " + venderName);
        }

        return;
    }

    if (venderName != "-1")
    {
        if (v.getStatus() == "AccessPoint")
        {
            v.setExtStatus("Vendor not mentioned in SSID, Drone " + venderName);
        }
        else if (v.getStatus() == "Client")
        {
            v.setExtStatus("Vendor not mentioned in SSID, Controller " + venderName);
        }
        else
            v.setExtStatus("Drone/Controller " + venderName);
        
        return;
    }

    // vender' name not exists in list of drones
    string extStatus = SearchInDB(otherDevices, mac); 
    if (extStatus != "-1")
    {
        v.setExtStatus(extStatus);
    }
    else
    {
        //v.setStatus("Unknown");
        v.setExtStatus("Unknown device");
    }

}

void UpdateStatus(Vertex& v)
{
    if (v.getStatus() != "Broadcast")
        v.setStatus("Client");        
}

vector<Network> CreateNetworks(Graph& g)
{
    vector<Network> networks;
    for (Vertex& v : g)
    {
        if (v.getSSID() == "")
            continue;

        Network n;
        v.setStatus("AccessPoint");

        n.setName(v.getSSID());
        n.setMac(v.getEdgeName());

        CheckDevice(v, n.getName());
        n.g.push_back(v);

        for (Edge& t : v.out)
        {
            int iterOut = SearchVertex(g, t.value);
            UpdateStatus(g[iterOut]);
            CheckDevice(g[iterOut], n.getName());
            n.g.push_back(g[iterOut]);
        }

        for (Edge& t : v.in)
        {
            int exist = SearchVertex(n.g, t.value);
            if (exist == -1)
            {
                int iterIn = SearchVertex(g, t.value);
                UpdateStatus(g[iterIn]);
                CheckDevice(g[iterIn], n.getName());
                n.g.push_back(g[iterIn]);
            }

        }

        networks.push_back(n);
    }

    return networks;
}

void PrintNetworkGraph(Graph& g, vector<Network>& networks)
{
    vector<Vertex> others;

    for (Network& n : networks)
    {
        vector<string> outMacs;
        cout << "Сеть: " << n.getName() << " MAC-адрес: " << n.getMac() << endl;
        cout << "\tMAC-адрес\t\tВх. фреймов\tИсх. фреймов\tСтатус\t\tРасш.статус" << endl;

        for (Vertex& v : n.g)
        {
            if (v.getStatus() != "AccessPoint")
                continue;

            int inSum = 0;
            int outSum = 0;

            for (Edge& t : v.out)
            {
                outSum += t.count;
            }

            for (Edge& t : v.in)
            {
                inSum += t.count;
            }

            cout << "\t" << v.getEdgeName() << "\t" << inSum << "\t\t" << outSum << "\t\t" << v.getStatus() << "\t" << v.getExtStatus() << endl;
            for (Edge& t : v.out)
            {
                outMacs.push_back(t.value);
                int iterOut = SearchVertex(n.g, t.value);
                PrintEdgeParameters(n.g, v.in, t, iterOut, "out");
            }

            for (Edge& t : v.in)
            {
                if (find(outMacs.begin(), outMacs.end(), t.value) == outMacs.end())
                {
                    int iterIn = SearchVertex(n.g, t.value);
                    PrintEdgeParameters(n.g, v.out, t, iterIn, "in");
                }
            }
        }
        cout << endl;
        
    }

    for (Vertex& v : g)
    {
        if (v.getStatus() == "")
        {
            CheckDevice(v, "");
            others.push_back(v);
        }
    }

    if (!others.empty())
    {
        cout << "\nАдреса без сети:\n";
        for (Vertex& v : others)
        {
            for (auto& t : v.out)
            {
                int iter = SearchVertex(g, t.value);
                cout << v.getEdgeName() << " -> " << t.value << " : " << t.count << "\t1st mac: " << v.getExtStatus() << " \t2nd mac: " << g[iter].getExtStatus() << endl;
            }
        }
    }
    
}



