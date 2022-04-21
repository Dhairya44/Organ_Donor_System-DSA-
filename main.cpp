#include <bits/stdc++.h>

using namespace std;

// struct that contains the info of each donor:
struct Donor
{
  string name;
  string organ; // see key above for number meanings
  string blood_type;
  string location;
  Donor *next;
  Donor *prev;
};

// struct that contains the info of each patient:
struct Patient
{
  string name;
  string organ;
  string blood_type;
  string location;
  int time_left; // priority
  Patient *next;
  Patient *prev;
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
  vector<vertex> path;
};

struct vertex
{
  vertex *previous;
  bool visited; // has the vertex been visited?
  int ID;       // what district does the vertex belong to?
  int numEdge;  // how many edges separate the vertex from the starting vertex?
  int distance; // how far is the vertex from the starting vertex?
  /*****************************************************************************************************************************************/
  string name;             // what is the name of the vertex?
  vector<adjVertex> adj;   // what are the adjacent vertices to the vertex?
  vector<queueVertex> que; // what are the edges, distance, and path to start vertex?
};

//-------------------------end of traditional graph variables--------------------------

class Project
{

public:
  Project();
  map<pair<string, string>, Patient *> PatientList;                                              // map of patients
  map<pair<string, string>, Donor *> DonorList;                                                  // map of donors
  Patient *addPatient(string name, string organ, string blood_type, string city, int time_left); // This adds a patient to the patientTable
  Donor *addDonor(string name, string organ, string blood_type, string city);                    // This adds a donor to the donor list
  void deletePatient(string name);                                                               // deletes patient from patient table
  void deleteDonor(string name);                                                                 // deletes donor from donor table
  int countPatients();                                                                           // traverses table and counts patients
  int countDonors();                                                                             // traverses table and counts donors
  void printDonors();                                                                            // traverses donor table and prints contents
  void printPatients();                                                                          // traverses patient table and prints contents
  void OperateHistory();                                                                         // operates and outputs results
  void buildGraph();                                                                             // builds graph with cities

  vector<pair<pair<string, string>, string>> history;
  Patient *findPatientMatch(Donor *); // searches the patient table to find the best match for a new donor
  Donor *findDonorMatch(Patient *);   // searches the donor table to find the first match for a new patient

protected:
private:
  vector<string> tempPath;
  vector<vertex> vertices;

  void addVertex(string city);                                        // adds vertexes of city graph
  void addEdge(string starting_city, string ending_city, int weight); // adds edges of city graph

  int findShortestDistance(string starting_city, string ending_city); // calls Dijkstra

  void chooseMatch();                                // chooses best patient donor match
  int Dijkstra(string starting, string destination); // used to find shortest path between two cities
};

Project::Project()
{
  PatientList.clear();
  DonorList.clear();
  history.clear();
  tempPath.clear();
}

// The addVertex function is used in the creation of our city graph.
// We are building a city graph in order to determine the shortest distance between donors
// and potential patients (each donor and patient has a location on the graph).
void Project::addVertex(string city)
{
  // make sure it's not already in the graph
  bool found = false;
  for (int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].name == city)
    {
      found = true;
      cout << vertices[i].name << "found" << endl;
    }
  }
  // add it to the vector of vertices
  if (found == false)
  {
    vertex v;
    v.name = city;
    vertices.push_back(v);
  }
}

// Here we write the addEdge function, to be used in our graph creation.
void Project::addEdge(string v1, string v2, int weight)
{

  for (int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].name == v1)
    {
      for (int j = 0; j < vertices.size(); j++)
      {
        if (vertices[j].name == v2 && i != j)
        {
          adjVertex av;
          av.v = &vertices[j];
          av.weight = weight;
          vertices[i].adj.push_back(av);

          // another vertex for edge in other direction
          adjVertex av2;
          av2.v = &vertices[i];
          av2.weight = weight;
          vertices[j].adj.push_back(av2);
        }
      }
    }
  }
}

