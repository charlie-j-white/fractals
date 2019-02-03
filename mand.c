/*
overall set
depth=1000;
	iteration=50;
	centrex=-0.5;
	centrey=0.0;
	sidelength=2;
	
seahorse valley
depth=1000;
	iteration=200;
	centrex=-0.79;
	centrey=0.16;
	sidelength=0.03;

satellite
depth=500;
	iteration=2000;
	centrex=-0.743643;
	centrey=0.1318265;
	sidelength=0.000015;
	L=390;	

satellite valley
depth=200;
	iteration=3000;
	centrex=-0.74364410;
	centrey=0.13182614;
	sidelength=0.0000006;
	L=900;		

satellite seahorses
depth=2000;
	iteration=5000;
	centrex=-0.743643918;
	centrey=0.131825910;
	sidelength=0.00000005;
	L=900;	

julia island
depth=400;
	iteration=6000;
	centrex=-0.74364389112598;
	centrey=0.13182591316075;
	sidelength=0.00000000007;
	L=1000;
	

*/

#include "lodepng.h"	//must include lodepng.h and lodepng.c file in the working directory
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>

/*
FORMATTING
Encode from raw pixels to disk with a single function call
The image argument has width * height RGBA pixels or width * height * 4 bytes
*/
void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(filename, image, width, height);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

int main(int argc, char *argv[])
{
    const char* filename = argc > 1 ? argv[1] : "1shadedmandelbrotset.png";

    //open the fractal variables
	double xx, yy, rf, gf, bf, centrex, centrey, sidelength, shading, escapeval, kval, iteration, cc, h, X, dd; 
	double logzn, nu, L;
	int  jj, kk, depth, logsensitivity;
	//FILE *file;
	
	//open the file
	//file=fopen("sawg.txt","w");
	
	//------------------------------------------------EDIT THESE--------------------------------------------------------------
	depth=300;
	iteration=600;
	centrex=-0.7;
	centrey=0.0;
	sidelength=2.5;
	L=60;									// shading factor, increase to get rid of bands and widen colour intervals
	//------------------------------------------------EDIT THESE--------------------------------------------------------------
	
	//initial conditions
	xx=-sidelength/2+centrex;
	yy=sidelength/2+centrey;
	kval=0;
	escapeval=2;
	
	//open the image variables
	unsigned width = depth, height = depth;
	unsigned char* image = malloc(width * height * 4 );
	unsigned x, y;

    //start the looping
	for(y = 0; y < height; y++){
		for(x = 0; x < width; x++){
			
			rf = 0, gf = 5, bf = 8;
			double complex z1 = xx + yy * I;
			double complex z2 = 0 + 0 * I;;
			
			
			//start the mandelbrot algorithm-------------------------------------------------------------------------------------------
			for(kk=0; kk<=iteration; kk++){
				
				//do the algorithm
				z2 = z2*z2 + z1;
				
				//for the main converged areas
				if(xx < sqrt((xx-0.25)*(xx-0.25)+yy*yy) - 2*((xx-0.25)*(xx-0.25)+yy*yy) +0.25 ){
					rf = 0, gf = 5, bf = 8;
					break;
				}
				if( (xx+1)*(xx+1) + yy*yy < 0.0625 ){
					rf = 0, gf = 5, bf = 8;
					break;
				}
				
				//for the divergent area
				if(cabs(z2)>=escapeval){					
					
					//apply smooth colour formula
					logzn = 0.5*log( cabs(z2) );
					nu = log( logzn/log(2) )/log(2);
					kval = kk + 1 -nu;
					kval = fmod( kval,L );
					//fprintf(file, "%lf\t%lf\t%lf\n", logzn, nu, kval);
					
					//define colours
					double a1=90, a2=0, a3=20;
					double b1=225, b2=225, b3=225;
					double c1=0, c2=220, c3=0;
					
					//colour the modulo scale
					if(0 <= kval && kval <= 0.33*L){			//g  r			
						rf = a1+(kval)*(b1-a1)/(0.33*L), gf = a2+(kval)*(b2-a2)/(0.33*L), bf = a3+(kval)*(b3-a3)/(0.33*L);
						break;
					}
					
					if(0.33*L <= kval && kval <= 0.66*L){		//r  w
						rf = b1+(kval-0.33*L)*(c1-b1)/(0.33*L), gf = b2+(kval-0.33*L)*(c2-b2)/(0.33*L), bf = b3+(kval-0.33*L)*(c3-b3)/(0.33*L);
						break;
					}
					
					if(0.66*L <= kval && kval <= L){			//w  g			
						rf = c1+(kval-0.66*L)*(a1-c1)/(0.34*L), gf = c2+(kval-0.66*L)*(a2-c2)/(0.34*L), bf = c3+(kval-0.66*L)*(a3-c3)/(0.34*L);
						break;
					}
					
					break;
				}
			}//------------------------------------------------------------------------------------------------------------------------
			
			
		//pattern RGBA	
	    unsigned red = rf, green = gf, blue = bf;
		image[4 * width * y + 4 * x + 0] = red;
		image[4 * width * y + 4 * x + 1] = green;
		image[4 * width * y + 4 * x + 2] = blue;
		image[4 * width * y + 4 * x + 3] = 225;

		//reset the values
		xx=xx+sidelength/depth;
		kval=0;
	}
	xx=-sidelength/2+centrex;
	yy=yy-sidelength/depth;
  }

  /*run the formatting function*/
  encodeOneStep(filename, image, width, height);

  free(image);
  
  //fclose(file);
  return 0;
}





						/*
						
						rf = 0, gf = (-cc)*(150)/(-0.5), bf = (-cc)*(150)/(-0.5);
						
						rf = (0.5-cc)*(225)/(-0.5), gf = 150+(0.5-cc)*(225-150)/(-0.5), bf = 150+(0.5-cc)*(225-150)/(-0.5);
						*/
