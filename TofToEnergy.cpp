/*
 * Time of flight distribution to energy conversion
 * First created by Liam Kelley 6/18/20
 * Last updated by Liam Kelley 8/10/20
 *
 *
 * This code takes raw energy calculations with their assigned counts, bins the counts by 1 eV and then groups
 * the counts by a user-inputted amount
 *
 * To run this code, have the energy file in the same folder as the executable.
 *
 * Enter the name of the file, and know whether it represents the energies from the fast peak
 * or the slow peak.
 *
 * Then, enter an size for the final bins, in eV.
 *
 * The resulting file is called "output.dat"
 *
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <bits/stdc++.h>
#include <conio.h>
#include <string>
#include <array>

using namespace std;

ifstream inFile;
ofstream outFile;

string inputFileName; //edit the names of the input file and what you want the output to be
string fastOrSlow; //user input for fast or slow peak
bool fastPeak;

string output; //output file name

array<double, 1000> data; //input from unbinned energies
int dataindex = 0; //index of that array

array<double, 500> binnedEnergy; //arrays for the intermediate and final steps in the binning process
array<double, 500> finalBinnedEnergy;
array<double, 500> binnedCounts;
array<double, 500> finalBinnedCounts;

double tempenergy; //energy holder for reading in
double tempcount; //count holder for reading in

int size = 0; //length of unbinned vectors
int newsize; //length of binned vectors

int highestBin; //upper limit of 1eV bins

double total = 0; //sum of counts
double lastTotal = 0;

double energyHolder; //holder for binning the energy
double countHolder; //holder for counts while the energy is binned

int closestLower; //nearest integer lower than the energy value
int closestHigher; //nearest integer higher than the energy value

double proportion; //ratio for counts assigned to closestLower and closestHigher

double binSize; //final bin size, controlled by the user

int main() {

	/*WORKING WITH THE FILES/READING IN*/

	cout << "Enter the name of the unbinned energy file\t"; //input file name
	cin >> inputFileName;

	output = inputFileName + "_binned";

	const char *inputName = (inputFileName + ".dat").c_str();

	inFile.open(inputName); //name the location of the file, should be in project folder

	while (!inFile.eof()) { //loop that gets each time/count and adds to the respective vectors ISSUE HERE
		inFile >> tempenergy;
		inFile >> tempcount; //reading in data to assign

		data[dataindex] = tempenergy; //putting into the data arrays
		dataindex++;
		data[dataindex] = tempcount;
		dataindex++;
		size++;

	}

	inFile.close();

	/*BEGINNING OF BINNED ENERGY IN 1EV BINS*/

	cout << "Is this a fast peak (f) or a slow peak (s)?\t"; //fast or slow peak
	cin >> fastOrSlow;

	if (fastOrSlow == "f") { //assigning fast or slow
		fastPeak = true;
	} else if (fastOrSlow == "s") {
		fastPeak = false;
	}

	if (fastPeak == true) { //determining where the highest energy bin is in the vector based on type of peak
		highestBin = ceil(data[0]);
	} else {
		highestBin = ceil(data[2 * size - 2]);
	}

	newsize = highestBin + 1;

	for (int i = 0; i < 2 * size; i += 2) { //loop for creating 1eV bins

		energyHolder = data[i];
		countHolder = data[i + 1]; //temporary values

		closestLower = floor(energyHolder); //the closest two integers to the value of the energy bins
		closestHigher = ceil(energyHolder);

		proportion = energyHolder - closestLower; //uses this proportion to assign the counts proportionately
		//to the higher and lower integer bins of the current energy bin

		binnedCounts[closestLower] = binnedCounts[closestLower] //adding up the new bins
		+ (1 - proportion) * countHolder;
		binnedCounts[closestHigher] = binnedCounts[closestHigher]
				+ (proportion) * countHolder;

	}

	for (int i = 0; i < newsize; i++) { //creating the intermediate energy vector
		binnedEnergy[i] = double(i);
		total = total + binnedCounts[i];

	}

	cout << total << endl;
	cout << newsize << endl; //tests to make sure the code runs


	/*ASSIGNING TO THE FINAL BIN SIZES, DETERMINED BY USER*/

	cout << "What size bins (in eV)?\t"; // prompt for the final bin size
	cin >> binSize;

	finalBinnedEnergy[0] = binnedEnergy[0]; //the 0 eV values
	finalBinnedCounts[0] = binnedCounts[0];

	int j = 0;
	int indexBin = 1;
	int indexFin = 1;

	do{ // this loop assigns the counts to their final bins

		j += binSize;

		double sumE = 0;
		double sumC = 0;

		for( ; indexBin <= j; indexBin++){ //counting every hit between bins

			sumE =	j;
			sumC += binnedCounts[indexBin];

		}

		finalBinnedEnergy[indexFin] = sumE;
		finalBinnedCounts[indexFin] = sumC;

		indexFin++;


	} while(j <= highestBin);

	for(int i = 0; i < indexFin; i++){ //checking the total amount of counts again
		lastTotal += finalBinnedCounts[i];
	}


	cout << lastTotal << endl;

	/*OUTPUTTING TO FILE*/

	outFile.open("output.dat");

	for (int k = 0; k < newsize; k++) { //writing to file
		outFile << finalBinnedEnergy[k];
		outFile << " ";
		outFile << finalBinnedCounts[k];
		outFile << "\n";

	}

	outFile.close();


	return 0;

}
