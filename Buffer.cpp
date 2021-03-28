/*
 * Buffer.cpp
 *
 *  Created on: Mar 21, 2021
 *      Author: phanh
 */
#include <stdio.h>
#include <string>
#include "Buffer.h"
using namespace std;


Buffer::Buffer(int bufferSize) {
	// TODO Auto-generated constructor stub
	size=bufferSize;
	innerBuffer=  new int[size]; //create a buffer with this size of array
	for (int i=0; i<bufferSize;i++){
		innerBuffer[i]=0; //let the pointer start from 0
	}
	index=bufferSize;//attach index to each cell
}

void Buffer::Write(int data){
	 innerBuffer[index]=data;
	 index++;
}

int Buffer::Read(){
	index--;
	return innerBuffer[index];
}
void Buffer::Print(){
	std::string piece;
	printf("---> [");
	for (int i=0; i<size;i++){
		if (i<index)
			printf("%2d",innerBuffer[i]);
		else
			printf(" ");
		if (i != size-1)
			printf("|");
	}
	printf("]\n");
}
bool Buffer::IsFull(){
	return index == size;

}

bool Buffer::IsEmpty(){
	return index == 0;
}



