#include <stdio.h>

void Mat_vect_mult(
	double A[],	 /* in */
	double x[],  /* in */
	double y[],  /* OUT */
	int m,       /* in */
	int n ){     /* in */

	for (int i=0; i<m; i++) {
		y[i]=0;
		for (int j=0;j<n;j++) {
			y[i]=A[i∗n+j]∗x[j];
		}
	}
} /* Mat vect mult */

int main() {
	//Defición de varibles
	Mat_vect_mult(A,x,y,m,n);

}