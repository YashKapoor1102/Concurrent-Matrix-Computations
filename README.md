# Concurrent-Matrix-Computations

## Description
This concurrent program consists of three processes that compute the determinant of a given 3x3 matrix.
Also, they compute the largest integer in the matrix. Upon completion, the determinant and the largest 
integer in the matrix is outputted to the user. 

This program is made up of two files:<br><br>
&nbsp;&nbsp;&nbsp;&nbsp;**DET.h**:		A header file<br>
&nbsp;&nbsp;&nbsp;&nbsp;**DET.c**:	 	Contains the code for the program<br><br>

&nbsp;&nbsp;&nbsp;&nbsp;**Makefile**:	Allows the user to compile and run the program

## Installation
Linux Operating System is required. 

If any other operating system (e.g., Windows, MAC OS) is being used, Oracle VM VirtualBox can be used to run Linux.

If Linux is not already installed on your operating system, use the following link to install it:

https://www.instructables.com/How-to-install-Linux-on-your-Windows/

## Usage

**Mandatory:**

**Step 1:** Clone the repository to your local machine:
```
$ git clone https://github.com/YashKapoor1102/Concurrent-Matrix-Computations.git
```

**Step 2:** Ensure the "Terminal" is opened.

**Step 3:** Go to the folder where you cloned the repository by typing the following command:
```
$ cd Concurrent-Matrix-Computations
```

**Step 4:** Once you are in the folder, type the following command to compile all the source files:

	$ make

**Step 5:** To run the program, type the following command:

	$ ./DET

This will display the matrix and output the determinant and largest integer in the matrix.


**Optional:**

**Step 6:** If you want to change the matrix, open the file called "DET.C".

Once you open it, you will see that the matrix has been defined inside the main method:

 	int m[3][3] = {
		{20, 20, 50}, 	// First row of the matrix
		{10, 6, 70}, 	// Second row of the matrix
		{40, 3, 2}	// Third row of the matrix
	};

**Step 7:** To test various matrices, simply change the values to other positive integers. For example, 
	if you want to test the following matrix:
	
M = 5   30  60
&nbsp;&nbsp;&nbsp;&nbsp;40  70  90
&nbsp;&nbsp;&nbsp;&nbsp;25  60  70

You will change the values of the matrix that has been defined to this:

int m[3][3] = {
&nbsp;&nbsp;&nbsp;&nbsp;{5, 30, 60}, 	// First row of the matrix
&nbsp;&nbsp;&nbsp;&nbsp;{40, 70, 90}, 	// Second row of the matrix
&nbsp;&nbsp;&nbsp;&nbsp;{25, 60, 70}	// Third row of the matrix
};

Make sure you save the file after changing the matrix.

**Step 8:** Repeat Steps 2-5 every time you change the matrix.

Once you are done using this application, you can type the following command in the terminal to clean up your files:
```
$ make clean
```

## Credits
- Yash Kapoor 