// Our buildGraph function utilizes our addVertex and addEdge functions.
// We list all of the city names and distances (in hours taken to travel) in this function.
// We considered writing a .txt file with a table of data, but decided against it.
// Perhaps someone else can do this if they feel so inclined?
void Project::buildGraph()
{
  // Graph g;
  addVertex("Baroda");
  addVertex("Delhi");
  addVertex("Jaipur");
  addVertex("Mumbai");
  addVertex("Surat");
  addVertex("Jalandhar");
  addVertex("Kolkata");
  addVertex("Chennai");
  addVertex("Hyderabad");
  addVertex("Bangalore");
  // edge written to be undirected
  addEdge("Baroda", "Jaipur", 3);
  addEdge("Baroda", "Surat", 1);
  addEdge("Baroda", "Mumbai", 3);
  addEdge("Surat", "Mumbai", 2);
  addEdge("Jaipur", "Delhi", 3);
  addEdge("Jaipur", "Jalandhar", 2);
  addEdge("Delhi", "Jalandhar", 2);
  addEdge("Delhi", "Kolkata", 4);
  addEdge("Delhi", "Bangalore", 5);
  addEdge("Hyderabad", "Delhi", 6);
  addEdge("Mumbai", "Hyderabad", 5);
  addEdge("Chennai", "Mumbai", 4);
  addEdge("Chennai", "Hyderabad", 3);
  addEdge("Hyderabad", "Kolkata", 5);
  addEdge("Bangalore", "Hyderabad", 2);
  addEdge("Jaipur", "Bangalore", 4);
  addEdge("Surat", "Chennai", 5);
  addEdge("Jalandhar", "Chennai", 7);
  addEdge("Mumbai", "Kolkata", 8);
}

// Here is our Dijkstra function. No surprising capabilities- it is used to find shortest
// path between two cities within our graph.
int Project::Dijkstra(string starting, string destination)
{

  vector<vertex *> solved;
  vertex *start, *dest;
  for (int i = 0; i < vertices.size(); i++)
  {
    vertices[i].visited = false;
    vertices[i].previous = NULL;
    vertices[i].distance = INT_MAX;
    if (starting == vertices[i].name)
    {
      start = &vertices[i];
    }
    if (destination == vertices[i].name)
    {
      dest = &vertices[i];
    }
  }
  start->visited = true;
  start->distance = 0;
  solved.push_back(start);

  while (!dest->visited)
  {
    int minDistance = INT_MAX;
    vertex *minVertex, *prevVertex;
    for (int i = 0; i < solved.size(); i++)
    {
      vertex *u = solved[i];
      for (int j = 0; j < u->adj.size(); j++)
      {
        if (!u->adj[j].v->visited)
        {
          int distance = u->distance + u->adj[j].weight;
          if (minDistance > distance)
          {
            minDistance = distance;
            minVertex = u->adj[j].v;
            prevVertex = u;
          }
        }
      }
    }
    solved.push_back(minVertex);
    minVertex->distance = minDistance;
    minVertex->previous = prevVertex;
    minVertex->visited = true;
  }
  vector<string> path;
  vertex *temp = dest;
  while (temp != NULL)
  {
    path.push_back(temp->name);
    temp = temp->previous;
  }
  // for (auto x : path)
  // {
  //   cout << x << "->";
  // }
  // cout << endl;
  tempPath = path;
  return dest->distance;
}

// This function simply calles Dijkstra if the two entered cities have different names. If the cities are the same, it returns
// a distance of zero (meaning it takes zero hours to travel between the two)
int Project::findShortestDistance(string city1, string city2)
{
  // check the two cities
  if (city1 == city2)
  {
    return 0;
  }
  // use Dijkstra traverse to find and print the shortest distance
  int distance = Dijkstra(city1, city2);
  return distance;
}

// This function traverses the table of patients and returns an overall count
int Project::countPatients()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  int count = 0;
  // iterate through organs
  Patient *temp = NULL;
  for (auto x : PatientList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      count++;
      temp = temp->next;
    }
  }
  return count;
}

