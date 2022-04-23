#include <bits/stdc++.h>
#include <ctime>
using namespace std;

// struct of each donor
struct Donor
{
  string name;
  string organ;
  string blood_type;
  string location;
  Donor *next;
  Donor *prev;
};

// struct of each patient
struct Patient
{
  string name;
  string organ;
  string blood_type;
  string location;
  int time_left;
  Patient *next;
  Patient *prev;
};

struct vertex;

struct adjVertex
{
  vertex *v;
  int weight;
};

// struct of Vertex
struct vertex
{
  vertex *previous;
  bool visited;
  int distance;
  string name;
  vector<adjVertex> adj;
};

class Project
{

public:
  Project();
  Patient *addPatient(string name, string organ, string blood_type, string city, int time_left);
  Donor *addDonor(string name, string organ, string blood_type, string city);
  void deletePatient(string name);
  void deleteDonor(string name);
  void printDonors();
  void printPatients();
  void OperateHistory();
  void buildGraph();
  int countPatients();
  int countDonors();
  vector<pair<pair<string, string>, string>> history; // Patient, Donor, Organ
  Patient *findPatientMatch(Donor *);
  Donor *findDonorMatch(Patient *);
  int findShortestDistance(string starting_city, string ending_city);
  map<pair<string, string>, Patient *> PatientList; // map of patients
  map<pair<string, string>, Donor *> DonorList;     // map of donors
  vector<vertex> vertices;

private:
  vector<string> tempPath;
  int countP;
  int countD;
  void addVertex(string city);
  void addEdge(string starting_city, string ending_city, int weight);
  void chooseMatch();
  int Dijkstra(string starting, string destination);
};

Project::Project()
{
  countP = 0;
  countD = 0;
  PatientList.clear();
  DonorList.clear();
  history.clear();
  tempPath.clear();
}

// Adds vertex in the graph
void Project::addVertex(string city)
{
  bool found = false;
  for (int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].name == city)
    {
      found = true;
      cout << vertices[i].name << " already present.." << endl;
    }
  }

  if (found == false)
  {
    vertex v;
    v.name = city;
    vertices.push_back(v);
  }
}

// Adds edges in the graph
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

          adjVertex av2;
          av2.v = &vertices[i];
          av2.weight = weight;
          vertices[j].adj.push_back(av2);
        }
      }
    }
  }
}

// Creates the graph by adding vertex and edges
void Project::buildGraph()
{
  // Vertices
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
  // Edges
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

// General Dijkstra function to find the shortest path between 2 cities
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
  tempPath = path;
  return dest->distance;
}

// Returns the shorted distance between 2 cities
int Project::findShortestDistance(string city1, string city2)
{
  if (city1 == city2)
  {
    return 0;
  }

  int distance = Dijkstra(city1, city2);
  return distance;
}

// Returns the number of Patients
int Project::countPatients()
{
  return countP;
}

// Returns the number of Donors
int Project::countDonors()
{
  return countD;
}

// Traverses the Patient table and prints Patients
void Project::printPatients()
{
  Patient *temp = NULL;
  for (auto x : PatientList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      cout << temp->name << " : " << temp->organ << " - Blood type " << temp->blood_type << " - " << temp->location << " - " << temp->time_left << " hours left" << endl;
      temp = temp->next;
    }
  }
  delete temp;
}

// Traverses the Donor table and prints Donors
void Project::printDonors()
{
  Donor *temp;
  for (auto x : DonorList)
  {
    temp = x.second;
    while (temp != NULL)
    {
      cout << temp->name << " : " << temp->organ << " - Blood type " << temp->blood_type << " - " << temp->location << " - " << endl;
      temp = temp->next;
    }
  }
  delete temp;
}

