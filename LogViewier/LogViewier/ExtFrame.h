#pragma once

#ifndef EXTFRAME_H
#define EXTFRAME_H

#include "Frame.h"
#include "FormatFunctions.h"

class ExtFrame : 
    public Frame
{
private:
    string mac;
    int frNumber;
    int seqNumber;
    string headerFlags;
    string rssi;
public:

    ExtFrame(Frame&, int, string);

    string getMac();
    void setMac(string);

    int getFrNum();
    void setFrNum(int);

    int getSeqNum();
    void setSeqNum(int);

    string getHFlags();
    void setHFlags(string);

    string getRssi();
    void setRssi(string);


    void Print();
};

#endif