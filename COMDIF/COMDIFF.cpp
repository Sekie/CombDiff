#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// #define ERROR 0.005 /********** CHANGE ERROR FOR COMBINATION DIFFERENCES HERE (1 wavenumber = 29970 MHz) **********/

/**************************************************************************************************
   THIS PROGRAM IS INTENDED TO CHECK ALL POSSIBLE COMBINATION DIFFERENCES COMPARED AGAINST THE
   REFERENCE ENERGIES. THE FOLLOWING NEEDS TO BE DONE IN ORDER TO RUN THE PROGRAM:
	- MAKE A .TXT FILE WITH ALL TRANSITION INTENSITIES SEPARATED BY NEWLINES. WILL ONLY WORK
      IF THERE IS ONLY ONE COLUMN. THIS FILE NEEDS TO BE PREPARED BY HAND. PUT THIS FILE
      IN THE SAME FOLDER AS THE SOURCE CODE.
	- CHANGE INPUT FILENAME AS NECESSARY IN MAIN FUNCTION
	- CHANGE ERROR AS NECESSARY ABOVE
	- DEBUG WHEN READY.
    - INPUT THREE REFERENCE FREQUENCIES FOR COMBINATION DIFFERENCE. THE FIRST OUTPUT CORRESPONDS
	  TO TRANSITIONS FROM OR TO THE FIRST INPUTTED LEVEL AND THE SECOND AND THIRD OUTPUTS AS 
	  TRANSITIONS FROM OR TO THE SECOND AND THIRD INPUTS RESPECTIVELY. IF -1 IS ENTERED FOR THE 
	  THIRD INPUT, THE PROGRAM WILL ONLY CHECK ONE DIFFERENCE.
	- AN OUTPUT FILE WILL BE GENERATED LISTING ALL THE POSITIVE COMBINATION DIFFERENCES. CHANGE 
	  THE FORMATTING OF THE FILE AS NECESSARY AT THE END OF THE MAIN FUNCTION. 
	  
   NOTE: THIS PROGRAM CHECKS THE DIFFERENCE BETWEEN THE FIRST TWO INPUTS AND THE FIRST AND THIRD
		 INPUTS. THE CONDITIONALS MUST BE CHANGED IF OTHER DIFFERENCES ARE TO BE TESTED. 

   DEVELOPER'S LOG
	7/22/2014 - Program is created and running as intended. Positives match up with Mourad's
				FORTRAN program.
*************************************************************************************************/

int main()
{
	/* Set precision of transition frequencies. */
	cout << setiosflags(ios::fixed) << setprecision(5);

	/* Read input file and store in vector. */
	double ERROR;
	string inputfile;
	cout << "Input filename with extension" << endl;
	getline(cin, inputfile);

	cout << "Enter error in MHz" << endl;
	cin >> ERROR;

	std::ifstream infile(inputfile); /********** CHANGE INPUT FILENAME HERE **********/
	std::vector<double> lines;
	double val;
	while (infile >> val)
		lines.push_back(val);

	/* Energy of transitions to be tested against in combination difference. */
	double FREQA;
	double FREQB;
	double FREQC;

	cout << "First Frequency: ";
	cin >> FREQA;
	cout << "Second Frequency: ";
	cin >> FREQB;
	cout << "Third Frequency / Enter -1: ";
	cin >> FREQC;

	/* Differences that will be used for the combination differences. 
	   Here the first and second are the first difference and the first and third are the second difference. 

	   NOTE: For any changes to the differences, changes must be made to the corresponding conditionals as well.
				a is the first line.
				b is the second line
				c is the third line
			 Comparing the second and third difference means comparing b and c and so on... */
	double DIFFA = FREQB - FREQA;
	double DIFFB = FREQC - FREQA;

	/* This is the output file listing all positive hits. The file is 
	   created in the same folder as the source code. Format as necessary. */
	ofstream combdiff("output.txt");
	combdiff << setiosflags(ios::fixed) << setprecision(5); // Sets the precision of output transition frequencies. 
	int i = 0; // Counter for the number of positive hits.

	if (FREQC == -1) // Switch to test one difference instead of two.
	{
		for (int a = 0; a < lines.size(); a++)
		{
			for (int b = 0; b < lines.size(); b++)
			{
				if (lines[a] - lines[b] - DIFFA < ERROR && lines[a] - lines[b] - DIFFA > -1 * ERROR) // We first find a pair of lines that corresponds to the first difference.
				{
					cout << lines[a] << "\t" << lines[b] << endl;
					combdiff << lines[a] << "\n" << lines[b] << "\n" << 0 << "\n\n";
					i++;
				}
			}
		}
	}
	else
	{
		for (int a = 0; a < lines.size(); a++)
		{
			for (int b = 0; b < lines.size(); b++)
			{
				if (lines[a] - lines[b] - DIFFA < ERROR && lines[a] - lines[b] - DIFFA > -1 * ERROR) // We first find a pair of lines that corresponds to the first difference.
				{
					for (int c = 0; c < lines.size(); c++)
					{
						if (abs(lines[a] - lines[c] - DIFFB) < ERROR && lines[a] - lines[c] - DIFFB > -1 * ERROR) // Then we look for a third line which corresponds to the second difference.
						{
							cout << lines[a] << "\t" << lines[b] << "\t" << lines[c] << endl;
							combdiff << lines[a] << "\n" << lines[b] << "\n" << lines[c] << "\n\n";
							i++;
						}
					}
				}
			}
		}
	}

	if (i == 0) // Means no positive hits.
	{
		cout << "No positive hits." << endl;
		combdiff << "No positive hits." << endl;
	}

	// system("pause"); // Keep command prompt open. Not necessary if output is created. 
	return 0;
}