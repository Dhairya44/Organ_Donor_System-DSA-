#include <iostream>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include "Patient_Donor_Database.h"

// DISCLAIMER: our public class is named PatientTree. This is a misnomer, as our database does not
// actually include any trees. Our organization of patients and donors in this project is much more
// of a 3-dimensional table, as described in the README.
// Apologies for the confusion.

using namespace std;

// Here we initialize our patient and donor tables by setting every space to NULL.
// We also initialize a few global variables, namely the counts for operation successes and operation failures
// as well as the head and tail of the operation queue.
PatientTree::PatientTree()
{
  // constructor
  successCount = 0;
  failureCount = 0;
  queueHead = 0;
  queueTail = 0;
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      PatientList[i][j] = NULL;
      DonorList[i][j] = NULL;
    }
  }
}

// The destructor currently does not call any functions.
PatientTree::~PatientTree()
{
}

// The addVertex function is used in the creation of our city graph.
// We are building a city graph in order to determine the shortest distance between donors
// and potential patients (each donor and patient has a location on the graph).
void PatientTree::addVertex(std::string n)
{
  // make sure it's not already in the graph
  bool found = false;
  for (int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].name == n)
    {
      found = true;
      cout << vertices[i].name << "found" << endl;
    }
  }
  // add it to the vector of vertices
  if (found == false)
  {
    vertex v;
    v.name = n;
    vertices.push_back(v);
  }
}

// Here we write the addEdge function, to be used in our graph creation.
void PatientTree::addEdge(std::string v1, std::string v2, int weight)
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
void PatientTree::buildGraph()
{
  // Graph g;
  addVertex("Boston");
  addVertex("New York City");
  addVertex("Philadelphia");
  addVertex("Atlanta");
  addVertex("Denver");
  addVertex("San Francisco");
  addVertex("Dallas");
  addVertex("Phoenix");
  addVertex("Seattle");
  addVertex("Honolulu");
  addVertex("Chicago");
  addVertex("Detroit");
  addVertex("Los Angeles");
  addVertex("Houston");
  addVertex("Miami");
  addVertex("Memphis");
  addVertex("Cleveland");
  addVertex("Boise");
  // edge written to be undirected
  addEdge("Seattle", "San Francisco", 2);
  addEdge("Seattle", "Phoenix", 3);
  addEdge("Seattle", "New York City", 6);
  addEdge("San Francisco", "New York City", 6);
  addEdge("San Francisco", "Phoenix", 2);
  addEdge("San Francisco", "Boston", 6);
  addEdge("San Francisco", "Denver", 3);
  addEdge("Boise", "Seattle", 1);
  addEdge("Boise", "Denver", 2);
  addEdge("Los Angeles", "San Francisco", 1);
  addEdge("Los Angeles", "Denver", 2);
  addEdge("Los Angeles", "Dallas", 3);
  addEdge("Los Angeles", "Miami", 5);
  addEdge("Los Angeles", "New York", 6);
  addEdge("Los Angeles", "Boston", 6);
  addEdge("Denver", "Phoenix", 2);
  addEdge("Denver", "Dallas", 2);
  addEdge("Denver", "New York", 4);
  addEdge("Honolulu", "San Francisco", 5);
  addEdge("Dallas", "Memphis", 1);
  addEdge("Dallas", "Houston", 1);
  addEdge("Dallas", "Atlanta", 2);
  addEdge("Houston", "Miami", 3);
  addEdge("Houston", "Philadelphia", 3);
  addEdge("Houston", "Chicago", 3);
  addEdge("Memphis", "Atlanta", 1);
  addEdge("Memphis", "Cleveland", 4);
  addEdge("Cleveland", "Detroit", 1);
  addEdge("Cleveland", "Chicago", 1);
  addEdge("Detroit", "Chicago", 1);
  addEdge("Detroit", "New York City", 2);
  addEdge("Detroit", "Philadelphia", 2);
  addEdge("Detroit", "Denver", 3);
  addEdge("Miami", "Los Angeles", 5);
  addEdge("Miami", "Atlanta", 2);
  addEdge("Atlanta", "Philadelphia", 2);
  addEdge("Philadelpha", "Boston", 1);
  addEdge("Boston", "New York City", 1);
}

