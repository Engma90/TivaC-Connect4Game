#include "CheckWin.h"



int winner(int P,int B[6][7],int row,int col){
	
	int counter=0,r=row,c=col;
	
	//v
	r=0;c=col;
	while(r<6){
	if(B[r][c]==P){
	counter++;
	}
	else{
	counter=0;
	}
	if(counter==4){
		return 1;
	}
	r++;
	}
	
	
	//h
	counter=0;
	r=row;c=0;
	
	while(c<7){
	if(B[r][c]==P){
	counter++;
	}
	else{
	counter=0;
	}
	if(counter==4){
		return 1;
	}
	c++;
	}
	
	//+d
	r=row,c=col;
	counter=0;
	while(r<=5 && c>=0){
		r++;
		c--;
	}
	r--;
	c++;
	
	while(r>=0 && c<7){
	if(B[r][c]==P){
	counter++;
	}
	else{
	counter=0;
	}
	if(counter==4){
		return 1;
	}
		r--;
		c++;
	}
	
	
	
	//-d
	r=row,c=col;
	counter=0;
	while(r>=0 && c>=0){
		r--;
		c--;
	}
	r++;
	c++;
	
	while(r<6 && c<7){
	if(B[r][c]==P){
	counter++;
	}
	else{
	counter=0;
	}
	if(counter==4){
		return 1;
	}
		r++;
		c++;
	}
	
	//no one won
	counter=0;
	for(r=0;r<6;r++){
		for(c=0;c<7;c++){
			if(B[r][c]!= 0 ){
			counter++;
			}
			if(counter==42){
				return 3;
			}
		}
	}
	
	


return 0;
}