// Searches the Patient map to find the best match for a new Donor
Patient *Project::findPatientMatch(Donor *d)
{
  vector<string> blood;
  if (d->blood_type == "A")
  {
    blood.push_back("A");
    blood.push_back("AB");
  }
  if (d->blood_type == "B")
  {
    blood.push_back("B");
    blood.push_back("AB");
  }
  if (d->blood_type == "AB")
  {
    blood.push_back("AB");
  }
  if (d->blood_type == "O")
  {
    blood.push_back("A");
    blood.push_back("B");
    blood.push_back("AB");
    blood.push_back("O");
  }

  Patient *best = NULL;
  Patient *secondBest = NULL;
  vector<string> finalPath;
  int minT = INT_MAX, minT2 = INT_MAX;
  Patient *p = NULL;
  int time_taken;

  for (int i = 0; i < blood.size(); i++)
  {
    p = PatientList[{d->organ, blood[i]}];
    while (p != NULL)
    {
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

// Searches the Donor map to find the best match for a new Patient
Donor *Project::findDonorMatch(Patient *p)
{
  vector<string> blood;
  if (p->blood_type == "A")
  {
    blood.push_back("A");
    blood.push_back("O");
  }
  if (p->blood_type == "B")
  {
    blood.push_back("B");
    blood.push_back("O");
  }
  if (p->blood_type == "AB")
  {
    blood.push_back("A");
    blood.push_back("B");
    blood.push_back("AB");
    blood.push_back("O");
  }
  if (p->blood_type == "O")
  {
    blood.push_back("O");
  }

  Donor *best = NULL;
  Donor *secondBest = NULL;
  int time_taken;
  int count;
  int minT = INT_MAX, minT2 = INT_MAX;
  vector<string> finalPath;

  for (int i = 0; i < blood.size(); i++)
  {
    count = 0;
    Donor *d = DonorList[{p->organ, blood[i]}];

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
  return best;
}

// Adds a patient to the patient map
Patient *Project::addPatient(string name, string organ, string blood_type, string city, int time_left)
{
  countP++;
  Patient *n = new Patient;
  n->name = name;
  n->time_left = time_left;
  n->organ = organ;
  n->blood_type = blood_type;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  if (PatientList.find({organ, blood_type}) == PatientList.end())
  {
    PatientList[{organ, blood_type}] = n;
    cout << "Patient " << n->name << " Added" << endl;
    return n;
  }
  Patient *x = PatientList[{organ, blood_type}];
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;
  cout << "Patient " << n->name << " Added" << endl;
  return n;
}

// Adds a donor to the donor map
Donor *Project::addDonor(string name, string organ, string blood_type, string city)
{
  countD++;
  Donor *n = new Donor;
  n->name = name;
  n->organ = organ;
  n->blood_type = blood_type;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  if (DonorList.find({organ, blood_type}) == DonorList.end())
  {
    DonorList[{organ, blood_type}] = n;
    cout << "Donor " << n->name << " Added " << endl;
    return n;
  }
  Donor *x = DonorList[{organ, blood_type}];
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;

  cout << "Donor " << n->name << " Added" << endl;
  return n;
}

// Deletes patient from patient map
void Project::deletePatient(string name)
{
  Patient *temp = NULL;
  int flag = 1;
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
          countP--;
          flag = 0;
          delete temp;
        }
        else if (temp->next == NULL)
        {
          temp->prev->next = NULL;
          countP--;
          flag = 0;
          delete temp;
        }
        else if (temp->prev == NULL)
        {
          temp->next->prev = NULL;
          PatientList[{temp->organ, temp->blood_type}] = temp->next;
          countP--;
          flag = 0;
          delete temp;
        }
        else
        {
          temp->prev->next = temp->next;
          temp->next->prev = temp->prev;
          countP--;
          flag = 0;
          delete temp;
        }
      }
      temp = temp->next;
    }
  }
  if (flag)
    cout << "Patient with name: " << name << " not found." << endl;
  else
    cout << "Deletion Successful.." << endl;
  return;
}

// Deletes donor from donor map
void Project::deleteDonor(string name)
{
  Donor *temp = NULL;
  int flag = 1;
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
          countD--;
          flag = 0;
          delete temp;
        }
        else if (temp->next == NULL)
        {
          temp->prev->next = NULL;
          countD--;
          flag = 0;
          delete temp;
        }
        else if (temp->prev == NULL)
        {
          temp->next->prev = NULL;
          DonorList[{temp->organ, temp->blood_type}] = temp->next;
          countD--;
          flag = 0;
          delete temp;
        }
        else
        {
          temp->prev->next = temp->next;
          temp->next->prev = temp->prev;
          countD--;
          flag = 0;
          delete temp;
        }
      }
      temp = temp->next;
    }
  }
  if (flag)
    cout << "Donor with name: " << name << " not found." << endl;
  else
    cout << "Deletion Successful.." << endl;
  return;
}