// Here is our Dijkstra function. No surprising capabilities- it is used to find shortest
// path between two cities within our graph.
int PatientTree::Dijkstra(string starting, string destination)
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
  return dest->distance;
}

// This function simply calles Dijkstra if the two entered cities have different names. If the cities are the same, it returns
// a distance of zero (meaning it takes zero hours to travel between the two)
int PatientTree::findShortestDistance(string city1, string city2)
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
int PatientTree::countPatients()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  Patient *x;
  int count = 0;

  // iterate through organs
  for (int i = 0; i < 7; i++)
  {
    // iterate through blood types
    for (int j = 0; j < 4; j++)
    {
      if (PatientList[i][j] != NULL)
      {
        count++;
        x = PatientList[i][j]; // head of linked list
        // iterate through patients
        while (x->next != NULL)
        {
          count++;
          x = x->next;
        }
      }
    }
  }
  return count;
}

// This function does the same thing as countPatients, but with the donor table
int PatientTree::countDonors()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  Donor *x;
  int count = 0;

  // iterate through organs
  for (int i = 0; i < 7; i++)
  {
    // iterate through blood types
    for (int j = 0; j < 4; j++)
    {
      if (DonorList[i][j] != NULL)
      {
        count++;
        x = DonorList[i][j]; // head of linked list
        // iterate through patients
        while (x->next != NULL)
        {
          count++;
          x = x->next;
        }
      }
    }
  }
  return count;
}

// This function traverses the patient table and prints the data entered for every patient.
// For ease of use, we are storing the data such as "organ needed" and blood-type as integers,
// so here we have to include 'if' statements in order to give outputs that are meaningful to the user.
void PatientTree::printPatients()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  string organ;
  string blood_type;
  Patient *x;

  // iterate through organs
  for (int i = 0; i < 7; i++)
  {
    // iterate through blood types
    for (int j = 0; j < 4; j++)
    {
      if (PatientList[i][j] != NULL)
      {
        x = PatientList[i][j]; // head of linked list
        // iterate through patients
        while (x != NULL)
        {
          if (x->organ == 1)
          {
            organ = "Heart";
          }
          else if (x->organ == 2)
          {
            organ = "Lungs";
          }
          else if (x->organ == 3)
          {
            organ = "Liver";
          }
          else if (x->organ == 4)
          {
            organ = "Pancreas";
          }
          else if (x->organ == 5)
          {
            organ = "Kidney";
          }
          else if (x->organ == 6)
          {
            organ = "Intestines";
          }
          else if (x->organ == 7)
          {
            organ = "Head";
          }

          if (x->blood_type == 1)
          {
            blood_type = "A";
          }
          else if (x->blood_type == 2)
          {
            blood_type = "B";
          }
          else if (x->blood_type == 3)
          {
            blood_type = "AB";
          }
          else if (x->blood_type == 4)
          {
            blood_type = "O";
          }
          cout << x->name << " : " << organ << " - Blood type " << blood_type << " - " << x->location << " - " << x->survivability << " chance of survival - " << x->time_left << " hours left" << endl;
          x = x->next;
        }
      }
    }
  }
}

