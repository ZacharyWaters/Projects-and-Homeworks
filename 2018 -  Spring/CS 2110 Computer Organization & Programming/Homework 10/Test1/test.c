#include "circ_list.h"
#include "array_list.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Here we are going to write some functions to support a list that stores
 * person data (name, age)
 */
typedef struct person {
    char *name;
    int age;
} Person;

Person* create_person(char* str, int age) {
	Person* person = malloc(sizeof(Person*));
	if (person == NULL) {
		return NULL;
	}
	person->name = malloc(strlen(str) + 1);
	if (person->name == NULL) {
		free(person);
		return NULL;
	}
	strcpy(person->name, str);
	person->age = age;
	return person;
}

void free_person(void* a) {
	Person* p = a;
    free(p->name);
    free(p);
}

int main() {
    //printf("NO TESTS ARE PROVIDED. YOU MUST WRITE YOUR OWN\n");
    // Person person2 = {"Brooke White", 19};
    // Person person3 = {"Katie Cox", 20};
    // Person* person = create_person("Harsh Jain", 20);
    // Person* person2 = create_person("Brooke White", 19);
    // Person* person3 = create_person("Katie Cox", 20);
    int* num = malloc(sizeof(int));
    *num = 1;
    int* num2 = malloc(sizeof(int));
    *num2 = 2;
    int* num3 = malloc(sizeof(int));
    *num3 = 3;
    array_list_t* list = create_array_list();
    add_to_array_list(list, 0, num);
    //printf(list);
    add_to_array_list(list, 3, num2);
    // //printf(list);
    add_to_array_list(list, 12, num3);
    // //printf(list);
    array_list_t* list2 = shallow_copy_array_list(list);
    // //printf(list2);
    trim_to_size(list);
    // //printf(list);
    // remove_from_array_list(list, 0, double1);
    // // //printf(list);
    // remove_from_array_list(list, 3, double2);
    // // //printf(list);
    // remove_from_array_list(list, 12, double3);
    // //printf(list);
    free(num);
    free(num2);
    free(num3);
    free(list2->entries);
    free(list2);
    destroy_array_list(list, free);
    // free(list->entries);
    // free(list);
    //destroy_array_list(list2, free_person);

    return 0;
}