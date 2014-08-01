#include "Sample.h"


Sample::Sample(void)
{
}

Sample::Sample(string name){

	setType(name);

	if(name ==  "BlueRock"){

		setHSVmin(Scalar(88,165,47));
		setHSVmax(Scalar(117,256,110));

		setColor(Scalar(255,0,0));
	}

		if(name ==  "RedPuck"){
			
		setHSVmin(Scalar(172,205,61));
		setHSVmax(Scalar(179,256,100));

		setColor(Scalar(0,0,255));
	}

	if(name ==  "OrangePipe"){

		setHSVmin(Scalar(0,196,123));
		setHSVmax(Scalar(9,256,172));

		setColor(Scalar(51,153,255));
	}

		if(name ==  "PinkBall"){

		setHSVmin(Scalar(161,58,102));
		setHSVmax(Scalar(175,153,205));

		setColor(Scalar(180,105,180));
	}




}
Sample::~Sample(void)
{
}

int Sample::getxPos(){

	return Sample::xPos;
}

void Sample::setxPos( int x ){

	Sample::xPos = x;
}


int Sample::getyPos(){

	return Sample::yPos;
}

void Sample::setyPos( int y ){

	Sample::yPos = y;
}

int Sample::getarea(){

	return Sample::area;
}

void Sample::setarea( int area){

	Sample::area = area;

}

Scalar Sample::getHSVmin(){

	return Sample::HSVmin;
}

Scalar Sample::getHSVmax(){

	return Sample::HSVmax;
}

void Sample::setHSVmin(Scalar min){

	Sample::HSVmin = min;

}

void Sample::setHSVmax(Scalar max){

	Sample::HSVmax = max;
}