#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "grades.h"
#include "linked-list.h"

// Declaring on our needed structs for this assignment according to the PDF.

struct student_info {
	char *student_name;
	int id;
	int courses_counter;
	struct course *courses_list; 
};

struct course {
	char *course_name;
	int course_grade;
};

struct grades {
	struct list *students_list;
};

// DECLARATION OF FUNCTIONS 

struct student_info* student_init(const char *name_of_student, int student_id);
struct course* course_init(const char *course_name, int grade_of_course);
void course_clone(struct course *course, struct course *destination);


// This function destroys created student struct
void destroy_student(void *elem) {
	
	struct student_info *student_destruction = (struct student_info*) elem;
	if (student_destruction == NULL) { 
		return; //As it's already invalid
	}
	for (int counter = 0; counter < (student_destruction->courses_counter); counter++) {
		free(student_destruction->courses_list[counter].course_name);
	}
	free(student_destruction->courses_list);
	free(student_destruction->student_name);
	free(student_destruction);
}

// This function destroys created grades struct
void grades_destroy(struct grades *grades) {
	
	list_destroy(grades->students_list);
	free(grades);

}


/* This function initializes the student struct according to the given information and if the initialization
  was successfull it returns a pointer and if it was not, it returns null */ 

struct student_info* student_init(const char *name_of_student, int student_id) {

	struct student_info *initial_student = (struct student_info*) malloc(sizeof(struct student_info));
	if (initial_student == NULL) {
		return NULL;
	}
	initial_student->student_name = (char*) malloc(sizeof(char)*(strlen(name_of_student) + 1));
	if (initial_student->student_name == NULL) {
		free(initial_student);
		return NULL;
	}
	strcpy(initial_student->student_name, name_of_student);
	struct course *student_courses = (struct course*) malloc(sizeof(struct course));
	if (student_courses == NULL) {
		free(initial_student->student_name);
		free(initial_student);
		return NULL;
	}
	initial_student->id = student_id;
	initial_student->courses_list = student_courses;
	initial_student->courses_counter = 0;
	return initial_student;
}


// This function clones an already existing struct into a new struct (it copies)
void course_clone(struct course *course, struct course *destination) {

	struct course *tmp_course = course;
	struct course *clone_course = destination;
	if (clone_course == NULL) {
		return;
	}
	clone_course->course_name = (char*) malloc(sizeof(char) * (strlen(tmp_course->course_name) + 1));
	if (clone_course->course_name == NULL) {
		return;
	}
	clone_course->course_grade = tmp_course->course_grade;
	strcpy(clone_course->course_name, tmp_course->course_name);
}

/* This function basically clones the given element to the output for student struct */
int student_clone(void *elem, void **output) {

	struct student_info *tmp = (struct student_info*) elem;
	if (tmp == NULL) {
		return -1;
	}
	struct student_info *clone_student = (struct student_info*) malloc(sizeof(struct student_info));
	if (clone_student == NULL) {
		return -2; 
	}
	clone_student->student_name = (char*) malloc(strlen(tmp->student_name) + 1 * sizeof(char));
	if (clone_student->student_name == NULL) {
		free(clone_student);
		return -3;
	}
	clone_student->courses_counter = tmp->courses_counter;
	clone_student->courses_list = (struct course*) malloc((clone_student->courses_counter)*sizeof(struct course));
	for (int count = 0; count < tmp->courses_counter; count++) {
		course_clone(&tmp->courses_list[count], &clone_student->courses_list[count]);
	}
	clone_student->id = tmp->id;
	strcpy(clone_student->student_name,tmp->student_name);
	*output = clone_student;

	return 0;
}

/* Initializing the structs named ''course'' and ''grade'' and in a similar way to the ''student_init'' it also returns a pointer 
	or NULL according to the progess. */

struct course* course_init(const char *course_name, int grade_of_course) {
	struct course *initial_course = (struct course*) malloc(sizeof(initial_course));
	if (initial_course == NULL) {
		return NULL;
	}
	initial_course->course_name = (char*) malloc(sizeof(char)*(strlen(course_name) + 1));
	if (initial_course->course_name == NULL) {
		free(initial_course);
		return NULL;
	}
	initial_course->course_grade = grade_of_course;
	strcpy(initial_course->course_name, course_name);
	return initial_course;
}


struct grades* grades_init() { 

	struct grades *grades_tmp;
	grades_tmp = (struct grades*) malloc(sizeof(struct grades));
	if (grades_tmp == NULL) {
		return NULL;
	}
	grades_tmp->students_list = list_init(student_clone, destroy_student);
	return grades_tmp;
}
/* Adds a new student with given info (His name and his ID). It failes or success according to it's progression 
 if there were errors like invalid info or already existing student it would fail. */

