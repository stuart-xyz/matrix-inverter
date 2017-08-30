// Matrix Inverter
// Inverts a user-specified square, non-singular matrix using Gaussian elimination and backsubstitution
// Outputs the inverted matrix to 3 decimal places
// WARNING: this program implements partial, not complete, pivoting, so rounding errors may occur for some matrices

////////////////////////////////////* HOW TO FORMAT THE INPUT FILE *///////////////////////////////////////////////
//
// [1] The file should be named 'matrix.txt' and placed in the directory that this program is being run from
// [2] There should be no blank lines at the beginning or end of the file
// [3] Each row should start at the beginning of a new line
// [4] There should be no spaces at the beginning or end of each row
// [5] The matrix elements on each row should be separated by a comma
// [6] There should be no commas at the beginning or end of each row
//
//		EXAMPLE:
//		0,1,0,2
//		1,0,1,0
//		0,1,0,1
//		1,0,2,0
//
// NOTE: you can choose to create an example file in the program directory when you run the program, already
//       named 'matrix.txt'.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Header files
#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
#include<cstdlib>
#include<string>
#include<iomanip>
#include<limits>

// Using the standard namespace to shorten code
using namespace std;

// Main function
int main(){
	// Declare user input characters
	char usrinput2, usrinput1;
	// Ask the user if they want the example matrix to be made for them
	cout << "Would you like to create 'matrix.txt' containing an example\nmatrix? Y/N\n\n";
	cin >> usrinput1;
	// Check if their answer is yes or no, and keep asking if it is not
	while (tolower(usrinput1) != 'y' && tolower(usrinput1) != 'n'){
		// Clear the cin buffer so that the while loop does not keep repeating
		cin.clear();
		cin.ignore(numeric_limits <streamsize> ::max(), '\n');
		cout << "\nWould you like to create 'matrix.txt' containing an example\nmatrix? Y/N\n\n";
		cin >> usrinput1;
	};
	// Clear the cin buffer so that extra characters won't affect the next question
	cin.clear();
	cin.ignore(numeric_limits <streamsize> ::max(), '\n');
	// Make the file if they say yes
	if (tolower(usrinput1) == 'y'){
		ofstream example("matrix.txt");
		if (example.is_open()){
			example << 0 << "," << 1 << "," << 0 << "," << 2 << "\n";
			example << 1 << "," << 0 << "," << 1 << "," << 0 << "\n";
			example << 0 << "," << 1 << "," << 0 << "," << 1 << "\n";
			example << 1 << "," << 0 << "," << 2 << "," << 0;
			// Success message
			cout << "\n'matrix.txt' has been successfully created in the program directory. This\ncontains an example matrix.\n";
		}
		else{
			// Error message if the file cannot write for some reason
			cout << "\nUnfortunately, 'matrix.txt' could not be created in the program directory. Please follow the instructions and create 'matrix.txt' manually.\n\n";
		};
		// Close the example file
		example.close();
	};
	// Ask the user whether the file is ready
	cout << "\n'matrix.txt' should be in the following format:\ne.g.	0,1,0,2\n	1,0,1,0\n	0,1,0,1\n	1,0,2,0\ni.e. elements are delimited by a comma, and each row is on a new line.\nThe matrix must be square and non-singular (invertible).\n\nType 'Y' when 'matrix.txt' is ready\n\n";
	cin >> usrinput2;
	// Keep asking if they say anything other than yes
	while (tolower(usrinput2) != 'y'){
		// Clear the cin buffer
		cin.clear();
		cin.ignore(numeric_limits <streamsize> ::max(), '\n');
		cout << "\nType 'Y' when 'matrix.txt' is ready\n\n";
		cin >> usrinput2;
	};
	// Clear the cin buffer again just in case
	cin.clear();
	cin.ignore(numeric_limits <streamsize> ::max(), '\n');
	// Declare variables
	double xt;
	string line;
	int columns;
	// 2D vector declaration
	vector < vector<double> > matrix;
	vector < vector<double> > identity;
	// 1D line vectors
	vector <double> lineData;
	vector <double> zeros;
	// Open the input file
	ifstream inputfile("matrix.txt");
	// Throw a warning if it cannot be found
	if (!inputfile.is_open()){
		cout << "\n'matrix.txt' cannot be found in the program directory. Please move it to the\nprogram directory and restart the program.\n\n";
		return 0;
	};
	// Intial setting of parsing success flag
	bool success_flag = false;
	// Loop through each line of the file and capture it as a string
	while (!inputfile.eof()){
		getline(inputfile, line);
		// Find the positions of each comma delimiter
		while (line.find(",") != string::npos){
			// Capture the substring up to the position of each successive comma delimiter
			string dataString = line.substr(0, line.find(","));
			// Convert it to a double
			xt = double(atof(dataString.c_str()));
			// Add this value to the current line data vector
			lineData.push_back(xt);
			// Erase the string up to this point to prepare the line string for the next operation
			line.erase(0, (line.find(',')) + 1);
			// Add a zero to the identity vector
			zeros.push_back(0);
			success_flag = true;
		};
		// If there was actually anything on this line (success flag is true), add on the last values
		if (success_flag){
			// Capture the remaining bit of the line that gets left off
			xt = double(atof(line.c_str()));
			lineData.push_back(xt);
			// Add in another 0 too
			zeros.push_back(0);
			// Add the whole line to the 2D vectors
			matrix.push_back(lineData);
			identity.push_back(zeros);
		};
		columns = lineData.size();
		// Clear the line data ready for the next loop and reset the success flag
		lineData.clear();
		success_flag = false;
	};
	// Close the input file
	inputfile.close();
	// Declare the variables that are necessary for row operations
	int N = matrix.size(), i, j, k, l, m, temp;
	int a, b;
	// If the matrix is not square, throw an error
	if (columns != N){
		cout << "\nYour matrix is not square (NxN)!\n\n";
		return 0;
	};
	// Loop through each element of the 'identity' matrix and set its diagonal elements to 1, then augment each line of the user's matrix with the (now actual) identity matrix (for ease of operations)
	for (a = 0; a < N; a++){
		for (b = 0; b < N; b++){
			if (a == b){
				identity[a][b] = 1;
			};
			matrix[a].push_back(identity[a][b]);
		};
	};
	double r;

	//////////////////////////////*GAUSSIAN ELIMINATION AND BACKSUBSTITUTION*//////////////////////////////////////////
	for (j = 0; j < N; j++){
		// Stores the current value of j in a temporary variable
		temp = j;
		// For the jth column of the matrix, loops through all the rows below the diagonal element
		for (i = j + 1; i < N; i++){
			// If an element in one of these rows is greater than the diagonal element, store the row number in the temporary variable. Repeat for each row, comparing to the last found largest value
			if (matrix[i][j]>matrix[temp][j]){
				// The temporary variable will eventually contain the row index of the largest element in that column
				temp = i;
			};
		};
		// If the value of temp has changed i.e. there was a larger element than the diagonal, swap the row containing the largest vertical element in the column with the jth row
		if (temp != j){
			matrix[temp].swap(matrix[j]);
		};
		for (k = 0; k < N; k++){
			// If k is not equal to j i.e. we are NOT looking at a diagonal element, loop through each vertical element of the jth column, and set the multiplying constant r to be the current element
			if (k != j){
				r = matrix[k][j];
				// Loop through each horizontal element of the row currently being analysed
				for (l = 0; l < N * 2; l++){
					// Divide the corresponding horizontal element of the jth row by the diagonal element of the jth row, then multiply by r
					// Subtract this whole quantity from the current horizontal element of the kth row
					matrix[k][l] -= (matrix[j][l] / matrix[j][j])*r;
				};
			}
			// If k is equal to j i.e. we ARE looking at a diagonal element
			else{
				r = matrix[k][j];
				for (m = 0; m < N * 2; m++){
					// Make sure r is non zero
					if (r != 0){
						// Just divide the current row by the last diagonal element
						matrix[k][m] /= r;
					}
					else{
						// If we are attempting to divide by 0, the matrix must be singular
						cout << "\nYour matrix is singular!\n\n";
						return 0;
					};
				};
			};
		};
	};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create the output file, ready for writing
	ofstream outputfile("inverted_matrix.txt");
	// Set the number precision
	outputfile.precision(3);
	outputfile.setf(ios::fixed);
	outputfile.setf(ios::showpoint);
	int tr = 0, tc = 0;
	// Finds the element with the largest number of characters (largest absolute value)
	for (int f = 0; f < N; f++){
		for (int g = N; g < N * 2; g++){
			if (abs(matrix[f][g])>matrix[tr][tc]){
				tr = f;
				tc = g;
			};
		};
	};
	int count = 0;
	double big = matrix[tr][tc];
	// Finds out how many characters the aforementioned element has before the decimal point by division by 10
	while (big>1){
		big /= 10;
		count++;
	};
	// Output the inverted matrix (which is now the latter half of the columns of the augmented matrix i.e. transformed identity matrix)
	for (int f = 0; f < N; f++){
		for (int g = N; g < N * 2; g++){
			// Check if the current element is 0. If it is, write the absolute value to get rid of the minus signs that appear on zeros
			if (matrix[f][g] == 0){
				if (g == (N * 2) - 1){
					// If we're at the end of the row, don't put a tab at the end
					// Set the width of each element to be equal to the total length (in characters) of the biggest element + 6
					outputfile << abs(matrix[f][g]) << setw(count + 4 + 6);
				}
				// If we're at the start of the row and the element is positive, shift it forward a space to be in line with the negative numbers
				else if (g == N){
					outputfile << " " << abs(matrix[f][g]) << "\t" << setw(count + 4 + 6);
				}
				else{
					// If we're not at the end of the row, put a tab at the end
					outputfile << abs(matrix[f][g]) << "\t" << setw(count + 4 + 6);
				};
			}
			else{
				if (g == (N * 2) - 1){
					outputfile << matrix[f][g] << setw(count + 4 + 6);
				}
				else if (g == N && matrix[f][g] > 0){
					outputfile << " " << matrix[f][g] << "\t" << setw(count + 4 + 6);
				}
				else{
					outputfile << matrix[f][g] << "\t" << setw(count + 4 + 6);
				};
			};
		};
		// New line for each row
		outputfile << "\n";
	};
	// Close the output file
	outputfile.close();
	cout << "\n'inverted_matrix.txt' can now be found in the program directory.\n";
	return 0;
};
