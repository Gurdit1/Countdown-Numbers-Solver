// countdown_numbers_solution.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>


bool validNumInput(double num) {
	if (num == 100) {
		return true;
	}
	else if(num == 75){
		return true;
	}
	else if (num == 50) {
		return true;
	}
	else if (num == 25) {
		return true;
	}
	else if ((num>=1)&&(num<=10)) {
		return true;
	}
	else {
		return false;
	}

}

int randomNumWithinRange(int a, int b) {
	int n = a + (std::rand() % (b - a + 1));
	return (double)n;
}

double oper(double a, double b, double op) {
	//Returns the result of the operation

	if (op == 1) {
		return a + b;
	}
	else if (op == 2) {
		return a - b;
	}
	else if (op == 3) {
		return a * b;
	}
	else if (op == 4) {
		return a / b;
	}
	else {
		return -1; //Returns -1 as fallback
	}
}

double calcAns(std::vector<double> solution) {
	//Calculates the answer to a solution

	double ans = NULL; //Default value

	int i = 0;
	while (i < (solution.size() - 1)) {
		if (ans == NULL) {
			ans = oper(solution[i], solution[i + 2], solution[i + 1]);
		}
		else {
			ans = oper(ans, solution[i + 2], solution[i + 1]);
		}
		i += 2;
	}

	return ans;
}

double fitness(std::vector<double> solution, int answer) {
	//Calculates fitness of a solution

	double calcAnswer = calcAns(solution);
	if (calcAnswer - answer < 0) {
		return answer - calcAnswer;
	}
	else {
		return calcAnswer - answer;
	}
}

std::vector<double> smallChange(std::vector<double> solutionOrg) {
	//Swaps either two numbers or two operators ans avoids outputs like 1*-3

	int i, j;
	double iVal, jVal;
	std::vector<double> solution(solutionOrg);

	i = randomNumWithinRange(0, solution.size() - 1);
	j = randomNumWithinRange(0, solution.size() - 1);

	if (i % 2 == 0) {
		//If i is even (a number)
		
		//While loop ensures j is also a number
		while ((j % 2 != 0) || (i == j)) {
			j = randomNumWithinRange(0, solution.size() - 1);
		}
	}
	else {
		//If i is odd (an operator)

		//While loop ensures j is also a operation
		while ((j % 2 == 0) || (i == j)) {
			j = randomNumWithinRange(0, solution.size() - 1);
		}
	}

	//Stores temp values
	iVal = solution[i];
	jVal = solution[j];

	//Swaps values
	solution[i] = jVal;
	solution[j] = iVal;

	return solution;
}

std::vector<double> randomStart(std::vector<double> inputNumbers) {
	//Randomly generataes a valid solution from scratch

	//A solution is in the form {n1, 1-4, n2, 1-4, ..., n5, 1-4, n6} where n is an int and 1-4 represents an operator

	std::vector<double> solution;
	int i;
	double tempNum;

	while (inputNumbers.empty() == false) {
		i = randomNumWithinRange(0, inputNumbers.size() - 1);
		tempNum = inputNumbers[i];
		solution.push_back(tempNum);
		inputNumbers.erase(inputNumbers.begin() + i);

		//if statement stops the last operator from being appended as it is unneeded
		if (inputNumbers.empty() == false) {
			i = randomNumWithinRange(1, 4);
			solution.push_back(i);
		}

	}

	return solution;
}

std::vector<double> findSolutionRMHC(std::vector<double> inputNumbersOrg, int answer) {
	//Contains the Random mutation Hill Climbing algorithm

	std::vector<double> solution, newSolution;
	int iter = 100;
	double fit, newFit;

	//Clone inputNumbersOrg
	std::vector<double> inputNumbers(inputNumbersOrg); //TODO: Verify that this works otherwise make own method

	//Random start
	solution = randomStart(inputNumbers);

	//Starting fitness
	fit = fitness(solution, answer);

	int i = 0;
	while ((i < iter) && (fit != 0)) {
		
		//Small change
		newSolution = smallChange(solution);
		newFit = fitness(newSolution, answer);

		if (newFit < fit) {
			solution = newSolution;
			fit = newFit;
		}
		
		i += 1;
	}

	return solution;
}

