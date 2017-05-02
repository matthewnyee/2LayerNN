///////////////////////////////////////////////////////////////////////////////
// Matt Yee
// CS 545
// Machine Learning
// 1 May 2017
// Homework 2
///////////////////////////////////////////////////////////////////////////////
// perceptron.h
// Function declarations for perceptron class
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

// Node to hold data when read from file
struct data_node
{
	int actual_value; // What number handwritten image is supposed to represent
	float data[784]; // Grayscale pixels of image (normalized to between 0 and 1)
	data_node * next; // Next image

	int print();
};

// Perceptron class
class perceptron
{
	public:
		perceptron(int hidden_nodes_count, data_node * the_train_head, data_node * the_test_head, int image_count);
		~perceptron(void);

		// Functions to I/O, data and weights
		int add_data(data_node * & head);
		int print_data(void);
		int print_weights(void);


		int train(data_node * curr, int hidden_nodes_count);
		float sigma(float sum);
		float get_accuracy(int, int);
		float individual_accuracy(data_node *, int);

/*		// Prints confusion matrix 
		int print_confusion_matrix(void);
		// Helper function: gets matrix data for a single data point 
		int get_values(data_node * current_node, int &actual, int &predict);
*/
	private:
		float learning_rate; // Learning rate (0.1, 0.01, or 0.001)
		int data_count; // Number of images in entire data set
		data_node * test_head; // Pointer to LLL of data read in from file
		data_node * train_head;

		float ** input_weights;
		float ** output_weights;

		float ** change_input_weights;
		float ** change_output_weights;
};