// Shows the previous history of matches
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

  Project myTree;
  myTree.buildGraph();
  bool running = true;

  ifstream textFile;
  textFile.open("patientList.txt");
  ifstream textFile2;
  textFile2.open("donorList.txt");
  ofstream PatientFile("patientList.txt", ios::out | ios::app);
  ofstream DonorFile("donorList.txt", ios::out | ios::app);
  ofstream MatchFile("historyList.txt", ios::out | ios::app);
  string line;
  string name;
  string organ;
  string blood_type;
  string city;
  int time_left;
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
    time_left = atoi(line.c_str());

    if (!textFile.eof())
    {
      myTree.addPatient(name, organ, blood_type, city, time_left);
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
      myTree.addDonor(name2, organ2, blood_type2, city2);
    }
  }
  while (running == true)
  {
    cout << "======Main Menu=====\n"
         << endl;
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
      cout << "Enter patient name:" << endl;
      string name;
      getline(cin, name);

      cout << "Enter organ needed:" << endl;
      string organ;
      getline(cin, organ);
      transform(organ.begin(), organ.end(), organ.begin(), ::tolower);

      string blood_type = "";
      while (flag == 0)
      {
        cout << "Enter blood type:" << endl;
        getline(cin, blood_type);
        transform(blood_type.begin(), blood_type.end(), blood_type.begin(), ::toupper);
        if (blood_type == "A" || blood_type == "B" || blood_type == "AB" || blood_type == "O")
        {
          flag = 1;
        }
        else
        {
          cout << "Please enter correct Blood Type" << endl;
        }
      }

      int time_left = 0;
      flag = 0;
      while (flag == 0)
      {
        cout << "Enter time left:" << endl;
        string str_time_left;
        getline(cin, str_time_left);
        int i = 0;
        for (i = 0; i < str_time_left.size(); i++)
        {
          if (str_time_left[i] < 48 || str_time_left[i] > 57)
            break;
        }
        if (i == str_time_left.size())
        {
          time_left = atoi(str_time_left.c_str());
          flag = 1;
        }
        else
        {
          cout << "Please enter Time in integer " << endl;
        }
      }

      string location = "";
      flag = 0;
      while (flag == 0)
      {
        cout << "Enter location:" << endl;
        getline(cin, location);
        transform(location.begin(), location.end(), location.begin(), ::tolower);
        location[0] = location[0] - 32;
        int k = 0;
        for (k = 0; k < myTree.vertices.size(); k++)
        {
          if (myTree.vertices[k].name == location)
          {
            flag = 1;
            break;
          }
        }
        if (k == myTree.vertices.size())
        {
          cout << "Wrong Location enter again\n"
               << endl;
        }
      }

      cout << endl;
      Patient *newPatient = myTree.addPatient(name, organ, blood_type, location, time_left);
      Donor *donorMatch = myTree.findDonorMatch(newPatient);

      if (donorMatch != NULL)
      {
        myTree.history.push_back({{newPatient->name, donorMatch->name}, organ});
        if (MatchFile.is_open())
        {
          cout << "\nMatch added to the match List" << endl;
          time_t now = time(0);
          char *date_time = ctime(&now);
          MatchFile << date_time << "Patient: " << newPatient->name << ", Donor: " << donorMatch->name << endl;
        }
        else
        {
          cout << "Unable to open file." << endl;
        }
        myTree.deleteDonor(donorMatch->name);
        myTree.deletePatient(newPatient->name);
      }
      else
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
      bool flag = 0;
      cout << "Enter donor name:" << endl;
      string name;
      getline(cin, name);

      cout << "Enter donated organ:" << endl;
      string organ;
      getline(cin, organ);
      transform(organ.begin(), organ.end(), organ.begin(), ::tolower);

      string blood_type = "";
      while (flag == 0)
      {
        cout << "Enter blood type:" << endl;
        getline(cin, blood_type);
        transform(blood_type.begin(), blood_type.end(), blood_type.begin(), ::toupper);
        if (blood_type == "A" || blood_type == "B" || blood_type == "AB" || blood_type == "O")
        {
          flag = 1;
        }
        else
        {
          cout << "Please enter correct Blood Type" << endl;
        }
      }

      string location = "";
      flag = 0;
      while (flag == 0)
      {
        cout << "Enter location:" << endl;
        getline(cin, location);
        transform(location.begin(), location.end(), location.begin(), ::tolower);
        location[0] = location[0] - 32;
        int k = 0;
        for (k = 0; k < myTree.vertices.size(); k++)
        {
          if (myTree.vertices[k].name == location)
          {
            flag = 1;
            break;
          }
        }
        if (k == myTree.vertices.size())
        {
          cout << "Wrong Location enter again\n"
               << endl;
        }
      }

      cout << endl;
      Donor *newDonor = myTree.addDonor(name, organ, blood_type, location);
      Patient *patientMatch = myTree.findPatientMatch(newDonor);

      if (patientMatch != NULL)
      {
        myTree.history.push_back({{patientMatch->name, newDonor->name}, organ});
        if (MatchFile.is_open())
        {
          cout << "\nMatch added to the match List" << endl;
          time_t now = time(0);
          char *date_time = ctime(&now);
          MatchFile << date_time << "Patient: " << patientMatch->name << ", Donor: " << newDonor->name << endl;
        }
        else
        {
          cout << "Unable to open file." << endl;
        }
        myTree.deletePatient(patientMatch->name);
        myTree.deleteDonor(newDonor->name);
      }
      else
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
      cout << "Enter patient name:" << endl;
      string name;
      getline(cin, name);
      myTree.deletePatient(name);
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
    }
    else if (answer == "4")
    {
      cout << "Enter donor name:" << endl;
      string name;
      getline(cin, name);
      myTree.deleteDonor(name);
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
    }
    else if (answer == "5")
    {
      int count = myTree.countPatients();
      cout << "There are " << count << " patients waiting." << endl;
    }
    else if (answer == "6")
    {
      int count = myTree.countDonors();
      cout << "There are " << count << " donors availible." << endl;
    }
    else if (answer == "7")
    {
      myTree.printPatients();
    }
    else if (answer == "8")
    {
      myTree.printDonors();
    }
    else if (answer == "9")
    {
      myTree.OperateHistory();
    }
    else if (answer == "10")
    {
      running = false;
      cout << "Adios!!\n"
           << endl;
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
