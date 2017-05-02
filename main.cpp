///////////////////////////////////////////////////////////////////////////////
// Matt Yee
// CS 545
// Machine Learning
// 1 May 2017
// Homework 2
///////////////////////////////////////////////////////////////////////////////
// main.cpp
// Contains program main function
/////////////////////////////////////////////////////////////////////////////// 

#include "perceptron.h"

int main(void)
{
	///////////////////////////////////////////////////////////////////////
	// Read in data set used to train perceptrons (~60,000 data points)
	data_node * the_head = NULL;
	ifstream fin;
	fin.open("mnist_train.csv");
	float results[785] = {}; // Temporary storage for each pixel of a data point
	char buf[3140]; // Buffer holds one line from input file
	int data_count = 0; // Count of total number of data points in file

	///////////////////////////////////////////////////////////////////////
	// Getting each data point (one line in the file = one data point/image)
	while (fin.getline(buf, 3140))	
	{
		++data_count; // Counting total number of data points in file
		char * token = new char[4];

		// Get first number from data point
		// This number is what handwritten image is supposed to represent
		token = strtok(buf, ","); // First strtok uses buf, then use NULL?
		results[0] = atoi(token);

		// Read rest of data point (pixels of image of handwritten letter)
		// Divide grayscale pixel by 255 to keep pixels between 0 and 1
		for (int i = 1; i < 785; ++i)
		{
			token = strtok(NULL, ","); // use NULL w/strtok here?
			results[i] = (float) atoi(token) / 255;
		}
		// Results array finished. All data read in.
		// Contains all pixels for one data point (one handwritten image)

		////////////////////////////////////////////////////////////////
		// Add to LLL this entire data point:
		if (!the_head)
		{

			the_head = new data_node;
			// Assign what number image is supposed to represent
			the_head->actual_value = results[0];
			// Assign rest of pixel data to "data" array
			the_head->data[0] = results[1];	
			for (int j = 1; j < 785; ++j)
			{
				the_head->data[j] = results[j + 1];
			}
			the_head->next = NULL;
		}
		else
		{
			data_node * curr = new data_node;
			curr->actual_value = results[0];
			curr->data[0] = results[1];
			for (int j = 2; j < 785; ++j)
			{
				curr->data[j] = results[j + 1];
			}
			curr->next = the_head;
			the_head = curr;
		}
	}

	////////////////////////////////////////////////////////////////////////
	// Read in test data into LLL held by 'test_head' pointer
	data_node * test_head = NULL;
	ifstream test_file;
	test_file.open("mnist_test.csv");
	float test_results[785] = {};
	char test_buf[3140];
	int test_data_count = 0; // Total number of data points (images) in file

	///////////////////////////////////////////////////////////////////////
	// Getting each data point (one line in the file = one data point)
	while (test_file.getline(test_buf, 3140))	
	{
		++test_data_count; // Counting total number of values (images) in file
		//fin.getline(buf, 3140);

		char * token = new char[4];

		// Get first number from data point
		// This number is what handwritten image is supposed to represent
		token = strtok(test_buf, ","); // First strtok uses buf, then use NULL?
		test_results[0] = atoi(token);

		// Read rest of data point (pixels of image of handwritten letter)
		// Divide grayscale pixel by 255 to keep pixels between 0 and 1
		for (int i = 1; i < 785; ++i)
		{
			token = strtok(NULL, ","); // use NULL w/strtok here?
			test_results[i] = (float) atoi(token) / 255;
		}
		// Results array now finished. All data read in.
		// Contains all pixels for one data point (one handwritten image)

		///////////////////////////////////////////////////////////////	
		// Add to LLL this entire data point:
		if (!test_head)
		{

			test_head = new data_node;
			// Assign what number image is supposed to represent
			test_head->actual_value = test_results[0];
			// Assign rest of pixel data to "data" array
			test_head->data[0] = test_results[1];	
			for (int j = 1; j < 785; ++j)
			{
				test_head->data[j] = test_results[j + 1];
			}
			test_head->next = NULL;
		}
		else
		{
			data_node * curr = new data_node;
			curr->actual_value = test_results[0];
			curr->data[0] = test_results[1];
			for (int j = 2; j < 785; ++j)
			{
				curr->data[j] = test_results[j + 1];
			}
			curr->next = test_head;
			test_head = curr;
		}
	}




	cout << "\n\nstart testing\n\n";
	perceptron n20(20, the_head, test_head, data_count);
	perceptron n50(50, the_head, test_head, data_count);
	perceptron n100(100, the_head, test_head, data_count);
	cout << "\n n20 train acc:  " << n20.get_accuracy(0, 20);
	cout << "\n n20 test acc:   " << n20.get_accuracy(1, 20);
	cout << "\n n50 train acc: " << n50.get_accuracy(0, 50);
	cout << "\n n50 test acc: " << n50.get_accuracy(1, 50);
	cout << "\n n100 train acc: " << n100.get_accuracy(0, 100);
	cout << "\n n100 test acc : " << n100.get_accuracy(1, 100);	

	//curr->print();
	data_node * curr = test_head;
	// for i < 50
		while (curr)
		{
			n20.train(curr, 20);
			n50.train(curr, 50);
			n100.train(curr, 100);
			curr = curr->next;
			cout << endl << endl;
			cout << "\n n20 train acc:  " << n20.get_accuracy(0, 20);
			cout << "\n n20 test acc:   " << n20.get_accuracy(1, 20);
			cout << "\n n50 train acc: " << n50.get_accuracy(0, 50);
			cout << "\n n50 test acc: " << n50.get_accuracy(1, 50);
			cout << "\n n100 train acc: " << n100.get_accuracy(0, 100);
			cout << "\n n100 test acc : " << n100.get_accuracy(1, 100);	
		}
		// ???float accuracy[50];
		//
	// get acc again at end
}