// This function does the same thing as printPatients, but for the donor table.
void PatientTree::printDonors()
{
  // organs: heart, kidney, liver, lungs, pancreas, intestines, head
  // blood tyes: O, A, B, AB
  Donor *x;
  string organ;
  string blood_type;

  // iterate through organs
  for (int i = 0; i < 7; i++)
  {
    // iterate through blood types
    for (int j = 0; j < 4; j++)
    {
      if (DonorList[i][j] != NULL)
      {
        x = DonorList[i][j]; // head of linked list
        // iterate through patients
        while (x != NULL)
        {
          if (x->organ == 1)
          {
            organ = "Heart";
          }
          else if (x->organ == 2)
          {
            organ = "Lungs";
          }
          else if (x->organ == 3)
          {
            organ = "Liver";
          }
          else if (x->organ == 4)
          {
            organ = "Pancreas";
          }
          else if (x->organ == 5)
          {
            organ = "Kidney";
          }
          else if (x->organ == 6)
          {
            organ = "Intestines";
          }
          else if (x->organ == 7)
          {
            organ = "Head";
          }

          if (x->blood_type == 1)
          {
            blood_type = "A";
          }
          else if (x->blood_type == 2)
          {
            blood_type = "B";
          }
          else if (x->blood_type == 3)
          {
            blood_type = "AB";
          }
          else if (x->blood_type == 4)
          {
            blood_type = "O";
          }
          cout << x->name << " : " << organ << " - Blood type " << blood_type << " - " << x->location << endl;
          x = x->next;
        }
      }
    }
  }
}

// findPatientMatch is a function we use to search through the patient table and see if there are any
// suitable matches for a donor's organ. We call this whenever a new donor is added to the donor table (using the function addDonor).
// If a suitable match is found, this function returns a pointer to the best candidate to recieve the organ.
Patient *PatientTree::findPatientMatch(Donor *d)
{
  int js[5];
  for (int k = 0; k < 5; k++)
  {
    js[k] = 0;
  }

  if (d->blood_type == 1)
  {
    js[0] = 1;
    js[1] = 3;
  }
  if (d->blood_type == 2)
  {
    js[0] = 2;
    js[1] = 3;
  }
  if (d->blood_type == 3)
  {
    js[0] = 3;
  }
  if (d->blood_type == 4)
  {
    js[0] = 1;
    js[1] = 2;
    js[2] = 3;
    js[3] = 4;
  }

  int best_score = INT_MIN;
  Patient *best = NULL;
  Patient *p = NULL;
  int count = 0;
  int score;
  int time_taken;

  int m = 0;
  while (js[m] != 0)
  {
    count = 0;
    p = PatientList[d->organ - 1][js[m] - 1];
    while (p != NULL)
    {
      count++; // relative time on waiting list
      time_taken = findShortestDistance(p->location, d->location);
      if (p->time_left >= time_taken)
      {
        score = (p->survivability) - count;
        if (score > best_score)
        {
          best_score = score;
          best = p;
        }
      }
      p = p->next;
    }
    m++;
  }
  if (best == NULL)
  {
    cout << "No suitable patient was found" << endl;
  }
  else
  {
    cout << "Match found!" << endl;
  }
  return best;
}

// This function does the same thing as findPatientMatch, however it searches the donor table for a match
// whenever a new patient is added (using the addPatient function). It returns a pointer to the matching donor.
Donor *PatientTree::findDonorMatch(Patient *p)
{
  int js[5];
  for (int k = 0; k < 4; k++)
  {
    js[k] = 0;
  }

  if (p->blood_type == 1)
  {
    js[0] = 1;
    js[1] = 4;
  }
  if (p->blood_type == 2)
  {
    js[0] = 2;
    js[1] = 4;
  }
  if (p->blood_type == 3)
  {
    js[0] = 1;
    js[1] = 2;
    js[2] = 3;
    js[3] = 4;
  }
  if (p->blood_type == 4)
  {
    js[0] = 4;
  }

  int best_score = INT_MIN;
  Donor *best = NULL;
  int score;
  int time_taken;
  int count;

  int m = 0;
  while (js[m] != 0)
  {
    count = 0;
    Donor *d = DonorList[p->organ - 1][js[m] - 1];
    while (d != NULL)
    {
      time_taken = findShortestDistance(p->location, d->location);
      if (p->time_left >= time_taken)
      {
        return d;
        cout << "A suitable donor was found!" << endl;
      }
      d = d->next;
    }
    m++;
  }
  cout << "No suitable donor was found." << endl;
  return NULL;
}

