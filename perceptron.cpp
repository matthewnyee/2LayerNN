///////////////////////////////////////////////////////////////////////////////
// Matt Yee
// CS 545
// Machine Learning
// 1 May 2017
// Homework 2
///////////////////////////////////////////////////////////////////////////////
// perceptron.cpp
// Function definitions for perceptron class
///////////////////////////////////////////////////////////////////////////////

#include "perceptron.h"

// Class constructor for perceptron class.
// Initializes learning rate, number of images in data file, loads pointers to
// training data and testing data.
perceptron::perceptron(int hidden_nodes_count, data_node * the_train_head,
			 data_node * the_test_head, int image_count)
{	
	learning_rate = 0.1;
	data_count = image_count;
	test_head = the_test_head;
	train_head = the_train_head;

	// Setting up weights matrices (build/set number of rows in matrix)
	// 'input_weights' are weights from input nodes to hidden layer.
	// 'output_weights' are weights from hidden layer to output nodes.
	// 'change_input_weights' and 'change_output_weights' are for updating
	// 	weights and keeping track for usage on next round of calculations.
	input_weights = new float * [hidden_nodes_count];
	output_weights = new float * [1 + hidden_nodes_count]; // Add one for hidden level bias node
	change_input_weights = new float * [hidden_nodes_count];
	change_output_weights = new float * [1 + hidden_nodes_count];

	// Set up columns of matricies
	for(int i = 0; i < (hidden_nodes_count + 1); ++i)
	{
		input_weights[i] = new float[785];
		change_input_weights[i] = new float[785];
		output_weights[i] = new float[10];
		change_output_weights[i] = new float[10];
	}

	// Build last row of matrix for 'output' weight matricies
	// Has extra row because of bias node on hidden layer
	output_weights[hidden_nodes_count] = new float[10];
	change_output_weights[hidden_nodes_count] = new float[10];

	// Initialize 'change weights' tables to all zeroes
	for (int j = 0; j < (hidden_nodes_count + 1); ++j)
	{
		for (int i = 0; i < 785; ++i)
		{
			// Input weights matrix has one less row than output
			if (j < hidden_nodes_count) change_input_weights[j][i] = 0;
			// Output weights matrix only has 10 columns
			if (i < 10) change_output_weights[j][i] = 0;
		}
	}
	
	// Set weight matricies to random values between -0.05 and 0.05
 	srand(time(NULL));
	for (int j = 0; j < (hidden_nodes_count + 1); ++j)
	{
		for (int i = 0; i < 785; ++i)
		{
			// Get random number between 0 and 100, then float-divide by 1000.
			// Subtract by 0.05 to get weights between -0.05 and 0.05.
			if (j < hidden_nodes_count)
			{
				input_weights[j][i] = (-0.05 + n);
				float n = ((float) (rand() % 101)) / 1000;
			}
			if (i < 10)
			{
				// Get random number between 0 and 100, then float-divide by 1000.
				// Subtract by 0.05 to get weights between -0.05 and 0.05.
				float n = ((float) (rand() % 101)) / 1000;
				output_weights[j][i] = (-0.05 + n);
			}
		}
	}
}



// Perceptron class destructor
perceptron::~perceptron(void)
{
}



// Prints Input data
int data_node::print()
{
	cout << "\n\n\n\n actual is: " << actual_value;
	for (int j = 0; j < 784; ++j)
	{
		cout << "\n pixel is: " << data[j];
	}
	if (next) cout << "\nhas next\n";
}