int grades_add_student(struct grades *grades, const char *name, int id) {

	if (grades == NULL) {
		return -1;
	}
	int size_of_list = list_size(grades->students_list);
	struct node *iterator = list_begin(grades->students_list);
	struct student_info *student = (struct student_info*)list_get(iterator);
	for(int count=0;count < size_of_list;count++) { 
			if (student->id == id) {
				return -2;
			}
			iterator = list_next(iterator);
			student = list_get(iterator);
	}

	struct student_info *helper = student_init(name, id);
	int checker = list_push_back(grades->students_list, helper);
	if ( checker == 0) {
		return 0;
	}
	return -3;
}
// This function adds a new course with an already given information, and it returns it's result (failed/succcess) according to it's progression.
int grades_add_grade(struct grades *grades,
					 const char *name,
					 int id,
					 int grade) {
	if (grades == NULL) {
		return -1;
	}
	int size_of_list = list_size(grades->students_list);
	struct node *iterator = list_begin(grades->students_list);
	struct student_info *student = list_get(iterator);

	if ((grade < 0 || grade > 100)) //INVALID GRADE (INPUT)
	{
		return -1;
	}

	for (int count=0;count<size_of_list;count++) {
		if (student->id == id) {
			for (int sec_count = 0; sec_count< (student->courses_counter) ; sec_count++) {
				if (!(strcmp(student->courses_list[sec_count].course_name,name))) {
					return -2;
				}
				
			}
			student->courses_list = (struct course*) realloc(student->courses_list, (student->courses_counter + 1) * sizeof(struct course)); // As we want to reallocate.
			if (student->courses_list == NULL) {
				return -3;
			}
			struct course *helper = course_init(name, grade);
			course_clone(helper,&student->courses_list[student->courses_counter]);
			student->courses_counter++;
			free(helper->course_name);
			free(helper);

		}
		iterator = list_next(iterator);
		student = list_get(iterator);
	}
	return 0;
}
/* This function basically calculates the student's average and again it depends on it's progression, whether the calculations
 were made correctly, or no errors were made during the process and it's returning value will be according to it. */
 
float grades_calc_avg(struct grades *grades, int id, char **out) {
	if (grades == NULL) {
		out = NULL;
		return -1;
	}
	int check_if_found_right_id = -1;
	float average_of_student = 0;
	int sum_of_student=0;
	int size_of_list = list_size(grades->students_list);
	struct node *iterator = list_begin(grades->students_list);
	struct student_info *student = list_get(iterator);
	
	for(int count=0;count<size_of_list;count++) {
		if (student->id == id) {
			*out = (char*)malloc(sizeof(char)*(strlen(student->student_name)+1));
			if(*out == NULL)
					return -2; 
			if (student->courses_counter == 0) {
				strcpy(*out, student->student_name);
				return average_of_student;
			}
			for (int sec_count = 0;sec_count< student->courses_counter;sec_count++) {
				sum_of_student = sum_of_student + (student->courses_list[sec_count].course_grade);
			}
				strcpy(*out, student->student_name);
				average_of_student = ((float) sum_of_student) / (student->courses_counter);
				check_if_found_right_id = 0;
		}
		iterator = list_next(iterator);
		student = list_get(iterator);
	}
	if (check_if_found_right_id == 0) 
		return average_of_student;
	
	return check_if_found_right_id;
}

// This function prints all students' grades  in a consistent format and in a certain order (as it was filled in).

int grades_print_all(struct grades *grades) {

	if (grades == NULL) {
		return -101;
	}
	struct node *iterator = list_begin(grades->students_list);
	struct student_info *student = list_get(iterator);
	for(int count = 0;count < list_size(grades->students_list);count++) {
		int helper_id = student->id;
		grades_print_student(grades, helper_id);
		iterator = list_next(iterator);
		student = list_get(iterator);
	}
	return 0;
}


// This function prints the students' courses with theirs grades.

int grades_print_student(struct grades *grades, int id) {

	if (grades == NULL) {
		return -1;
	}
	struct node *iterator = list_begin(grades->students_list);
	struct student_info *student = list_get(iterator);
	while (student != NULL) {
		if (student->id == id) {
			if(student->courses_counter == 0){
				printf("%s %d:\n", student->student_name, student->id);
				return 0;
			} 
			else {
				printf("%s %d: ", student->student_name, student->id);
			}
			for (int counter = 0; counter < student->courses_counter; counter++) {
				printf("%s %d", student->courses_list[counter].course_name, student->courses_list[counter].course_grade);
				if (counter != student->courses_counter - 1) {
					printf(", ");
				} 
				else
					printf("\n");
			}
		}
		iterator = list_next(iterator);
		student = list_get(iterator);
	}
	return 0;
}