// This function does the same thing as countPatients, but with the donor table
int Project::countDonors()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  int count = 0;
  Donor *temp = NULL;
  // iterate through organs
  for (auto x : DonorList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      count++;
      temp = temp->next;
    }
  }

  return count;
}

// This function traverses the patient table and prints the data entered for every patient.
// For ease of use, we are storing the data such as "organ needed" and blood-type as integers,
// so here we have to include 'if' statements in order to give outputs that are meaningful to the user.
void Project::printPatients()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  Patient *temp = NULL;

  // iterate through organs
  for (auto x : PatientList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      cout << temp->name << " : " << temp->organ << " - Blood type " << temp->blood_type << " - " << temp->location << " - " << temp->time_left << " hours left" << endl;
      temp = temp->next;
    }
  }
}

// This function does the same thing as printPatients, but for the donor table.
void Project::printDonors()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  Donor *temp;

  // iterate through organs
  for (auto x : DonorList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      cout << temp->name << " : " << temp->organ << " - Blood type " << temp->blood_type << " - " << temp->location << " - " << endl;
      temp = temp->next;
    }
  }
}

// findPatientMatch is a function we use to search through the patient table and see if there are any
// suitable matches for a donor's organ. We call this whenever a new donor is added to the donor table (using the function addDonor).
// If a suitable match is found, this function returns a pointer to the best candidate to recieve the organ.
Patient *Project::findPatientMatch(Donor *d)
{
  string js[5];
  for (int k = 0; k < 5; k++)
  {
    js[k] = "";
  }

  if (d->blood_type == "A")
  {
    js[0] = "A";
    js[1] = "AB";
  }
  if (d->blood_type == "B")
  {
    js[0] = "B";
    js[1] = "AB";
  }
  if (d->blood_type == "AB")
  {
    js[0] = "AB";
  }
  if (d->blood_type == "O")
  {
    js[0] = "A";
    js[1] = "B";
    js[2] = "AB";
    js[3] = "O";
  }

  int best_score = INT_MIN;
  Patient *best = NULL;
  Patient *secondBest = NULL;
  int minT = INT_MAX, minT2 = INT_MAX;
  Patient *p = NULL;
  int count = 0;
  int score;
  int time_taken;
  vector<string> finalPath;
  int m = 0;

  while (js[m] != "")
  {
    count = 0;
    p = PatientList[{d->organ, js[m]}];
    while (p != NULL)
    {
      // count++; // relative time on waiting list
      time_taken = findShortestDistance(p->location, d->location);
      if (time_taken <= minT2)
      {
        secondBest = p;
        minT2 = time_taken;
      }
      if (p->time_left >= time_taken && p->time_left <= minT)
      {
        best = p;
        minT = p->time_left;
        finalPath = tempPath;
      }
      p = p->next;
    }
    m++;
  }
  if (best == NULL)
  {
    if (secondBest == NULL)
    {
      cout << "\nNo compatible Patient Found!!" << endl;
    }
    else
    {
      cout << "\nNo Patient which can be reached on time..\n";
      cout << "\nThe nearest Patient " << secondBest->name << " was found " << minT2 << " hours far in " << secondBest->location << endl;
    }
  }
  else
  {
    cout << "\nMatch found with " << best->name << " at " << best->location << endl;

    cout << "\nThe route is: ";
    for (int i = 0; i < finalPath.size() - 1; i++)
    {
      cout << finalPath[i] << "->";
    }
    cout << finalPath[finalPath.size() - 1] << endl;
  }
  return best;
}