// Trains neural net on one data point
int perceptron::train(data_node * curr, int hidden_nodes_count)
{	
	// Array of target values for each output node (10 total)
	// Target is 0.9 if output node is supposed to fire
	// Target is 0.1 if output node is NOT supposed to fire
	float target[10];
	for (int i = 0; i < 10; ++i) target[i] = 0.1;
	target[(curr->actual_value - 1)] = 0.9;

	// Array of hidden node layer activation values
	// Add one to array length for bias node
	// Index 0 is bias node, set to 1
	float * hidden = new float[hidden_nodes_count + 1];
	hidden[0] = 1;

	// Calculate hidden node layer activation values
	// Dot product of weights with input data (and bias node)
	// Store result in 'hidden' float array
	//
	// Iterate through all hidden nodes
	for (int i = 0; i < hidden_nodes_count; ++i)
	{
		float sum = 0;

		// Input layer bias node is 1, so 'input weight * bias' is just weight
		sum += input_weights[i][0];
		//cout << "\n sum is: " << sum;

		// Start at index 1 because index 0 was calculated above for bias node
		for (int j = 1; j < 785; ++j)
		{
			sum += input_weights[i][j] * curr->data[j];
			//cout << "\n\n\n\n sum is: " << sum;
			//cout << "\n input weight is: " << input_weights[i][j + 1];
			//cout << "\n input is: " << curr->data[j];
		}
		//cout << "\n sum is: " << sum;

		// Assign at hidden[i + 1] because hidden[0] is bias node
		// Pass counter 'sum' through sigma function (program defined)
		hidden[i + 1] = sigma(sum);
	}


	// Array of output node layer activation values (10 output nodes)
	float * output = new float[10];

	// Calculate output node layer activation values
	// Dot product of weights with hidden layer activation values (and bias node)
	// Store result in 'output' float array
	//
	// Iterate through all hidden nodes 
	for (int i = 0; i < 10; ++i)
	{
		float sum = 0;
		for (int j = 0; j < (hidden_nodes_count + 1); ++j)
		{
			sum += hidden[j] * output_weights[j][i];
		}
		output[i] = sigma(sum);
	}

	// Output layer error values (10 output nodes)
	float output_error[10];

	// target is what the node's activation value was supposed to be
	for (int i = 0; i < 10; ++i)
	{
		output_error[i] = output[i] * (1 - output[i]) * (target[i] - output[i]);
	}

	// Hidden layer error
	// Don't need "+1" because no hidden unit error? 
	// h * (1 - h) would be zero because bias is always 1 
	float * hidden_error = new float[hidden_nodes_count];

	// dot product 'output_error' with 'output_weights'
	// then apply to formula
	for (int i = 0; i < hidden_nodes_count; ++i)
	{
		float sum = 0;
		for (int j = 0; j < 10; ++j)
		{
			sum += output_error[j] * output_weights[i][j];
		}
		hidden_error[i] = hidden[i] * (1 - hidden[i]) * sum;
	}

	// Set changes in output weights
	// Use formula with previous weight and hidden layer activation values
	for (int j = 0; j < (hidden_nodes_count + 1); ++j)
	{
		for (int i = 0; i < 10; ++i)
		{
			change_output_weights[j][i] = (hidden[j] * output_error[i] * 0.1) + (0.9 * change_output_weights[j][i]);
		}
	}

	// Set changes in input weights
	// Use formula with previous weight, data point, and hidden layer error values
	for (int j = 0; j < hidden_nodes_count; ++j)
	{
		for (int i = 0; i < 785; ++i)
		{
			/*if (false){//j == 19){
				cout << "\n\n i is          		" << i;
				cout << "\n hidden error    		" << hidden_error[j];
				cout << "\n curr data is    		" << curr->data[i];
				cout << "\n prev change is  		" << change_input_weights[j][i];
			}*/
			change_input_weights[j][i] = (hidden_error[j] * curr->data[i] * 0.1) + (0.9 * change_input_weights[j][i]);
		}
	}

	// Update 'output_weights' with its changes
	for (int j = 0; j < (hidden_nodes_count + 1); ++j)
	{
		for (int i = 0; i < 10; ++i)
		{
			output_weights[j][i] += change_output_weights[j][i];
		}
	}
	
	// Update 'input_weights' with its changes
	for (int j = 0; j < hidden_nodes_count; ++j)
	{
		for (int i = 0; i < 785; ++i)
		{
			input_weights[j][i] = change_input_weights[j][i];
		}
	}
}



// Calculate accuracy of weights 
float perceptron::get_accuracy(int test_or_train, int hidden_nodes_count)
{
	float results[data_count]; // accuracy for each data point
	data_node * curr;

	// Set to training data or testing data, depending on input bool
	if (test_or_train == 0) curr = train_head; // why not this in if/else loop??? data_node * curr = train_head;
	else curr = test_head;

	int i = 0;

	// Iterate through all data points (all images)
	while (curr)
	{
		// Fetch accuracy for each data point (binary 1 or 0)
		results[i] = individual_accuracy(curr, hidden_nodes_count);
		++i;
		curr = curr->next;
	}

	// Iterate through results (accuracy for each data point: 1 or 0)
	// Get mean by summing all, dividing by total;
	int sum = 0;
	for (int j = 0; j < data_count; ++j) sum += results[j];
	return (float) sum / (float) data_count;
}



// Helper accuracy function
// Calculate accuracy for a single data point
float perceptron::individual_accuracy(data_node * node, int hidden_nodes_count)
{
	float target[10];
	for (int i = 0; i < 10; ++i) target[i] = 0.1;
	target[(node->actual_value - 1)] = 0.9;

	float outputs[10]; // each output node's value, to be compared with "target"

	float * hidden = new float[hidden_nodes_count + 1];
	hidden[0] = 1;

	float accuracy[10];

	float return_value = 0;

	// iterate through all hidden nodes
	for (int i = 0; i < hidden_nodes_count; ++i)
	{
		float sum = 0;
		sum += input_weights[i][0];
		for (int j = 1; j < 785; ++j) // b/c bias is at index 0, so start at 1
		{
			sum += input_weights[i][j] * node->data[j];
		}
		// Assign at hidden[i + 1] because hidden[0] is bias node
		hidden[i + 1] = sigma(sum);
	}

	for (int i = 0; i < 10; ++i)
	{
		float sum = 0;
		for (int j = 0; j < (hidden_nodes_count + 1); ++j)
		{
			sum += hidden[j] * output_weights[j][i];
		}
		outputs[i] = sigma(sum);
	}

	for (int i = 0; i < 10; ++i)
	{
		if (target[i] == 0.9)
		{
			if (outputs[i] >= 0.9) accuracy[i] = 1;
			else accuracy[i] = 0;
		}

		else if (target[i] == 0.1)
		{
			if (outputs[i] <= 0.1) accuracy[i] = 1;
			else accuracy[i] = 0;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		return_value += accuracy[i];
	}

	return (return_value / 10);
}



float perceptron::sigma(float value)
{
	return (1 / (1 + exp(value)));
}



// Function assigns to class variable 'head' head of LLL of input data
int perceptron::add_data(data_node * & data_head)
{
	//head = data_head;
	return 1;
}



// Function prints all data values of input starting from head of LLL
int perceptron::print_data(void)
{
	data_node * current = test_head;
	while (current)
	{
		cout << endl << current->actual_value << " ";
		for (int j = 0; j < 784; ++j)
		{
			cout << endl << j << ": " << current->data[j] << " ";
		}
		cout << endl;
		current = current->next;
	}
/*
	data_node * current = train_head;
	while (current)
	{
		cout << endl << current->actual_value << " ";
		for (int j = 0; j < 784; ++j)
		{
			cout << endl << j << ": " << current->data[j] << " ";
		}
		cout << endl;
		current = current->next;
	}
*/
	return 1;
}
