// store adjacency lists in unordered map C++ structure
//
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <fstream>
#include <array>
#include <vector> 
#include <algorithm>
#include <sstream>
#include <cstring>
#include <string>
#include <queue>
using namespace std;

/*class Node{
	public: 
		string key;
		Node *parent;
		int distance = 1;
		Node (string a, Node *p,int b){
			key = a;
			parent = p;
			distance = b;
		}
};
*/

class Item{
	public:
		string key;
		vector<string> path;
		Item (string a, vector<string> p){
			key = a;
			path = p;
		}
};

vector<string> growVector(vector<string> old, string new_key){ // pass by value
	vector<string> vect2;
	for (int i = 0; i < old.size(); i++){
		vect2.push_back(old[i]);
	}
	vect2.push_back(new_key);
	return vect2;
}


int main(int argc, char** argv)
{
  if (argc < 3) // must provide two arguments as input
  {
      throw std::invalid_argument("Usage: ./treewrapper <INPUT FILE> <OUTPUT FILE>"); // throw error
  }
	
  // initializing stuff --------------------------//
  ifstream input; // stream for input file
  ofstream output; // stream for output file
  ifstream queries;
  input.open("cleaned_movielist.txt"); // open input file
  queries.open(argv[1]);

  output.open(argv[2]); // open output file
  FILE *out_file = fopen(argv[2],"w");
  string command; 
  char *movie, *actor; 
  
  // unordered_map & parsing
  unordered_map<string, unordered_map<string,string>> catalog;
 
  // storing text file into graph: --------------------------------------
  while(getline(input,command)) // get next line of input, store as repeat
  {
    if (command.length() == 0) // command is empty
      continue;
    char *yippee = strdup(command.c_str()); // annoying, first copy string into a "C-style" string
    string com = yippee;
    movie = strtok(yippee, " \t"); //tokenize command on whitespace, first token is operation
    assert(movie != NULL);
    
    actor = strtok(NULL, " \t"); // next token is value, as string (check out documentation on strtok, to understand the pa
    vector<string> actors_list;
    while (actor != NULL) { 
      actors_list.insert(actors_list.end(),actor);
      actor = strtok(NULL, " "); //tokenize command on whitespace, first token is ope
    }
    for (string act1 : actors_list){
    	bool isPresent = true; 
	unordered_map<string, string> coactors;    

 	for (string act2 : actors_list){	
		if ((act1).compare(act2) != 0){
		    // if main actor doesn't already exist in catalog
		    if (catalog.count(act1) == 0){
			    isPresent = false;
			    coactors.insert({act2, movie});
		     } 
		    else {
			// assert(catalog[actors_list[i]] != NULL); --> problematic ... do need some way to make sure not empty tho
			catalog[act1].insert({act2,movie});
		    }
		}
	}
	if (isPresent == false){
		catalog[act1] = coactors;
	}
    }
    free(yippee);
  }
  //int added = 0;
  //int deleted = 0;
  string buf;


  // PARSING QUERIES ------------------------------------------------------------------------------
  while(getline(queries, buf)){
	  if (buf.length() == 0){
	  	continue;
	  }
    	 char *hm = strdup(buf.c_str()); // annoying, first copy string into a "C-style" string
     
    	 string actor1 = strtok(hm, " \t");
	 string actor2 = strtok(NULL, " \t");
	 free(hm);
	 // if an actor doesn't exist
  	 if(catalog.count(actor1) != 1 || catalog.count(actor2)!= 1){
	 	output << "Not present" << endl;
	 } 
	 // if they are the same actor
	 else if (actor1.compare(actor2) == 0){
	 	output << actor1 << endl;
	 } 
	 // BFS SEARCH ---------------------------------------------------------------------------------
	 
	 else{
	 	// initialize queue
		//queue<Node*> q;
		queue<Item*> q;
		 
		unordered_map<string, int> visited;
		visited.insert({actor1, 0});

		// grab all coactors of actor1
		for (auto x: catalog[actor1]){
			//Node *act = new Node(x.first, NULL, 1);
			// CREATE NEW ITEM:
			vector<string> my_path;
			my_path.push_back(actor1);
			my_path.push_back(x.first);
			Item *act = new Item(x.first, my_path);
			
			// CHECKING BALANCE OF ADDED	
			//added++;
			//cout << "adding: " << act->key << endl;
			q.push(act);
			visited.insert({x.first,0});
		} 	 

		bool isFound = false;
		while (!q.empty()){
			//Node *p = q.front();
			Item *p = q.front();
			//cout << p->key << endl;
			q.pop();
			assert(catalog.count(p->key) == 1);
			
			// if we have successfully found actor 2!
			if ((p->key).compare(actor2) == 0){
				
				// outputting answer to user
				isFound = true;
				//Node *temp = p;
				string prev = ".";
				for (string a : p->path){
					if (prev.compare(".") != 0){
						output << " -(" << catalog[prev][a] << ")- ";
					}
					output << a;
					prev = a;
				}
				output << endl;
				//cout << " freing : " << p->key << endl;
				//deleted++;
				delete(p);
				p = NULL;
				while (!q.empty()){
					Item *top = q.front();
					//cout << "freeing: " << top->key << " ";
					q.pop();
					//deleted++;
					delete(top);
					top = NULL;
				}
				//cout << endl;

				/*vector<string> formatting;
				string hyp = "-";
				string lpar = "(";
				string rpar = ")";
				string space = " ";
				
				// until we have reached the head, append answers to formatting
				while (temp->parent != NULL){
					formatting.insert(formatting.end(),rpar+hyp+space+temp->key);
					Node *save = temp;
					cout << "i am about to be freed! this is my parents: " << temp->parent->key << endl;
					temp = temp->parent;
					formatting.insert(formatting.end(), space+hyp+lpar+catalog[temp->key][save->key]);
					
					cout << "freeing: " << save->key << endl;
					freed_nodes.insert(freed_nodes.end(), save->key);
					deleted++;
					// delete(save);
					// save = NULL;
				}
				assert(temp != NULL);
				formatting.insert(formatting.end(),temp->key);
				formatting.insert(formatting.end(),catalog[actor1][temp->key]+rpar+hyp+space);
				formatting.insert(formatting.end(),actor1+space+hyp+lpar);
				reverse(formatting.begin(), formatting.end());
				for (string ele : formatting){
					output << ele;
				}
				output << endl;
				cout << "i should literally be one node: freeing: " << temp->key << endl;
				freed_nodes.insert(freed_nodes.end(), temp->key);
				deleted++;
				*/
				// delete(temp);
				// temp = NULL;
				
				// free rest of memory:
				break;
			} else{
				for (auto x: catalog[p->key]){
					if (visited.count(x.first) != 1){
						//Node *act = new Node(x.first, p, p->distance+1);
						vector<string> parent_list = p->path;
						vector<string> new_list = growVector(parent_list, x.first);
						Item *act = new Item(x.first, new_list);
						// for adding
						//added++;
						//cout << "adding: " << act->key << " with parent: " << p->key << endl;
						q.push(act);
						visited.insert({x.first, 0});
					} 
				}
				//cout << "I think I can free you now: " << p->key << endl;
				//deleted++;
				delete(p);
				p = NULL;
			}
		}
		if (!isFound){
			output << "Not present" << endl;
		}
	 }
  }
  //cout << added << endl;
  //cout << deleted << endl;
  queries.close();
  fclose(out_file);
  input.close();
  output.close();

}

