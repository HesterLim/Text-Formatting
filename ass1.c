/* Solution to comp10002 Assignment 1, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Hester Tze Hung Lim , 1044793
   Dated: 10 September 2019  

*/

// Import the necessary library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

/* assume that input lines will be at most 999 characters long */
#define LINELEN	999 
/* each output line emitted is as long as possible, but not longer 
than 50 characters; */
#define MAX_OUTPUT_CHAR 50 
/* At most five levels of headings may occur, counting from nn = 1 
(the top-level heading) to nn = 5
*/
#define MAX_HEADING_LEVEL 5

/* function prototypes */
int read_line(char *line, int maxlen);
int mygetchar();
int remove_whitespace();
void print_prompt(void);
int process_line(char *line, int line_len, int value, int width, 
	int *heading_num);
int getword(char *line, int line_len, int value, int width);
int left_margin(int value);
int center_margin(char *line, int width);
	
int get_left_margin(char *line, int value);
int get_width(char *line, int width);

int show_header(int number, int width ,int *heading_num, int value);
int get_header(char *line, int width, int *heading_num, int value);
int new_header(int number, int *heading_num); //initialize all array back to 1

// Main function
int
main(int argc, char **argv) {
	char line[LINELEN+1];
	
	// store the length of line;
	int line_len = 0;
	int value = 4;
	int width = 50;
	
	int heading_num[MAX_HEADING_LEVEL + 1];
	int i;
	
	// Initiliaze the values
	for(i = 1; i < MAX_HEADING_LEVEL + 1; i++){
		heading_num[i] = 0;	
	}
	heading_num[0] = 1;
	
	print_prompt();
	left_margin(value);
	while (read_line(line, LINELEN)) {
		if (strlen(line)>0){
			value = get_left_margin(line, value);
			width = get_width(line, width);
			/* non empty line, so process it */
			line_len = process_line(line ,line_len, value , width, heading_num);
			printf(" ");
			line_len = line_len + 1; 
		}
	}
	printf("\n");
	print_prompt();
	
	return 0;
}

/* Process the text file line by line
*/
int
process_line(char *line, int line_len, int value, int width, int *heading_num) {

	if(line[0] == '.'){
		if(line[1] == 'b'){
			/*.b – break the current line, so that the next input token 
			starts at the beginning of the next line;
			*/
			printf("\n");
			left_margin(value);
			line_len = 0;
		} else if(line[1] == 'p'){
			/* .p – leave a blank line and start a new paragraph without 
			altering the margins, with the next input token starting the 
			new paragraph;
			*/
			printf("\n");
			printf("\n");
			left_margin(value);
			line_len = 0;
		} else if(line[1] == 'l'){
			/* .l nn – alter the left margin from its current value
			(default initial value of 4) to the new value nn,
			and start a new paragraph;
			*/
			printf("\n");
			printf("\n");
			left_margin(value);
			line_len = 0;
		} else if(line[1] == 'c'){
			/* take the remaining contents of this line and center them within 
			the current output width.
			*/
			printf("\n");
			center_margin(line, width);
			line_len = 0;
		} else if(line[1] == 'h'){
			/* .h nn – take the remainder of the contents of this line 
			and use them as a section heading at the level indicated by nn.
			*/
			printf("\n");
			get_header(line, width, heading_num, value);
			line_len= 0;
		}
	} else {
		line_len = getword(line, line_len, value, width);
		// printf("%s",line);
	}
	return line_len;
}

/* Get the header value (aka nn) and print the  
Section/subsection/subsubsection number
*/
int 
get_header(char *line, int width, int *heading_num, int value){
	char num[MAX_HEADING_LEVEL + 1];
	int i;
	int counter = 0;
	int number;	
	
	// Get the nn which is at location 3 and 4
	for(i = 3; i <= 4; i++){
		num[counter++] = line[i];
	}
	num[counter] = '\0';
	number = atoi(num);
	*heading_num = show_header(number, width, heading_num, value);
	// Get the character which starts from location 5
	for(i = 5; i < strlen(line); i++){
		printf("%c",line[i]);	
	}
	printf("\n");
	return 0;
}

/* Reinitialize the heading numbers 
(i.e  Section/subsection/subsubsection number) as it is called
*/
int 
new_header(int number, int *heading_num){
	int i;
	for(i = number + 1; i < 6; i++){
		heading_num[i] = 0;	
	}
	heading_num[0] = 1;
	return 0;
}

