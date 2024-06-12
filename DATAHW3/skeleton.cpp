#include <iostream>
#include <vector>
#include <fstream>
#include <map>

#include<queue>
#include <algorithm>
#include <unordered_map>
#include<string>
using namespace std;

struct Node {
    string MID;
    vector<Node*> adj;
    vector<string> relation;
};


map<string, Node*> graph_map = {};
map<string, string> tsvNames = {};

//The first part
void neighbors(const string& search) 
{
    Node* node = graph_map[search];

    if (graph_map.find(search)== graph_map.end()) 
    {
        return; //means there is no node
    }

   //The adj vector gives me the neighbours of that node, I can reach how many elements the vector has directly
    cout<< node->adj.size()<< " neighbours"<< endl;

    for (Node* traverse : node->adj) 
    {
        cout << traverse->MID <<" " << tsvNames[traverse->MID] << endl;
    }

}



//The Second Part
void degreeOfNode() 
{
    //I will store the nodes with centralities in the centrality vector
    vector<pair<string, int> > centrality;

    for ( auto& element : graph_map) 
    {
        string namee = element.first;
        int degree = element.second->adj.size();

        centrality.push_back(make_pair(namee, degree) );
    }

    // Sortin the centrality vector in descending order in order to find the top 10 highest centrality nodes
    sort(centrality.begin(), centrality.end(), [](const pair<string, int>& a, const pair<string, int>& b) {return b.second < a.second;} );

    // Printing first 10 nodes
    for (int i = 0; i < 10 && i < centrality.size(); ++i){
        string namee = centrality[i].first;
        int degree = centrality[i].second;

        cout << namee <<"\t" <<tsvNames[namee]<< ":  " << degree << " degree centrality" << endl;
    }
}


//The Third Part
// Finding shortest distance and path
void ShortestDistancePath(const string& node1, const string& node2) 
{
    if (graph_map.find(node1) == graph_map.end() || graph_map.find(node2) == graph_map.end()) 
    {
        cout << "There is no particular node/nodes in the graph " << endl;
        return;
    }

    queue<string> Queue;
    Queue.push(node1); //The first mid was pushed to the queue

    unordered_map<string, bool> visited;
    unordered_map<string, string> parent;

    visited[node1] = true;//and marked as true
    parent[node1] = "";

    while (!Queue.empty()) 
    {
        string current = Queue.front();

        Queue.pop();

        if (current == node2) 
        {
            // The node that I want to reach was found, since this is BFS I found the shortest path when I find the MID2
            vector<string> path;

            for (string at = node2; !at.empty(); at = parent[at]) 
            {
                path.push_back(at);
            }

            reverse(path.begin(), path.end() );

            cout << "Shortest distance: "<< path.size() - 1 << endl;
            cout << "Path: "<< endl;

            for (string& name : path) 
            {
                cout << name <<" ";
                cout<< tsvNames[name]<< endl;
            }
            cout << endl;
            return;
        }

        for (Node* neighbor: graph_map[current]->adj) 
        {
            if (!visited[neighbor->MID]) 
            {
                Queue.push(neighbor->MID);
                visited[neighbor->MID] = true;
                parent[neighbor->MID] = current;
            }
        }
    }

    cout << "No path is available" << endl;
}



int main(int argc, char* argv[])
{

    ifstream infile("freebase.tsv");

    string line;
    while (getline(infile, line))
    {
        string ent1 = line.substr(0, line.find("\t"));
        string remain = line.substr(line.find("\t")+1,line.length()-ent1.length());
        string relationship = remain.substr(0, remain.find("\t"));
        remain = remain.substr(remain.find("\t")+1, remain.length()-relationship.length());
        string ent2 = remain.substr(0, remain.find("\r"));

        Node* ent1_node, *ent2_node;

        if (graph_map.find(ent1) == graph_map.end()) {
            ent1_node = new Node;
            ent1_node->MID = ent1;
            graph_map[ent1] = ent1_node;
        } else {
            ent1_node = graph_map[ent1];
        }

        if (graph_map.find(ent2) == graph_map.end()) {
            ent2_node = new Node;
            ent2_node->MID = ent2;
            graph_map[ent2] = ent2_node;
        } else {
            ent2_node = graph_map[ent2];
        }

        ent1_node->adj.push_back(ent2_node);
        ent1_node->relation.push_back(relationship);

        ent2_node->adj.push_back(ent1_node);
        ent2_node->relation.push_back(relationship);
    }    

    ifstream infile2("mid2name.tsv");

    while (getline(infile2, line))
    {
        string MID = line.substr(0, line.find("\t"));
        string remain = line.substr(line.find("\t")+1,line.length()-MID.length());
        string name = remain.substr(0, remain.find("\r"));

        if(tsvNames.find(MID)== tsvNames.end())
        {
            tsvNames[MID]= name;
        }
    
        //cout << name<< endl;
    }

  
   // neighbors("/m/04mx8h4");

   //degreeOfNode();

   //ShortestDistancePath("/m/0xn6", "/m/0y09");
if (argc < 2) {
        cerr << "Insufficient arguments provided." << endl;
        return 1;
    }

    string command = argv[1];

    if (command == "part1") 
    {
        if (argc != 3) {
            cerr << "Write as: ./main part1 MID" << endl;
            return 1;
        }
        string MID = argv[2];
        neighbors(MID);
    } 
    else if(command == "part2") 
    {
        degreeOfNode();
    } 
    else if (command == "part3") 
    {
        if (argc != 4) {
            cerr << "Write as: ./main part3 MID1 MID2" << endl;
            return 1;
        }
        string MID1 = argv[2];
        string MID2 = argv[3];
        ShortestDistancePath(MID1, MID2);
    } else {
        cerr << "Unknown command: " << command << endl;
        return 1;
    }


   
    return 0;


}