std::vector<double> randomRestartHC(std::vector<double> inputNumbers, int answer) {
	//Contains the complete Random Restart Hill Climbing algorithm and calls the Random mutation Hill Climbing algorithm

	int iter=10000;
	std::vector<double> bestSolution;
	double bestFitness = NULL; //Default value
	double currentFitness;
	std::vector<double> solution;

	int i = 0;
	while(i<iter){
		solution = findSolutionRMHC(inputNumbers, answer);
		
		currentFitness = fitness(solution, answer);
		if (currentFitness == 0) {
			//Stops loop if solution is found
			i = iter;
		}
		else if (bestFitness == NULL) {
			//If first iteration
			bestSolution = solution;
			bestFitness = currentFitness;
		}
		else if (currentFitness < bestFitness) {
			//If new solution is better than current existing one
			bestSolution = solution; //TODO: Check if requires cloning
			bestFitness = currentFitness;
		}
		i += 1;

	}

	return solution;
}

std::string findSymbol(double n) {
	//Return the operator a number represents

	if (n == 1) {
		return " + ";
	}
	else if (n == 2) {
		return " - ";
	}
	else if (n == 3) {
		return " * ";
	}
	else if (n == 4) {
		return " / ";
	}
	else {
		return " SYMBOL ERROR ";
	}
}

void displayAnswer(std::vector<double> solution) {
	//Displays solution

	std::cout << "\n\tSolution:\n\n";
	double ans = NULL; //Default value

	int i = 0;
	while (i < solution.size()-1) {
		if (ans == NULL) {
			ans = oper(solution[i], solution[i + 2], solution[i + 1]);
			std::cout << solution[i] << findSymbol(solution[i + 1]) << solution[i + 2] << " = " << ans << "\n";
		}
		else {
			std::cout << ans;
			ans = oper(ans, solution[i + 2], solution[i + 1]);
			std::cout << findSymbol(solution[i + 1]) << solution[i + 2] << " = " << ans << "\n";
		}
		i += 2;
	}
}

void displayNumbers(std::vector<double> inputNumbers) {
	system("cls");
	std::cout << "\n\t";

	//Displays current numbers
	if (inputNumbers.empty() == false) {
		for (double number : inputNumbers) {
			std::cout << number << " ";
		}
	}
	std::cout << "\n\n";
}

void start() {
	double numInput;
	int target;
	std::vector<double> inputNumbers;
	std::vector<double> solution;
	std::string countArr[] = { "first" , "second", "third", "fourth", "fifth", "sixth" };

	//Gets numbers from user
	for(std::string count: countArr){
		displayNumbers(inputNumbers);
		
		std::cout << "Enter " << count << " number\n>>>";
		std::cin >> numInput;
		while (validNumInput(numInput)==false) {
			std::cout << "Invalid input: Input must be 1-10, 25, 50, 75 or 100\nEnter " << count << " number\n>>>";
			std::cin >> numInput;
		}
		inputNumbers.push_back(numInput);
	}

	displayNumbers(inputNumbers);

	//Gets target from user
	std::cout << "Enter target\n>>>";
	std::cin >> target;
	while (target > 999) {
		std::cout << "Invalid input: Target must be 999 or less\nEnter target\n>>>";
		std::cin >> target;
	}

	//Re-displays numbers and target
	displayNumbers(inputNumbers);
	std::cout << "\t\t" << target << "\n";

	solution = randomRestartHC(inputNumbers, target);
	
	displayAnswer(solution);

	double fit = fitness(solution, target);
	if (fit != 0) {
		std::cout << "\nOff by: " << fit << "\n";
	}
}

int main()
{
	std::string choice = "Y";
	while ((choice == "Y")||(choice == "y")) {
		start();
		std::cout << "\nDo you want to try again?\nEnter Y for yes and any other key for no\n>>>";
		std::cin >> choice;
		system("cls");
	}
    return 0;
}

