#include "ExtFrame.h"

ExtFrame::ExtFrame()
{
    seqNumber = 
    frNumber = 0;
}

ExtFrame::ExtFrame(Frame& f){//, string interArrivalTime) {
    mac = f.GetAddress(f.getFrameHex(), 20);
    setSize(f.getSize());
    //setOffset(interArrivalTime);
    setOffset(f.getOffset());
    headerFlags = ffunc::HexToBin(f.getFrameHex().substr(2, 2));

    string seqControl;
    string seqControlBin;

    if (f.getType() == 0 || f.getType() == 2)
        seqControl = f.getFrameHex().substr(46, 2) + f.getFrameHex().substr(44, 2);
    else if (f.getType() == 1)
        seqControl = f.getFrameHex().substr(34, 2) + f.getFrameHex().substr(32, 2);

    seqControlBin = ffunc::HexToBin(seqControl);

    seqNumber = ffunc::BinToDec(seqControlBin.substr(0, 12));
    frNumber = ffunc::BinToDec(seqControlBin.substr(12, 4));
}

string ExtFrame::getMac() { return mac; }
void ExtFrame::setMac(string value) { mac = value; }

int ExtFrame::getFrNum() { return frNumber; }
void ExtFrame::setFrNum(int value) { frNumber = value; }

int ExtFrame::getSeqNum() { return seqNumber; }
void ExtFrame::setSeqNum(int value) { seqNumber = value; }

string ExtFrame::getHFlags() { return headerFlags; }
void ExtFrame::setHFlags(string value) { headerFlags = value; }

string ExtFrame::getRssi() { return rssi; }
void ExtFrame::setRssi(string value) { rssi = value; }

void ExtFrame::Print()
{
    cout << "mac: " << mac << endl;
    cout << "fragment number: " << frNumber << endl;
    cout << "sequence number: " << seqNumber << endl;
    cout << "header flags: " << headerFlags << endl;
    cout << "size: " << getSize() << endl;
    cout << "offset: " << getOffset() << endl;
    cout << endl;
}

bool cmp(ExtFrame& a, ExtFrame& b)
{
    if (a.getSeqNum() < b.getSeqNum()) return true;
    if (a.getSeqNum() > b.getSeqNum()) return false;
    if (a.getFrNum() < b.getFrNum()) return true;
    if (a.getFrNum() > b.getFrNum()) return false;
    if (a.getHFlags() < b.getHFlags()) return true;
    if (a.getHFlags() > b.getHFlags()) return false;
}

bool operator==(ExtFrame a, ExtFrame b)
{
    if (a.getMac() == b.getMac() && a.getSize() == b.getSize() &&
        a.getOffset() == b.getOffset() && a.getHFlags() == b.getHFlags() &&
        a.getSeqNum() == b.getSeqNum() && a.getFrNum() == b.getFrNum())
        return true;
    else
        return false;
}