// This function does the same thing as findPatientMatch, however it searches the donor table for a match
// whenever a new patient is added (using the addPatient function). It returns a pointer to the matching donor.
Donor *Project::findDonorMatch(Patient *p)
{
  string js[5];
  for (int k = 0; k < 5; k++)
  {
    js[k] = "";
  }

  if (p->blood_type == "A")
  {
    js[0] = "A";
    js[1] = "O";
  }
  if (p->blood_type == "B")
  {
    js[0] = "B";
    js[1] = "AB";
  }
  if (p->blood_type == "AB")
  {
    js[0] = "A";
    js[1] = "B";
    js[2] = "AB";
    js[3] = "O";
  }
  if (p->blood_type == "O")
  {
    js[0] = "O";
  }

  int best_score = INT_MIN;
  Donor *best = NULL;
  Donor *secondBest = NULL;
  int score;
  int time_taken;
  int count;
  int minT = INT_MAX, minT2 = INT_MAX;
  vector<string> finalPath;
  int m = 0;

  while (js[m] != "")
  {
    count = 0;
    Donor *d = DonorList[{p->organ, js[m]}];

    while (d != NULL)
    {
      time_taken = findShortestDistance(p->location, d->location);
      if (time_taken <= minT2)
      {
        secondBest = d;
        minT2 = time_taken;
      }
      if (p->time_left >= time_taken && p->time_left <= minT)
      {
        best = d;
        minT = p->time_left;
        finalPath = tempPath;
      }
      d = d->next;
    }
    m++;
  }
  if (best != NULL)
  {
    cout << "\nMatch found with " << best->name << " at " << best->location << endl;

    cout << "\nThe route is: ";
    for (int i = 0; i < finalPath.size() - 1; i++)
    {
      cout << finalPath[i] << "->";
    }
    cout << finalPath[finalPath.size() - 1] << endl;
  }
  else
  {
    if (secondBest == NULL)
    {
      cout << "\nNo compatible Donor Found!!" << endl;
    }
    else
    {
      cout << "\nNo Donor which can be reached on time..\n";
      cout << "The nearest Donor " << secondBest->name << " was found " << minT2 << " hours far in " << secondBest->location << endl;
    }
  }
  return NULL;
}

// The addPatient function takes the input data for a new patient and adds them to the patient table.
Patient *Project::addPatient(string name, string organ, string blood_type, string city, int time_left)
{
  bool cityfound = false;
  transform(organ.begin(), organ.end(), organ.begin(), ::tolower);
  transform(blood_type.begin(), blood_type.end(), blood_type.begin(), ::toupper);
  transform(city.begin(), city.end(), city.begin(), ::tolower);
  city[0] = city[0] - 32;

  for (int k = 0; k < vertices.size(); k++)
  {
    if (vertices[k].name == city)
    {
      cityfound = true;
    }
  }

  if (blood_type != "A" && blood_type != "B" && blood_type != "AB" && blood_type != "O")
  {
    cout << "Invalid Blood Group!!" << endl;
    return NULL;
  }

  if (cityfound == false)
  {
    cout << "City not Found!!" << endl;
    return NULL;
  }

  Patient *n = new Patient;
  n->name = name;
  n->time_left = time_left;
  n->organ = organ;
  n->blood_type = blood_type;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  Patient *x = PatientList[{organ, blood_type}];
  if (x == NULL)
  {
    PatientList[{organ, blood_type}] = n;
    cout << "Patient " << n->name << " Added" << endl;
    return n;
  }
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;
  cout << "Patient " << n->name << " Added" << endl;
  return n;
}

// The addDonor function does the same thing as the addPatient function, but with donors and the donor table.
Donor *Project::addDonor(string name, string organ, string blood_type, string city)
{
  bool cityfound = false;
  transform(organ.begin(), organ.end(), organ.begin(), ::tolower);
  transform(blood_type.begin(), blood_type.end(), blood_type.begin(), ::toupper);
  transform(city.begin(), city.end(), city.begin(), ::tolower);
  city[0] = city[0] - 32;
  for (int k = 0; k < vertices.size(); k++)
  {
    if (vertices[k].name == city)
    {
      cityfound = true;
    }
  }

  if (blood_type != "A" && blood_type != "B" && blood_type != "AB" && blood_type != "O")
  {
    cout << "Invalid Blood Group!!" << endl;
    return NULL;
  }

  if (cityfound == false)
  {
    cout << "City not Found!!" << endl;
    return NULL;
  }

  Donor *n = new Donor;
  n->name = name;
  n->organ = organ;
  n->blood_type = blood_type;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  Donor *x = DonorList[{organ, blood_type}];
  if (x == NULL)
  {
    DonorList[{organ, blood_type}] = n;
    cout << "Donor " << n->name << " Added " << endl;
    return n;
  }
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;

  cout << "Donor " << n->name << " Added" << endl;
  return n;
}

