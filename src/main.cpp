#include "node.h"

vector<RoutingNode*> distanceVectorNodes;

void routingAlgo(vector<RoutingNode*> distanceVectorNodes);

int main() {

    int n;

    cin >> n;

    string name;

    distanceVectorNodes.clear();

    /*
      Create nodes
    */

    for (int i = 0; i < n; i++) {

        RoutingNode *newnode = new RoutingNode();

        cin >> name;

        newnode->setName(name);

        distanceVectorNodes.push_back(newnode);
    }

    cin >> name;

    /*
      Input format:

      NodeName MyIP NeighborIP NeighborName Cost

      Example:

      A 10.0.0.1 10.0.0.21 B 4
    */

    while (name != "EOE") {

        for (int i = 0; i < distanceVectorNodes.size(); i++) {

            string myeth;
            string oeth;
            string oname;

            int cost;

            if (distanceVectorNodes[i]->getName() == name) {

                /*
                  Read topology entry
                */

                cin >> myeth;

                cin >> oeth;

                cin >> oname;

                cin >> cost;

                /*
                  Find connected node
                */

                for (int j = 0;
                     j < distanceVectorNodes.size();
                     j++) {

                    if (distanceVectorNodes[j]->getName()
                        == oname) {

                        /*
                          Add interface with cost
                        */

                        distanceVectorNodes[i]->addInterface(
                            myeth,
                            oeth,
                            cost,
                            distanceVectorNodes[j]
                        );

                        /*
                          Initialize routing table
                          cost = 0 to self
                        */

                        distanceVectorNodes[i]->addTblEntry(
                            myeth,
                            0
                        );

                        break;
                    }
                }
            }
        }

        cin >> name;
    }

    /*
      Run Distance Vector Routing
    */

    routingAlgo(distanceVectorNodes);

    return 0;
}