// The addPatient function takes the input data for a new patient and adds them to the patient table.
Patient *PatientTree::addPatient(string name, string organ, string blood_type, string city, int time_left, int survivability)
{
  bool cityfound = false;
  int i = 1 * ((organ == "heart" || organ == "Heart")) + 2 * ((organ == "lungs" || organ == "Lungs")) + 3 * ((organ == "liver" || organ == "Liver")) + 4 * ((organ == "pancreas" || organ == "Pancreas")) + 5 * ((organ == "kidney" || organ == "Kidney")) + 6 * ((organ == "intestines" || organ == "Intestines")) + 7 * ((organ == "head" || organ == "Head"));
  if (i == 0)
  {
    cout << "Organ not found" << endl;
    // new
    return NULL;
  }
  int j = 1 * (blood_type == "A" || blood_type == "a") + 2 * (blood_type == "B" || blood_type == "b") + 3 * (blood_type == "AB" || blood_type == "ab") + 4 * (blood_type == "O" || blood_type == "o");
  if (j == 0)
  {
    cout << "Blood type not found" << endl;
    // new
    return NULL;
  }

  for (int k = 0; k < vertices.size(); k++)
  {
    if (vertices[k].name == city)
    {
      cityfound = true;
    }
  }

  if (cityfound == false)
  {
    cout << "City not found" << endl;
    return NULL;
  }

  Patient *n = new Patient;
  n->name = name;
  n->survivability = survivability;
  n->time_left = time_left;
  n->organ = i;
  n->blood_type = j;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  Patient *x = PatientList[i - 1][j - 1];
  if (x == NULL)
  {
    PatientList[i - 1][j - 1] = n;
    return n;
  }
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;

  return n;
}

// The addDonor function does the same thing as the addPatient function, but with donors and the donor table.
Donor *PatientTree::addDonor(string name, string organ, string blood_type, string city)
{
  bool cityfound = false;
  int i = 1 * ((organ == "heart" || organ == "Heart")) + 2 * ((organ == "lungs" || organ == "Lungs")) + 3 * ((organ == "liver" || organ == "Liver")) + 4 * ((organ == "pancreas" || organ == "Pancreas")) + 5 * ((organ == "kidney" || organ == "Kidney")) + 6 * ((organ == "intestines" || organ == "Intestines")) + 7 * ((organ == "head" || organ == "Head"));
  if (i == 0)
  {
    cout << "Organ not found" << endl;
    return NULL;
  }
  int j = 1 * (blood_type == "A" || blood_type == "a") + 2 * (blood_type == "B" || blood_type == "b") + 3 * (blood_type == "AB" || blood_type == "ab") + 4 * (blood_type == "O" || blood_type == "o");
  if (j == 0)
  {
    cout << "Blood type not found" << endl;
    return NULL;
  }

  for (int k = 0; k < vertices.size(); k++)
  {
    if (vertices[k].name == city)
    {
      cityfound = true;
    }
  }

  if (cityfound == false)
  {
    cout << "City not found" << endl;
    return NULL;
  }

  Donor *n = new Donor;
  n->name = name;
  n->organ = i;
  n->blood_type = j;
  n->location = city;
  n->next = NULL;
  n->prev = NULL;
  Donor *x = DonorList[i - 1][j - 1];
  if (x == NULL)
  {
    DonorList[i - 1][j - 1] = n;
    cout << "Added " << n->name << " to spot " << i - 1 << "," << j - 1 << endl;
    return n;
  }
  while (x->next != NULL)
  {
    x = x->next;
  }
  n->prev = x;
  x->next = n;

  cout << n->name << endl;
  return n;
}

// The deletePatient function removes a patient from the patient table. We call this function when the findPatientMatch
// or findDonorMatch functions match a patient with a donor. You can also manually delete patients from the table, as seen in
// our main menu.
void PatientTree::deletePatient(string name)
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Patient *x = PatientList[i][j];
      while (x != NULL)
      {
        if (x->name == name)
        {
          if (x->next == NULL and x->prev == NULL)
          {
            DonorList[i][j] = NULL;
            delete x;
            return;
          }
          if (x->next == NULL)
          {
            x->prev->next = NULL;
            delete x;
            return;
          }
          if (x->prev == NULL)
          {
            x->next->prev = NULL;
            PatientList[i][j] = x->next;
            delete x;
            return;
          }
          else
          {
            x->prev->next = x->next;
            x->next->prev = x->prev;
            delete x;
            return;
          }
        }
        x = x->next;
      }
    }
  }
  cout << "patient not found." << endl;
}

