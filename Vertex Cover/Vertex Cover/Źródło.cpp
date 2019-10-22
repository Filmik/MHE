#include<iostream>
#include<math.h>
#include<stdio.h>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<tuple>
#include<string>

using namespace std;

vector<vector<int> > g;//vector for edge conections
bool v[11110];
int i, j,minimum_vertex_number=0;//usuñ zmienne globalne
vector<int> sol_vertex(int n, int e) {
	vector<int> S;
	for (i = 0; i < n; i++) {
		if (!v[i]) {
			for (j = 0; j < (int)g[i].size(); j++) {
				if (!v[g[i][j]]) {
					v[i] = true;
					v[g[i][j]] = true;
					break;
				}
			}
		}
	}
	for (i = 0; i < n; i++)
		if (v[i])
			S.push_back(i);
	return S;
}

int best_solution(int s) {
	if (minimum_vertex_number > s) {
		minimum_vertex_number = s;
	}
	return minimum_vertex_number;
}

tuple<int,int> data_from_file(int number_of_vertex, int number_of_edges,int conection_start, int conection_end)
{
	fstream input_data;
	input_data.open("input_data.txt", ios::in);
	if (input_data.good() == false) {
		cout << "Missing file input_data.txt!";
		exit(0);
	}
	input_data >> number_of_vertex;
	input_data >> number_of_edges;
	cout << "Number of vertices(from file):" << number_of_vertex << endl;
	cout << "Number of Edges(from file):" << number_of_edges << endl;

	g.resize(number_of_vertex);
	memset(v, 0, sizeof(v));
	for (i = 0; i < number_of_edges; i++) {
		cout << "Enter the end-points of edge " << i + 1 << " : ";//Enter conections 
		//cin >> a >> b;
		input_data >> conection_start >> conection_end;
		conection_start--; conection_end--;
		g[conection_start].push_back(conection_end);
		g[conection_end].push_back(conection_start);
	}

	input_data.close();
	return { number_of_vertex,number_of_edges };
}

void data_to_file(int number_of_vertex, int number_of_edges)
{
	fstream output_data;
	output_data.open("output_data.txt", ios::out | ios::app);
	output_data << "Numer of vertex: " << number_of_vertex << endl;
	output_data << "Number of edges: " << number_of_edges << endl;
	//conections to file
	
	output_data.close();
}

tuple<int, int,int,int> generate_graph()
{
	int number_of_vertex, number_of_edges, conection_start, conection_end;
	cout << "enter number of vertex: " ;
	cin >> number_of_vertex;
		if (number_of_vertex<2)
		{
			cout << "number of vertex > 2 !";
			exit(0);
		}
	number_of_edges = rand() % ((number_of_vertex * (number_of_vertex - 1)) / 2);
	cout << "The graph has " << number_of_edges << " edges."<<endl;
	
	g.resize(number_of_vertex);
	memset(v, 0, sizeof(v));
	int current_start, current_end;
	for (int x = 0; x < number_of_edges; x++) {
		current_start = rand() % number_of_vertex;
		current_end = rand() % number_of_vertex;
		if (current_end == current_start) 
		{
			current_end++;
		}
		if(find(g.begin(), g.end(), current_start) != g.end())
		{
			if (find(g.begin(), g.end(), current_end) != g.end())
			{
				// elem exists in the vector
			}
		}
		else {
			current_start--; current_end--;
			g[current_start].push_back(current_end);
			g[current_end].push_back(current_start);
		}
		/*g[x].push_back(current_edge);
		g[current_edge].push_back(x);
		cout << g[x][current_edge];
		current_edge--;*/
	}
	return { number_of_vertex, number_of_edges, conection_start, conection_end };
}

int main() {
	int number_of_vertex=0, number_of_edges=0, conection_start=0, conection_end=0, minimum_vertex_number=0;

	
	//tie(n,e)= data_from_file(number_of_vertex, number_of_edges,conection_start , conection_end);
	tie(number_of_vertex, number_of_edges, conection_start, conection_end)= generate_graph();
	cout << number_of_vertex << number_of_edges << endl;
	cout << g[1][2];
	data_to_file(number_of_vertex, number_of_edges);
	
	vector<int> S = sol_vertex(number_of_vertex, number_of_edges);//zrób rozwi¹zanie si³owe

	cout << "The required vertex cover is as follows:\n";//Exit
	for (i = 0; i < (int)S.size(); i++) {
		cout << S[i] + 1 << " ";
		minimum_vertex_number = minimum_vertex_number + 1;
	}
	cout << endl;
	cout <<"Number on needed edges: "<< minimum_vertex_number;
	minimum_vertex_number = minimum_vertex_number;
	
	best_solution(minimum_vertex_number);//FUNKCJA CELU,posprz¹taj,wczytywanie z pliku w funkcji
	
	cout << endl <<"minimum vertex=" << minimum_vertex_number;
	return 0;
}
/*Enter number of vertices:4
Enter number of Edges:5
Enter the end-points of edge 1 : 2 1
Enter the end-points of edge 2 : 3 2
Enter the end-points of edge 3 : 4 3
Enter the end-points of edge 4 : 1 4
Enter the end-points of edge 5 : 1 3
The required vertex cover is as follows:
1 2 3 4 */