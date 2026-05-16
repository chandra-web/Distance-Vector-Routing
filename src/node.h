#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 999;

/*
  Each row in the table will have these fields
  dstip: Destination IP address
  nexthop: Next hop on the path to reach dstip
  ip_interface: nexthop is reachable via this interface
  cost: total path cost
*/

class RoutingEntry {
public:
    string dstip, nexthop;
    string ip_interface;
    int cost;
};

/*
  Sorting routing table entries
*/

class Comparator {
public:
    bool operator()(const RoutingEntry &R1, const RoutingEntry &R2) {

        if (R1.cost == R2.cost) {
            return R1.dstip.compare(R2.dstip) < 0;
        }

        return R1.cost < R2.cost;
    }
};

/*
  Routing Table
*/

struct routingtbl {
    vector<RoutingEntry> tbl;
};

/*
  Route Message
*/

class RouteMsg {
public:
    string from;
    struct routingtbl *mytbl;
    string recvip;
};

/*
  Network Interface
*/

class NetInterface {

private:
    string ip;
    string connectedTo;
    int linkCost;

public:

    string getip() {
        return this->ip;
    }

    string getConnectedIp() {
        return this->connectedTo;
    }

    int getCost() {
        return this->linkCost;
    }

    void setCost(int c) {
        this->linkCost = c;
    }

    void setip(string ip) {
        this->ip = ip;
    }

    void setConnectedip(string ip) {
        this->connectedTo = ip;
    }
};

/*
  Node Structure
*/

class Node {

private:
    string name;

    vector<pair<NetInterface, Node*> > interfaces;

protected:

    struct routingtbl mytbl;

    virtual void recvMsg(RouteMsg* msg) {
        cout << "Base" << endl;
    }

    bool isMyInterface(string eth) {

        for (int i = 0; i < interfaces.size(); ++i) {

            if (interfaces[i].first.getip() == eth)
                return true;
        }

        return false;
    }

public:

    void setName(string name) {
        this->name = name;
    }

    /*
      Add interface
    */

    void addInterface(string ip,
                      string connip,
                      int cost,
                      Node *nextHop) {

        NetInterface eth;

        eth.setip(ip);
        eth.setConnectedip(connip);
        eth.setCost(cost);

        interfaces.push_back({eth, nextHop});
    }

    /*
      Add routing table entry
    */

    void addTblEntry(string myip, int cost) {

        RoutingEntry entry;

        entry.dstip = myip;
        entry.nexthop = myip;
        entry.ip_interface = myip;
        entry.cost = cost;

        mytbl.tbl.push_back(entry);
    }

    /*
      Update routing table entry
    */

    void updateTblEntry(string dstip, int cost) {

        for (int i = 0; i < mytbl.tbl.size(); i++) {

            if (mytbl.tbl[i].dstip == dstip) {

                mytbl.tbl[i].cost = cost;
            }
        }
    }

    /*
      Get link cost to neighbor
    */

    int getLinkCost(string neighip) {

        for (int i = 0; i < interfaces.size(); i++) {

            if (interfaces[i].first.getConnectedIp() == neighip) {

                return interfaces[i].first.getCost();
            }
        }

        return INF;
    }

    /*
      Remove failed link
    */

    void removeLink(string neighip) {

        for (int i = 0; i < interfaces.size(); i++) {

            if (interfaces[i].first.getConnectedIp() == neighip) {

                interfaces.erase(interfaces.begin() + i);
                break;
            }
        }
    }

    string getName() {
        return this->name;
    }

    struct routingtbl getTable() {
        return mytbl;
    }

    /*
      Print routing table
    */

    void printTable() {

        Comparator myobject;

        sort(mytbl.tbl.begin(),
             mytbl.tbl.end(),
             myobject);

        cout << this->getName() << ":" << endl;

        for (int i = 0; i < mytbl.tbl.size(); ++i) {

            cout << mytbl.tbl[i].dstip
                 << " | "
                 << mytbl.tbl[i].nexthop
                 << " | "
                 << mytbl.tbl[i].ip_interface
                 << " | "
                 << mytbl.tbl[i].cost
                 << endl;
        }
    }

    /*
      Send routing updates
      WITH POISONED REVERSE
    */

    void sendMsg() {

        for (int i = 0; i < interfaces.size(); ++i) {

            struct routingtbl *ntbl = new routingtbl;

            string neighborIP =
                interfaces[i].first.getConnectedIp();

            /*
              Build neighbor-specific routing table
            */

            for (int j = 0; j < mytbl.tbl.size(); ++j) {

                RoutingEntry entry = mytbl.tbl[j];

                /*
                  Poisoned Reverse
                */

                if (entry.nexthop == neighborIP &&
                    entry.dstip != neighborIP) {

                    entry.cost = INF;
                }

                ntbl->tbl.push_back(entry);
            }

            RouteMsg msg;

            msg.from =
                interfaces[i].first.getip();

            msg.recvip =
                neighborIP;

            msg.mytbl = ntbl;

            interfaces[i].second->recvMsg(&msg);
        }
    }
};

/*
  Routing Node
*/

class RoutingNode : public Node {

public:
    void recvMsg(RouteMsg *msg);
}; 