/* At most five levels of headings may occur, counting from nn = 1 
(the top-level heading) to nn = 5.
*/
int 
show_header(int number, int width, int *heading_num, int value){
	int i;
	
	// Exit if section/subsection/subsubsection number is more than 5
	if(number > MAX_HEADING_LEVEL){
		return 0;	
	} else{ //Print the section/subsection/subsubsection number
		if(number == 1){
			new_header(number, heading_num);
			heading_num[1]++;
			left_margin(value);
			for(i = 0; i < width - value; i++){
				printf("-");	
			}
			printf("\n");
			// plus left_margin
			left_margin(value);
			printf("%d ",heading_num[1]);
		}else if (number == 2){
			new_header(number, heading_num);
			heading_num[2]++;
			left_margin(value);
			printf("%d.%d ",heading_num[1],heading_num[2]);
		}else if (number == 3){
			new_header(number, heading_num);
			heading_num[3]++;
			left_margin(value);
			printf("%d.%d.%d ",heading_num[1], heading_num[2],heading_num[3]);
		}else if (number == 4){
			new_header(number, heading_num);
			heading_num[4]++;
			left_margin(value);
			printf("%d.%d.%d.%d ",heading_num[1], heading_num[2],
				heading_num[3],heading_num[4]);
		}else if (number == 5){
			new_header(number, heading_num);
			heading_num[5]++;
			left_margin(value);
			printf("%d.%d.%d.%d.%d ",heading_num[1], heading_num[2],
				heading_num[3],heading_num[4],heading_num[5]);
		}
	}
	
	return 0;
}

/* .c – take the remaining contents of this line and center them 
within the current output width. If the remaining contents cannot fit within 
the current output width, then it should be placed so that it starts at the 
left margin and overflows beyond the current width. When there is an odd number
of spaces to be assigned, the rounded-down half should be at the beginning of 
the line, and the rounded-up half at the end of the line.

*/
int 
center_margin(char *line, int width){
	int len_string;
	int left_space;
	int i;
	int len_newline;
	char newline[LINELEN];
	int counter;
	
	if (strlen(line) > 2){
		// Eliminate ".c" from the string
		counter = 0;
		len_string = strlen(line);
		for(i = 3;i < len_string; i++){
			newline[counter++] = line[i];
		}
		newline[counter] = '\0';
	
		len_newline = strlen(newline);
	
		left_space = (width - len_newline) / 2;
	
		for(i = 0; i <left_space; i++){
			printf(" ");
		}
	
		printf("%s",newline);
		printf("\n");
	} else{
		return 0;	
	}
	
	return 0;	
}

/*.w nn – alter the width of each line from its current value 
(default initial value of 50) to the new value nn, and start a new paragraph.
*/
int 
get_width(char *line, int width){
	int i;
	char num[2];
	int num_len = 0;
	
	if(line[0] == '.' && line[1] == 'w'){
		for(i = 3; i <= 4; i++){
			num[num_len] = line[i];
			num_len++;
		}
		num[num_len] = '\0';
		width = atoi(num);
	} 
	return width;
}

/* Get the number of spaces for the left margin
*/
int 
get_left_margin(char *line , int value){
	
	int i;
	char num[2];
	int num_len = 0;
	
	if(line[0] == '.' && line[1] == 'l'){
		for(i = 3; i <= 4; i++){
			num[num_len] = line[i];
			num_len++;
		}
		num[num_len] = '\0';
		value = atoi(num);
	} 
	return value;
}


/* print the left margin spaces indicating ready for input
*/

int 
left_margin(int value){
	int i = 0;
	for(i = 0; i < value; i++){
		printf(" ");	
	}
	
	return 0;
}

/* Extract a single word out of the array, of not more than limit characters.
*/
int 
getword(char *line, int line_len, int value , int width){
	
	int len = 0;
	// Loop till the end of the line
	while(len < strlen(line)){
		if(line_len > width){
			printf("\n");
			left_margin(value);
			printf("%c",line[len]);
			line_len = 1;
		} else {
			printf("%c",line[len]);
			line_len = line_len + 1;	
		}
		len++;
	}
	//line_len = line_len + len;
	return line_len; 
}

/* read text input from stdin (you may assume that input lines
will be at most 999 characters long);
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c= mygetchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		} else if( i < maxlen && isspace(c)){
			c = remove_whitespace();
			line[i++] = ' ';
			line[i++] = c;
		}
	}
	line[i] = '\0';
	return ((i>0) || (c!=EOF));
}

/* Get the characters
 Reference: https://people.eng.unimelb.edu.au/ammoffat/teaching/10002/ass1/
*/
int
mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}

/* replace all instances of (multiple) whitespace characters 
(blanks, tabs, newlines) by single blanks;
*/

int
remove_whitespace(){
	int next;
	next = mygetchar();
	
	if((next == ' ' || next == '\t' || next == '\n' )&& next != EOF){
		return remove_whitespace();
	}
	return next;
}

/* prints the prompt indicating ready for input
 */
void
print_prompt(void) {
	printf("0----5---10---15---20---25---30---35---40---45---50---55---60 \n");
}

// ALGORITHMS ARE FUN!!!
