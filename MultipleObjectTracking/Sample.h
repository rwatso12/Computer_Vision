#pragma once
#include <string>
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace std;
using namespace cv;


class Sample
{
public:
	Sample(void);
	~Sample(void);

	Sample(string name);

	int getxPos();
	void setxPos( int x );

	int getarea();
	void setarea( int area ); 

	int getyPos();
	void setyPos( int y );

	Scalar getHSVmin();
	Scalar getHSVmax();
	 
	void setHSVmin( Scalar min );
	void setHSVmax( Scalar max );

	string getType(){return type;}
	void setType(string t){type = t;}

	Scalar getColor(){

		return Color;

	}

	void setColor(Scalar c){

		Color = c; 
	}
private:

	int xPos, yPos, area;
	string type;
	Scalar HSVmin, HSVmax; 
	Scalar Color;
};

