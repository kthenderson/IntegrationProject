/* Project: Option 1 for module PHY2027
Date: 22/11/2018
 
Program is used to integrate any polynomial function defined by the user with up to 10 terms
The trapezoidal rule, Simpson's rule and the midpoint rule are all options in a menu*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NMAX 10

// Structure definitions
typedef struct polynom{
    int w;
    float coefficients[NMAX];
    float order[NMAX];
    int n;
    int a;
    int b;
    int width;
    float error;
} Polynom;

// Function prototypes
int polynomial(Polynom *poly);
int limits(Polynom *poly);
void trapez_rule(Polynom *poly, float *T);
void simpson_rule(Polynom *poly, float *S);
void midpoint_rule(Polynom *poly, float *S);
void find_error(Polynom *poly);

int main(){
    
    // Dynamic memory allocation
    Polynom *poly = NULL;
    poly = (Polynom*)malloc((NMAX)*sizeof(Polynom));
    
    
    // Calling functions to define polynomial and limits
    polynomial(poly);
    limits(poly);

    // Asking user to choose integration method
    printf("\n Calculation Menu\n\n"
           "\t 1. Integrate using the trapezoidal rule\n"
           "\t 2. Integrate using Simpson's rule\n"
           "\t 3. Integrate using the midpoint rule\n"
           "\t 4. Quit program\n");
    
    int choice;
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            printf("You chose to integrate your polynomial using the trapezoidal rule\n");
            
            // Defining final value and dynamic memory allocation
            float *T = 0;
            T = (float*)malloc((NMAX)*sizeof(float));
            
            // Calling functions to calculate value of integral and error
            trapez_rule(poly, T);
            find_error(poly);
            printf("The error between the calculated value and the actual value of the integral is %.2f\n", fabsf(poly->error - *T));
            
            // Freeing memory
            free(T);
            break;
            
        case 2:
            printf("You chose to integrate your polynomial using Simpson's rule\n");
            
            // Defining final value and dynamic memory allocation
            float *S = 0;
            S = (float*)malloc((NMAX)*sizeof(float));
            
            // Calling functions to calculate value of integral and error
            simpson_rule(poly, S);
            find_error(poly);
            printf("The error between the calculated value and the actual value of the integral is %.2f\n", fabsf(poly->error - *S));
            
            // Freeing memory
            free(S);
            break;
            
        case 3:
            printf("You chose to integrate your polynomial using the midpoint rule\n");
            
            // Defining final value and dynamic memory allocation
            float *M = 0;
            M = (float*)malloc((NMAX)*sizeof(float));
            
            // Calling functions to calculate value of integral and error
            midpoint_rule(poly, M);
            find_error(poly);
            printf("The error between the calculated value and the actual value of the integral is %.2f\n", fabsf(poly->error - *M));
            
            // Freeing memory
            free(M);
            break;
            
        // Quitting program
        case 4:
            printf("Goodbye and thanks for using my program\n");
            return 0;
            
        // Sanity check
        default:
            printf("Unknown option: %d\n", choice);
            return 1;
    }
}


// Function to define coefficients and order of each term in the polynomial
int polynomial(Polynom *poly){
    
    printf("Please enter the number of terms in the polynomial (must be an integer less than 10)\n");
    scanf("%d", &poly->n);
    
    // Sanity check
    int check = 0;
    
    while (poly->n >= 9){
        printf("Error, please try again and enter an integer less than 10\n");
        scanf("%d", &poly->n);
        ++check;
        if (check>10){
            printf("Incorrect input detected multiple times, quitting program\n");
            return -1;
        }
    }
    
    // Assiging coefficients and order of each term to arrays
    for (int i=0; i<poly->n; ++i){
        printf("Please enter the coefficient and order for term %d\n", i+1);
        scanf("%f" "%f", &poly->coefficients[i], &poly->order[i]);
        printf("Coefficient = %.1f, order = %.1f\n", poly->coefficients[i], poly->order[i]);
    }
    
    return 0;
}

// Function asks user to input limits and does sanity checks before calculating the width of the interval
int limits(Polynom *poly){
    
    int check = 0;
    
    // Asking for limits to be entered
    printf("Please enter two integers you would like to integrate between\n");
    scanf("%d %d", &poly->a, &poly->b);
    
    // Sanity check
    if (poly->b > poly->a){
        poly->w = poly->b - poly->a;
    }else if (poly->a > poly->b){
        int hold = poly->a;
        poly->a = poly->b;
        poly->b = hold;
        poly->w = poly->b - poly->a;
    }else{
        while (poly->a == poly->b){
            printf("Error, please try again and enter two different integers: \n");
            scanf("%d %d", &poly->a, &poly->b);
            ++check;
            if (check>10){
                printf("Incorrect input detected multiple times, quitting program\n");
                return -1;
            }
        }
    }
    return 0;
}

// Function to calculate the result of using trapezoidal rule on a polynomial
void trapez_rule(Polynom *poly, float *T){
    
    int width;
    
    // Asking for width of each strip
    printf("Please enter the width of each strip\n");
    scanf("%d", &width);
    
    // Defining variables
    int num = (poly->w)/width;
    int x[num];
    int *s = NULL;
    int y[num];
    int *sum = 0;
    
    // Dynamic memory allocation
    s = (int*)malloc((NMAX)*sizeof(y));
    sum = (int*)malloc((NMAX)*sizeof(num));
    
    // First for loop calculates x values
    for(int i=0; i<=num; i++){
        x[i]=(poly->a)+i*width;
        *s = 0;
        // Second for loop calculates the y value for each x value
        for(int j=0; j<poly->n; j++){
            *s = *s + poly->coefficients[j]*pow(x[i],poly->order[j]);
        }
        y[i] = *s;
    }
    
    // Calculating sum of terms
    for(int j=1; j<num; j++){
        *sum = *sum + y[j];
    }
    
    // Calculating and printing answer to 2 decimal places
    *T = (width/2.0) * (y[0] + y[num] + 2*(*sum));
    printf("Your answer is = %.2f\n", *T);
}

// Function to calculate the result of using Simpson's rule on a polynomial
void simpson_rule(Polynom *poly, float *S){
    
    // Asking for width of each strip
    printf("Please enter the width of each strip\n");
    scanf("%d", &poly->width);
    
    // Defining variables
    int num = (poly->w)/poly->width;
    int x[num];
    int *s = NULL;
    int y[num];
    int *sumodd = 0;
    int *sumeven = 0;
    
    // Dynamic memory allocation
    s = (int*)malloc((NMAX)*sizeof(y));
    sumodd = (int*)malloc((NMAX)*sizeof(num/2));
    sumeven = (int*)malloc((NMAX)*sizeof(num/2));
    
    // First for loop calculates x values
    for(int i=0; i<=num; i++){
        x[i]=(poly->a)+i*poly->width;
        *s = 0;
        // Second for loop calculates the y value for each x value
        for(int j=0; j<poly->n; j++){
            *s = *s + poly->coefficients[j]*pow(x[i],poly->order[j]);
        }
        y[i] = *s;
    }
    
    // Calculating sum of terms
    for(int j=1; j<(num); j+=2){
        *sumodd = *sumodd + y[j];
    }
    
    for(int k=2; k<(num); k+=2){
        *sumeven = *sumeven + y[k];
    }
    
    // Calculating and printing answer to 2 decimal places
    *S = (poly->width/3.0) * (y[0] + y[num] + 4*(*sumodd) + 2*(*sumeven));
    printf("Your answer is = %.2f\n", *S);
}

// Function to calculate the result of using the midpoint rule on a polynomial
void midpoint_rule(Polynom *poly, float *M){
    
    int width;
    
    // Asking for width of each strip
    printf("Please enter the width of each strip\n");
    scanf("%d", &width);
    
    // Defining variables
    int num = (poly->w)/width;
    int x[num+1];
    int *s = NULL;
    float y[num];
    
    // Dynamic memory allocation
    s = (int*)malloc((NMAX)*sizeof(y));
    
    // First for loop calculates x values
    for(int i=0; i<=num; i++){
        x[i]=(poly->a)+(i*width) + 0.5;
        *s = 0;
        // Second for loop calculates the y value for each x value
        for(int j=0; j<poly->n; j++){
            *s = *s + poly->coefficients[j]*pow(x[i],poly->order[j]);
        }
        y[i] = *s;
    }
    
    // Calculating sum of terms and printing answer
    for(int j=0; j<num; j++){
        *M = *M + (y[j]+y[j+1])/2;
    }
    printf("Your answer is = %.2f\n", *M);
}

// Function to find error compared to exact integral
void find_error(Polynom *poly){
    
    // Defining variables and dynamic memory allocation
    int x[2];
    int integ[2];
    int *s = NULL;
    s = (int*)malloc((NMAX)*sizeof(integ));
    x[0] = poly->b;
    x[1] = poly->a;
    
    // Integrating function
    for(int i=0; i<poly->n; i++){
        poly->order[i] = poly->order[i] + 1;
        poly->coefficients[i] = poly->coefficients[i]/poly->order[i];
    }
    
    // Calculating exact value of integral
    int error1 = 0;
    int error2 = 0;
    
    for(int j=0; j<2; j++){
        *s = 0;
        for(int k=0; k<poly->n; k++){
            *s = *s + poly->coefficients[k]*pow(x[j],poly->order[k]);
            integ[k] = *s;
            if(j==0)
                error1 = integ[k];
            else
                error2 = integ[k];
        }
    
    }
    // Difference between exact value and calculated value
    poly->error = error1 - error2;
}

/* Results
 
 Trapezoid Rule:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 3
 Please enter the coefficient and order for term 1
 1
 3
 Coefficient = 1, order = 3
 Please enter the coefficient and order for term 2
 1
 2
 Coefficient = 1, order = 2
 Please enter the coefficient and order for term 3
 2
 1
 Coefficient = 2, order = 1
 Please enter two integers you would like to integrate between
 5
 1
 
 Calculation Menu
 
 1. Integrate using the trapezoidal rule
 2. Integrate using Simpson's rule
 3. Integrate using the midpoint rule
 4. Quit program
 1
 You chose to integrate your polynomial using the trapezoidal rule
 Please enter the width of each strip
 1
 Your answer is = 228.00
 The error between the calculated value and the actual value of the integral is 7.00
 Program ended with exit code: 0
 
 Simpson's Rule:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 2
 Please enter the coefficient and order for term 1
 3
 2
 Coefficient = 3, order = 2
 Please enter the coefficient and order for term 2
 2
 1
 Coefficient = 2, order = 1
 Please enter two integers you would like to integrate between
 5
 1
 
 Calculation Menu
 
 1. Integrate using the trapezoidal rule
 2. Integrate using Simpson's rule
 3. Integrate using the midpoint rule
 4. Quit program
 2
 You chose to integrate your polynomial using Simpson's rule
 Please enter the width of each strip
 1
 Your answer is = 148.00
 The error between the calculated value and the actual value of the integral is 0.00
 Program ended with exit code: 0
 
 Midpoint Rule:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 2
 Please enter the coefficient and order for term 1
 3
 2
 Coefficient = 3, order = 2
 Please enter the coefficient and order for term 2
 2
 1
 Coefficient = 2, order = 1
 Please enter two integers you would like to integrate between
 5
 1
 
 Calculation Menu
 
 1. Integrate using the trapezoidal rule
 2. Integrate using Simpson's rule
 3. Integrate using the midpoint rule
 4. Quit program
 3
 You chose to integrate your polynomial using the midpoint rule
 Please enter the width of each strip
 1
 Your answer is = 150.00
 The error between the calculated value and the actual value of the integral is 2.00
 Program ended with exit code: 0
 
 Number of terms is too large:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 12
 Error, please try again and enter an integer less than 10

 Limits are equal:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 2
 Please enter the coefficient and order for term 1
 3
 2
 Coefficient = 3, order = 2
 Please enter the coefficient and order for term 2
 2
 1
 Coefficient = 2, order = 1
 Please enter two integers you would like to integrate between
 5
 5
 Error, please try again and enter two different integers:
 5
 5
 Error, please try again and enter two different integers:
...
 Incorrect input detected multiple times, quitting program
 Program ended with exit code: 0
 
 Quitting Program:
 
 Please enter the number of terms in the polynomial (must be an integer less than 10)
 2
 Please enter the coefficient and order for term 1
 3
 2
 Coefficient = 3, order = 2
 Please enter the coefficient and order for term 2
 2
 1
 Coefficient = 2, order = 1
 Please enter two integers you would like to integrate between
 5
 1
 
 Calculation Menu
 
 1. Integrate using the trapezoidal rule
 2. Integrate using Simpson's rule
 3. Integrate using the midpoint rule
 4. Quit program
 4
 Goodbye and thanks for using my program
 Program ended with exit code: 0
 */