// The deletePatient function removes a patient from the patient table. We call this function when the findPatientMatch
// or findDonorMatch functions match a patient with a donor. You can also manually delete patients from the table, as seen in
// our main menu.
void Project::deletePatient(string name)
{
  Patient *temp = NULL;

  for (auto x : PatientList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      if (temp->name == name)
      {
        if (temp->next == NULL && temp->prev == NULL)
        {
          PatientList[{temp->organ, temp->blood_type}] = NULL;
          delete temp;
          return;
        }
        else if (temp->next == NULL)
        {
          temp->prev->next = NULL;
          delete temp;
          return;
        }
        else if (temp->prev == NULL)
        {
          temp->next->prev = NULL;
          PatientList[{temp->organ, temp->blood_type}] = temp->next;
          delete temp;
          return;
        }
        else
        {
          temp->prev->next = temp->next;
          temp->next->prev = temp->prev;
          delete temp;
          return;
        }
      }
      temp = temp->next;
    }
  }
  cout << "Patient with name: " << name << " not found." << endl;
  return;
}

// Once again, deleteDonor has the same functionality as deletePatient, except it pertains to the donor table
void Project::deleteDonor(string name)
{
  Donor *temp = NULL;

  for (auto x : DonorList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      if (temp->name == name)
      {
        if (temp->next == NULL && temp->prev == NULL)
        {
          DonorList[{temp->organ, temp->blood_type}] = NULL;
          delete temp;
          return;
        }
        else if (temp->next == NULL)
        {
          temp->prev->next = NULL;
          delete temp;
          return;
        }
        else if (temp->prev == NULL)
        {
          temp->next->prev = NULL;
          DonorList[{temp->organ, temp->blood_type}] = temp->next;
          delete temp;
          return;
        }
        else
        {
          temp->prev->next = temp->next;
          temp->next->prev = temp->prev;
          delete temp;
          return;
        }
      }
      temp = temp->next;
    }
  }
  cout << "Donor with name: " << name << " not found." << endl;
  return;
}

// This function is relatively unique. When called, the operate function "performs" a surgery. It takes the first Pair
// in the queue, reads the included "survivability" value (which is simply a success rate for the surgery), and then
// chooses whether the operation was a success or a failure, using the success rate as a probability. If the surgery is
// a success, it adds one to the global successCount counter. Otherwise, it adds one to the failurCount. Once the surgery is done,
// the Pair is dequeued, and the next Pair is at the top of the queue, awaiting surgery.
void Project::OperateHistory()
{
  cout << "History of Previously Matched Donors and Patients: " << endl;
  for (auto x : history)
  {
    cout << "Patient: " << x.first.first << " Donor: " << x.first.second << " Organ: " << x.second << endl;
  }
}

