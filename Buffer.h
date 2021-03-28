/*
 * Buffer.h
 *
 *  Created on: Mar 21, 2021
 *      Author: phanh
 */

#ifndef BUFFER_H_
#define BUFFER_H_

class Buffer {
private:
	//pointer for string array containing buffer
	int* innerBuffer;
	int size; //size of array
	int index; //index of next cell
public:
	Buffer(int); //create a buffer by size of array
	void Write(int);
	int Read();
	void Print();
	bool IsFull(); //check if buffer is full
	bool IsEmpty(); //check if buffer is empty

};

#endif /* BUFFER_H_ */
