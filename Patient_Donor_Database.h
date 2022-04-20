#ifndef PATIENTLIST_H
#define PATIENTLIST_H

// NOTICE: The following is the key we use, to make storage and manipulation of data easier:
// heart = 1
// lungs = 2
// liver = 3
// pancreas = 4
// kindey = 5
// intestines = 6
// head = 7

// A = 1
// B = 2
// AB = 3
// O = 4

// struct that contains the info of each donor:
struct Donor
{
    std::string name;
    int organ; // see key above for number meanings
    int blood_type;
    std::string location;
    Donor *next;
    Donor *prev;
};

// struct that contains the info of each patient:
struct Patient
{
    std::string name;
    int organ; // see key above for number meanings
    int blood_type;
    int survivability;
    std::string location;
    int time_left;
    Patient *next;
    Patient *prev;
};

// The 'Pair' struct is created when a patient and donor are matched
struct Pair
{
    std::string patient;
    std::string donor;
    int success_rate;
};

//--------------------start of traditional graph-building variables--------------------
struct vertex;

struct adjVertex
{
    vertex *v;
    int weight;
};

struct queueVertex
{
    int edges;
    int distance;
    std::vector<vertex> path;
};

struct vertex
{
    vertex *previous;
    bool visited;                 // has the vertex been visited?
    int ID;                       // what district does the vertex belong to?
    int numEdge;                  // how many edges separate the vertex from the starting vertex?
    int distance;                 // how far  is the vertec from the starting vertex?
    std::string path[100];        // what is the path between the vertex and the starting vertex?
    std::string name;             // what is the name of the vertex?
    std::vector<adjVertex> adj;   // what are the adjacent vertices to the vertex?
    std::vector<queueVertex> que; // what are the edges, distance, and path to start vertex?
};
//-------------------------end of traditional graph variables--------------------------

class PatientTree
{

public:
    PatientTree();
    virtual ~PatientTree();
    Patient *PatientList[7][4]; // This is our intial 2-D array. There are 7 organ types and 4 blood types.
    Donor *DonorList[7][4];
    Patient *addPatient(std::string name, std::string organ, std::string blood_type, std::string city, int time_left, int survivability); // This adds a patient to the patientTable
    Donor *addDonor(std::string name, std::string organ, std::string blood_type, std::string city);                                       // This adds a donor to the donor list
    void deletePatient(std::string name);                                                                                                 // deletes patient from patient table
    void deleteDonor(std::string name);                                                                                                   // deletes donor from donor table
    int countPatients();                                                                                                                  // traverses table and counts patients
    int countDonors();                                                                                                                    // traverses table and counts donors
    void printDonors();                                                                                                                   // traverses donor table and prints contents
    void printPatients();                                                                                                                 // traverses patient table and prints contents
    void printMatches();                                                                                                                  // prints all of the matches with patients and donors that have been made
    void Operate();                                                                                                                       // operates and outputs results
    void buildGraph();                                                                                                                    // builds graph with cities
    void buildPatientList();                                                                                                              // builds empty patient list
    void buildDonorList();                                                                                                                // builds empty donor list
    void enqueue(Pair);                                                                                                                   // adds a Pair (matched donor and patient) to the operation queue
    Pair dequeue();                                                                                                                       // removes a Pair from the queue (first-in, first-out).
    bool queueIsFull();                                                                                                                   // checks if queue is full
    bool queueIsEmpty();                                                                                                                  // checks if queue is empty

    int queueSize;
    int queueHead;
    int queueTail;
    int successCount;
    int failureCount;
    Pair arrayQueue[10];
    Patient *findPatientMatch(Donor *); // searches the patient table to find the best match for a new donor
    Donor *findDonorMatch(Patient *);   // searches the donor table to find the first match for a new patient

protected:
private:
    std::vector<vertex> vertices;

    void addVertex(std::string city);                                             // adds vertexes of city graph
    void addEdge(std::string starting_city, std::string ending_city, int weight); // adds edges of city graph

    int findShortestDistance(std::string starting_city, std::string ending_city); // calls Dijkstra

    void chooseMatch();                                          // chooses best patient donor match
    int Dijkstra(std::string starting, std::string destination); // used to find shortest path between two cities
};
#endif // MOVIETREE_H