int main()
{

  // Here we initialize our data structures
  Project myTree;
  myTree.buildGraph(); // builds our city graph
  bool running = true; // initialize bool to keep program running. Will = false when 'Quit' is selected

  // The following block of code reads in the patient list (.txt file) and adds them all to the patient table
  ifstream textFile;
  textFile.open("patientList.txt");
  ifstream textFile2;
  textFile2.open("donorList.txt");
  ofstream PatientFile("patientList.txt", ios::out | ios::app);
  ofstream DonorFile("donorList.txt", ios::out | ios::app);
  string line;
  string name;
  string organ;
  string blood_type;
  string city;
  int time;
  while (!textFile.eof())
  {
    getline(textFile, line, ',');
    name = line;

    getline(textFile, line, ',');
    organ = line;

    getline(textFile, line, ',');
    blood_type = line;

    getline(textFile, line, ',');
    city = line;

    getline(textFile, line);
    time = atoi(line.c_str());

    if (!textFile.eof())
    {
      myTree.addPatient(name, organ, blood_type, city, time); // adds each patient from the txt file to the patient table, replacing the placeholders
    }
  }
  string line2;
  string name2;
  string organ2;
  string blood_type2;
  string city2;
  while (!textFile2.eof())
  {
    getline(textFile2, line2, ',');
    name2 = line2;

    getline(textFile2, line2, ',');
    organ2 = line2;

    getline(textFile2, line2, ',');
    blood_type2 = line2;

    getline(textFile2, line2);
    city2 = line2;

    if (!textFile2.eof())
    {
      myTree.addDonor(name2, organ2, blood_type2, city2); // adds each patient from the txt file to the patient table, replacing the placeholders
    }
  }
  while (running == true)
  {
    // build main menu
    cout << "======Main Menu=====" << endl;
    cout << "1. Add a patient" << endl;
    cout << "2. Add a donor" << endl;
    cout << "3. Delete a patient" << endl;
    cout << "4. Delete a donor" << endl;
    cout << "5. Count patients" << endl;
    cout << "6. Count donors" << endl;
    cout << "7. Print patients" << endl;
    cout << "8. Print donors" << endl;
    cout << "9. Operate History" << endl;
    cout << "10. Quit\n"
         << endl;

    // takes user input
    string answer;
    getline(cin, answer);

    cout << endl;
    if (answer == "1")
    {
      bool flag = 0;
      cout << "Enter patient name:" << endl; // ex: Johnson (We entered last names, but any name can identify a patient)
      string name;
      getline(cin, name);

      cout << "Enter organ needed:" << endl; // ex: heart
      string organ;
      getline(cin, organ);

      cout << "Enter blood type:" << endl; // ex: AB
      string blood_type;
      getline(cin, blood_type);

      int time_left = 0;
      while (flag == 0)
      {
        cout << "Enter time left:" << endl; // ex: 3 (this is measured in hours)
        string str_time_left;
        getline(cin, str_time_left);
        int i = 0;
        for (i = 0; i < str_time_left.size(); i++)
        {
          if (str_time_left[i] < 49 || str_time_left[i] > 57)
            break;
        }
        if (i == str_time_left.size())
        {
          time_left = atoi(str_time_left.c_str());
          flag = 1;
        }
      }

      cout << "Enter location:" << endl; // ex: Bangalore
      string location;
      getline(cin, location);
      cout << endl;
      // here we add the input information into the patient table, and then check if there is a donor
      // already available to be matched with the patient. If there is, a Pair is created and the patient
      // is removed from the table.
      Patient *newPatient = myTree.addPatient(name, organ, blood_type, location, time_left); // adds patient to the patient table
      Donor *donorMatch = NULL;
      if (newPatient != NULL)
      {
        Donor *donorMatch = myTree.findDonorMatch(newPatient); // searches donor table for a match for the patient, assigns it to donorMatch if found
      }
      else
      {
        cout << "\nInvalid Patient enter again" << endl;
      }

      if (donorMatch != NULL)
      {
        myTree.history.push_back({{newPatient->name, donorMatch->name}, organ}); // adds the pair to the surgery waiting list
        myTree.deleteDonor(donorMatch->name);                                    // deletes donor from table because the organ has been allocated
        myTree.deletePatient(newPatient->name);                                  // deletes patient from table because they no longer need an organ
      }
      else if (newPatient != NULL)
      {
        if (PatientFile.is_open())
        {
          cout << "\nPatient Added to List" << endl;
          PatientFile << name << "," << organ << "," << blood_type << "," << location << "," << time_left << endl;
        }
        else
        {
          cout << "Unable to open file." << endl;
        }
      }
    }
    else if (answer == "2")
    {
      cout << "Enter donor name:" << endl; // ex: Smith
      string name;
      getline(cin, name);

      cout << "Enter donated organ:" << endl; // ex: lungs
      string organ;
      getline(cin, organ);

      cout << "Enter blood type:" << endl; // ex: O
      string blood_type;
      getline(cin, blood_type);

      cout << "Enter location:" << endl; // ex: Ranchi
      string location;
      getline(cin, location);
      cout << endl;
      // here we add the input information into the donor table and check for a suitable match in the patient table.
      // If a candidate is found, a Pair is made and the donor is removed from the table.
      Donor *newDonor = myTree.addDonor(name, organ, blood_type, location); // Adds the donor to the donor table
      Patient *patientMatch = NULL;                                         // initializes the matched patient struct
      if (newDonor != NULL)
      {
        patientMatch = myTree.findPatientMatch(newDonor); // searches patient tree for a match to the donor, assigns it to patientMatch if found
      }
      else
      {
        cout << "\nInvalid Donor enter again" << endl;
      }

      if (patientMatch != NULL)
      {
        myTree.history.push_back({{patientMatch->name, newDonor->name}, organ}); // places the donor-patient pair into a waiting list for surgery
        myTree.deletePatient(patientMatch->name);                                // removes the patient from the patient table (since they no longer need a donor)
        myTree.deleteDonor(newDonor->name);                                      // removes the donor from the donor table (since the organ has been allocated)
      }
      else if (newDonor != NULL)
      {
        if (DonorFile.is_open())
        {
          cout << "\nDonor Added to List" << endl;
          DonorFile << name << "," << organ << "," << blood_type << "," << location << endl;
        }
        else
        {
          cout << "Unable to open file." << endl;
        }
      }
    }
    else if (answer == "3")
    {
      cout << "Enter patient name:" << endl; // ex: Jamison
      string name;
      getline(cin, name);
      myTree.deletePatient(name); // deletes patient from patient table
    }
    else if (answer == "4")
    {
      cout << "Enter donor name:" << endl; // ex: Chen
      string name;
      getline(cin, name);
      myTree.deleteDonor(name); // deletes donor from donor table
    }
    else if (answer == "5")
    {
      int count = myTree.countPatients();                            // counts the number of patients in the table and assigns it to a variable
      cout << "There are " << count << " patients waiting." << endl; // outputs the number of patients in the patient table
    }
    else if (answer == "6")
    {
      int count = myTree.countDonors();                              // counts the number of donors in the table and assigns it to a variable
      cout << "There are " << count << " donors availible." << endl; // outputs the number of donors in the donor table
    }
    else if (answer == "7")
    {
      myTree.printPatients(); // prints the list of patients and all their attributes
    }
    else if (answer == "8")
    {
      myTree.printDonors(); // prints the list of donors and all their attributes
    }
    else if (answer == "9")
    {
      myTree.OperateHistory(); // Performs a surgery on the first Pair in the queue
    }
    else if (answer == "10")
    {
      ofstream PatientFileClose("patientList.txt", ios::out | ios::trunc);
      Patient *Ptemp = NULL;
      for (auto x : myTree.PatientList)
      {
        Ptemp = x.second;
        while (Ptemp != NULL)
        {
          PatientFileClose << Ptemp->name << "," << Ptemp->organ << "," << Ptemp->blood_type << "," << Ptemp->location << "," << Ptemp->time_left << endl;
          Ptemp = Ptemp->next;
        }
      }
      PatientFileClose.close();
      ofstream DonorFileClose("donorList.txt", ios::out | ios::trunc);
      Donor *Dtemp = NULL;
      for (auto x : myTree.DonorList)
      {
        Dtemp = x.second;
        while (Dtemp != NULL)
        {
          DonorFileClose << Dtemp->name << "," << Dtemp->organ << "," << Dtemp->blood_type << "," << Dtemp->location << endl;
          Dtemp = Dtemp->next;
        }
      }
      DonorFileClose.close();
      running = false; // ends while loop, thus ending program

      cout << "Goodbye!" << endl;
      return 0;
    }
    else
    {
      cout << "Invalid Input.. Please enter correct Input.." << endl;
    }

    cout << "\n\nEnter any key to continue ..." << endl;
    string wait;
    getline(cin, name);
    system("CLS");
  }
}