#include<iostream>
#include<cstdlib>
#include<sstream>
#include<string>
#include<vector>
#include<omp.h>
#include<ctime>

using namespace std;

class Vertex{
	static int graphSize;
	static Vertex **idIndex; //pointer array to all vertices

	int id;
	vector<Vertex*> neighbors;
	//for DFS
	bool searched;
	omp_lock_t vertexLock;
public:
	static void init(int); //initialize the static variables
	static int getNumOfVertices();
	static Vertex* getVertexWithId(int);

	Vertex(int);
	int getId();
	void addNeighbor(Vertex*);
	int getDegree();
	Vertex* getNeighbor(int);
	//for DFS
	void setSearched(bool);
	bool isSearched();
	void lock();
	void unlock();
};

int Vertex::graphSize;
Vertex **Vertex::idIndex;

void Vertex::init(int graphSize){
	Vertex::graphSize = graphSize;
	idIndex = new Vertex*[graphSize];

	#pragma omp parallel for
	for(int i = 0; i < graphSize; i++){
		idIndex[i] = new Vertex(i);
	}
}

int Vertex::getNumOfVertices(){
	return graphSize;
}

Vertex* Vertex::getVertexWithId(int id){
	return idIndex[id];
}

Vertex::Vertex(int id){
	this->id = id;
	searched = false;
	omp_init_lock(&vertexLock);
}

int Vertex::getId(){
	return id;
}

void Vertex::addNeighbor(Vertex *neighbor){
	neighbors.push_back(neighbor);
}

int Vertex::getDegree(){
	return (int)neighbors.size();
}

Vertex* Vertex::getNeighbor(int index){
	return neighbors.at(index);
}

void Vertex::setSearched(bool value){
	searched = value;
}

bool Vertex::isSearched(){
	return searched;
}

void Vertex::lock(){
	omp_set_lock(&vertexLock);
}

void Vertex::unlock(){
	omp_unset_lock(&vertexLock);
}

//main program start from here

void init();
vector<string> split(string, char);

int main(int argc, char *argv[]){
	init(); //read the input graph and build up the structure

	int componentCount = 0;
	//DFS without function recursion
	vector<Vertex*>* searchBuffer = new vector<Vertex*>();
	for(int i = 0; i < Vertex::getNumOfVertices(); i++){
		Vertex *startVertex = Vertex::getVertexWithId(i);
		if(startVertex->isSearched() == false){
			componentCount++;
			startVertex->setSearched(true);
			searchBuffer->push_back(startVertex);
		}
		while(searchBuffer->empty() == false){
			vector<Vertex*>* newSearchBuffer = new vector<Vertex*>();

			//#pragma omp parallel for
			for(int j = 0; j < (int)searchBuffer->size(); j++){
				Vertex *vertex = searchBuffer->at(j);
				for(int k = 0; k < vertex->getDegree(); k++){
					Vertex *neighbor = vertex->getNeighbor(k);
					//neighbor->lock();
					if(neighbor->isSearched() == false){
						neighbor->setSearched(true);
						//#pragma omp critical
						newSearchBuffer->push_back(neighbor);
					}
					//neighbor->unlock();
				}
			}

			delete searchBuffer;
			searchBuffer = newSearchBuffer;
		}
	}
	cout << componentCount << endl;

	return 0;
}

void init(){
	cin.sync_with_stdio(false);
	string line;
	getline(cin, line);
	int size = atoi(line.c_str());

	//time_t pre = time(NULL);

	Vertex::init(size);

	//cout << "init " << (pre = time(NULL) - pre) << endl;

	vector<string> stringCache;


	while(getline(cin, line)){
		stringCache.push_back(line);
	}

	//cout << "read lines " << (pre = time(NULL) - pre) << endl;

	#pragma omp parallel for
	for(int i = 0; i < (int)stringCache.size(); i++){
		size_t found;
		string cacheLine = stringCache.at(i);
		if((found = cacheLine.find("->")) != string::npos){
			Vertex *current = Vertex::getVertexWithId(atoi(cacheLine.substr(0, found).c_str()));
			cacheLine = cacheLine.substr(found + 2);
			vector<string> neighbors = split(cacheLine, ',');
			for(int i = 0; i < (int)neighbors.size(); i++){
				int id = atoi(neighbors.at(i).c_str());
				current->addNeighbor(Vertex::getVertexWithId(id));
			}
		}
	}

	//cout << "link " << (pre = time(NULL) - pre) << endl;
}

vector<string> split(string str, char delim){
	vector<string> items;
	size_t found;
	while((found = str.find(delim)) != string::npos){
		items.push_back(str.substr(0, found));
		str = str.substr(found + 1);
	}
	items.push_back(str);
	return items;
}


back return to list