// Once again, deleteDonor has the same functionality as deletePatient, except it pertains to the donor table
void PatientTree::deleteDonor(string name)
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      Donor *x = DonorList[i][j];
      while (x != NULL)
      {
        if (x->name == name)
        {
          if (x->next == NULL and x->prev == NULL)
          {
            DonorList[i][j] = NULL;
            delete x;
            return;
          }
          if (x->next == NULL)
          {
            x->prev->next = NULL;
            delete x;
            return;
          }
          if (x->prev == NULL)
          {
            x->next->prev = NULL;
            DonorList[i][j] = x->next;
            delete x;
            return;
          }
          else
          {
            x->prev->next = x->next;
            x->next->prev = x->prev;
            delete x;
            return;
          }
        }
        x = x->next;
      }
    }
  }
  cout << "patient not found." << endl;
}

// The buildPatientList function sets every space in the organ-blood type table to 0;
void PatientTree::buildPatientList()
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      PatientList[i][j] = 0;
    }
  }
}

// The buildDonorList does the same thing as the buildPatientList, but for the donor table.
void PatientTree::buildDonorList()
{
  for (int i = 0; i < 7; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      DonorList[i][j] = 0;
    }
  }
}

// When a patient is matched with a donor (or vice versa), a Pair struct is created. This struct is then placed into
// an operation queue using this function: enqueue. The Pair then awaits surgery. This is a first-in, first-out queue.
void PatientTree::enqueue(Pair match)
{ // we can do this bc we've declared push in header
  arrayQueue[queueTail] = match;
  queueTail++;
  queueTail = queueTail % 10;
}

// This function dequeues the first Pair in the queue, thus removing them from it and moving every other Pair up a space.
Pair PatientTree::dequeue()
{
  Pair match = arrayQueue[queueHead];
  queueHead++;
  queueHead = queueHead % 10;
  return match;
}

// This bool function checks to see if the queue is full.
bool PatientTree::queueIsFull()
{
  if ((queueTail + 1) % 10 == queueHead)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// This bool function checks to see if the queue is empty.
bool PatientTree::queueIsEmpty()
{
  if (queueHead == queueTail)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// This function prints the queue- it lists off each matched Pair of donor and patient that have not yet undergone surgery.
void PatientTree::printMatches()
{
  /*cout << "Head: " << queueHead << endl;
  cout << "Tail: " << queueTail << endl;*/
  for (int i = queueHead; i < queueTail; i++)
  {
    cout << "Patient: " << arrayQueue[i].patient << " - Donor: " << arrayQueue[i].donor << " - Chance of sucessfull operation: " << arrayQueue[i].success_rate << endl;
  }
}

// This function is relatively unique. When called, the operate function "performs" a surgery. It takes the first Pair
// in the queue, reads the included "survivability" value (which is simply a success rate for the surgery), and then
// chooses whether the operation was a success or a failure, using the success rate as a probability. If the surgery is
// a success, it adds one to the global successCount counter. Otherwise, it adds one to the failurCount. Once the surgery is done,
// the Pair is dequeued, and the next Pair is at the top of the queue, awaiting surgery.
void PatientTree::Operate()
{
  Pair operatedPair;
  int successChance = arrayQueue[queueHead].success_rate;
  bool success = (rand() % 100) < successChance;
  if (success == true and !queueIsEmpty())
  {
    cout << arrayQueue[queueHead].patient << "'s surgery was a success." << endl;
    operatedPair = dequeue();
    successCount++;
  }
  else if (!queueIsEmpty())
  {
    cout << arrayQueue[queueHead].patient << "'s surgery was a failure." << endl;
    operatedPair = dequeue();
    failureCount++;
  }
  else
  {
    cout << "No one to operate on" << endl;
  }
}
