#include "node.h"
#include <iostream>

using namespace std;

/*
  Print Routing Tables
*/

void printRT(vector<RoutingNode*> nd) {

    for (int i = 0; i < nd.size(); i++) {

        nd[i]->printTable();
    }
}

/*
  Distance Vector Routing Algorithm
*/

void routingAlgo(vector<RoutingNode*> nd) {

    /*
      Multiple rounds of updates
    */

    for (int round = 0; round < nd.size(); ++round) {

        for (RoutingNode* node : nd) {

            node->sendMsg();
        }
    }

    /*
      Print final routing tables
    */

    printf("Printing the routing tables after the convergence\n");

    printRT(nd);
}

/*
  Receive Routing Message
*/

void RoutingNode::recvMsg(RouteMsg *msg) {

    /*
      Sender routing table
    */

    routingtbl *recvRoutingTable = msg->mytbl;

    /*
      Cost to sender
    */

    int link_cost = getLinkCost(msg->from);

    /*
      Traverse received routing table
    */

    for (RoutingEntry entry : recvRoutingTable->tbl) {

        /*
          Skip own interfaces
        */

        if (isMyInterface(entry.dstip))
            continue;

        /*
          Weighted DVR Formula
        */

        int new_cost;

        if (entry.cost >= INF)
            new_cost = INF;
        else
            new_cost = entry.cost + link_cost;

        /*
          Check if route already exists
        */

        bool entryExists = false;

        for (int i = 0; i < mytbl.tbl.size(); ++i) {

            RoutingEntry myEntry = mytbl.tbl[i];

            if (myEntry.dstip == entry.dstip) {

                entryExists = true;

                /*
                  CASE 1:
                  Better route found
                */

                if (new_cost < myEntry.cost) {

                    mytbl.tbl[i].cost = new_cost;

                    mytbl.tbl[i].nexthop =
                        msg->from;

                    mytbl.tbl[i].ip_interface =
                        msg->recvip;
                }

                /*
                  CASE 2:
                  Poisoned Reverse update

                  If current route already uses
                  this sender as next hop,
                  update EVEN IF WORSE
                */

                else if (myEntry.nexthop == msg->from) {

                    mytbl.tbl[i].cost = new_cost;

                    mytbl.tbl[i].ip_interface =
                        msg->recvip;
                }
            }
        }

        /*
          Route not present
        */

        if (!entryExists) {

            RoutingEntry newEntry;

            newEntry.dstip =
                entry.dstip;

            newEntry.nexthop =
                msg->from;

            newEntry.ip_interface =
                msg->recvip;

            newEntry.cost =
                new_cost;

            mytbl.tbl.push_back(newEntry);
        }
    }
}