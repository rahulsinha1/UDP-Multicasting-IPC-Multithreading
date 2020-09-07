#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
std::ifstream infile("data_id_port");


int getPorts(char file_name[], string container_ids[], int ports[]) {
	int size = 0;

	FILE* file = fopen(file_name, "r");

	std::string line;
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);
	    if (!(iss >> container_ids[size] >> ports[size])) { break; } // error
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
	 cout<<"checking file "<< argv[1];
	 size = getPorts(argv[1], container_ids, ports);

	// Print container IDS and ports
	 for(int i = 0; i<size; i++) {
	 	cout<<"container_id is "<< container_ids[i] << " and port is " << ports[i] << endl;
	 }
	return 0;
}