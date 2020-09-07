#include <iostream>
#include <sstream>
using namespace std;

int getPorts(char file_name[], string container_ids[], int ports[]) {
	char line[256];
	int size = 0;

	FILE* file = fopen(file_name, "r");
	
	while (fgets(line, sizeof(line), file)) {
		//cout<<line<<endl;
		string id;
		string port_str = "";
		string word = "";
		int i = 0;
		while(1) {
			//cout << "reading line " << i << ":::"<< line[i] << endl;
			if(line[i] == ' ') {
				id = word;
				word = "";
				i++;
			}
			else if(line[i] == '\n') {
				port_str = word;
				i++;
				break;
			}
			else {
				word += line[i];
				i++;
			}
		}
		container_ids[size] = id;
  	    stringstream ss(port_str); 
	  
	    // The object has the value 12345 and stream 
	    // it to the integer x 
	    int port = 0; 
	    ss >> port; 
  
    // Now the variable x holds the value 12345 
  
		ports[size++] = port;
		//cout<<"container_id is "<< id << " and port is " << port << endl;
		size++;
	}
	return size;
}

int main(int argc, char *argv[])
{
	char filename[256];
	string container_ids[20];
	int ports[20];
	int size;
	if (argc < 2) {
		cout << "Usage: ./read_data_file_to_arrays <filename>\n";
		exit(0);
	}
	//cout<<"checking file "<< argv[1];
	size = getPorts(argv[1], container_ids, ports);

	// Print container IDS and ports
	for(int i = 0; i<size; i++) {
		cout<<"container_id is "<< container_ids[i] << " and port is " << ports[i] << endl;
	}
	return 0;
}