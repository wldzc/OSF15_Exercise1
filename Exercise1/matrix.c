#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/* 
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols 
 * INPUTS: 
 *	name the name of the matrix limited to 50 characters 
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if (!*(new_matrix)){
			printf("New matrix already exists!\n");
			return false;
	}
	else if (!name){
			printf("No name creates!\n");
			return false;
	}	
	else if (rows <= 0 || cols <= 0){
			printf("The number of rows or cols is invalid!\n");
			return false;
	}


	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1; 
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Free the memory allocated in the matrix
 * Input:
 * 		m: Matrix address that need to be freed
 * Return:
 * 		Return nothing
*/
void destroy_matrix (Matrix_t** m) {

	//ERROR CHECK INCOMING PARAMETERS
	if (!m){
			printf("No matrix exists to be destroyed!\n");
			return;
	}
	
	free((*m)->data);
	free(*m);
	*m = NULL;
}


	
	//TODO FUNCTION COMMENT
/*
 * Purpose:
 * 		Compare two matrices to see if they are equal
 * Input:
 * 		a: one of compared matrices 
 * 		b: one of compared matrices
 * Return:
 * 		Return true if two matrices are same
 * 		False if error occurs
*/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//ERROR CHECK INCOMING PARAMETERS	
	if (!a || !b || !a->data || !b->data) {
		printf("Either matrix a or matrix b is NULL!\n");
		return false;	
	}

	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Copy the matrix src as matrix dest
 * Inputs:
 * 		src: the source matrix
 * 		des: the destination matrix 
 *  Return:
 * 		True if nothing happens during the process of duplication
 * 		FALSE IF ERROR OCCURS
*/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//ERROR CHECK INCOMING PARAMETERS
	if (!src) {
		printf("No source matrix exists!\n");
		return false;
	}
	else if (!dest){
		printf("Destination matrix is not created!\n");
		return false;
	}
	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);	
	return equal_matrices (src,dest);
}

	//TODO FUNCTION COMMENT
/*
 * Purpose:  
 * 		Shift the matrix to a direction left or right according to the user for a certain bits. 
 * Inputs:
 * 		a: The matrix to be shifted
 * 		direction: The direction of shift, either left or right
 * 		shift: The amount of bits for the shift
 * Return: 
 * 		True if no error occurs during the shift process
 * 		False if error occurs during the process
*/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {
	
	//ERROR CHECK INCOMING PARAMETERS
	if (!a) {
		printf("No input matrix for the shift!\n");
		return false;
	}

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}
	
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose:
 * 		Add two matrix a and b together and then store the result into matrix c
 * Input:
 * 		a: one of the matrices to add
 * 		b: one of the matrices to add
 * 		c: the matrix where to put the add result
 * Return:
 * 		True if the add up process is successful and no error occurs
 * 		False if any error occurs during the process of adding
*/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//ERROR CHECK INCOMING PARAMETERS
	if (!a || !b){
		printf("The input matrices to add have error!\n");
		return false;
	}
	if (a->rows != b->rows && a->cols != b->cols) {
		printf("a and b are in different rows or cols, add failed\n");
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Display a selected matrix for user
 * Input:
 * 		m: the matrix to display
 * Return:
 * 		Return nothing 
*/
void display_matrix (Matrix_t* m) {
	
	//ERROR CHECK INCOMING PARAMETERS
	if (!m){
		printf("The input matrix is invalid!\n");
		return;
	}

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Open a file and read the matrix from it to use one in the program
 * Input:
 * 		matrix_input_filename: the name of the file selected
 * 		m: the matric created from the input file
 * Return:
 * 		True if a matrix is created from the file successfully
 * 		False if any error occurs during the process of the function
*/	
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {
	
	//ERROR CHECK INCOMING PARAMETERS
	if (!matrix_input_filename){
		printf("The file is invalid.\n");
		return false;
	}

	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;
	
	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;	
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);

	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose:
 * 		Open a file and write the content of given matrix to it
 * Input:
 * 		matrix_output_filename: the name of file to write the content
 * 		m: the matrix written to a given file
 * Return:
 * 		True if the content is written to a file successfully
 * 		False if any error occurs during the process
*/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {
	
	//ERROR CHECK INCOMING PARAMETERS
	if (!matrix_output_filename){
		printf("Written file is invalid!\n");
		return false;
	}
	else if (!m){
		printf("Input matrix is invalid!\n");
		return false;
	}
	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);	
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");	
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	
	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Generate a random value of range for a given matrix 
 * Inputs:
 * 		m: the selected matrix
 * 		start_range: the start range for the random value generated
 * 		end_range: the end range for the random value generated
 * Return:
 * 		True if a random value is successful generated
 * 		False if any error occurs during the process
*/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {
	//ERROR CHECK INCOMING PARAMETERS
	if (m == NULL){
		printf("The given matrix is invalid!\n");
		return false;
	}	
	else if (!start_range || !end_range){
		printf("The start range or end range is invalid!\n");
		return false;
	}
	else if (start_range > end_range){
		printf("The range of the matrix is not valid!\n");
		return false;
	}
	
	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Load a matrix from data into a matrix m by copying the memory
 * Inputs:
 * 		m: the matrix to be generated with data
 * 		data: the data that is used to generate the matrix
 * Return:
 * 		Return nothing 
*/
void load_matrix (Matrix_t* m, unsigned int* data) {
	//ERROR CHECK INCOMING PARAMETERS
	if (m == NULL){
			printf("The input matrix is Null!\n");
			return;
	}
	else if (data == NULL){
			printf("The data is Null!\n");
			return;
	}	
	
	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

	//TODO FUNCTION COMMENT
/*
 * Purpose: 
 * 		Add a given matrix to the array and destroy the existed matrix which will replace with the new given matrix
 * Inputs:
 * 		mats: the array of matrices
 * 		new_matrix: the given matrix to be added in the array
 * 		num_mats: the amount of matrices
 * Return:
 * 		The position of the new added matrix 
 * 		Otherwise return -1 if any error occurs during the process
 *  
*/
unsigned int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {
	//ERROR CHECK INCOMING PARAMETERS
	if (!mats){
			printf("The input array is invalid!\n");
			return -1;
	}	
	else if (!new_matrix){
			printf("No new matrix is added to the array!\n");
			return -1;
	}
	else if (!num_mats){
			printf("The amount of matrices is invalid!\n");
			return -1;
	}
		
	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	} 
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
