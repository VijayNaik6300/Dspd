
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct person{
    int id;
    char name[20];
    int age;
    int goal;
    int steps[7];
    struct person *next;
}person;

typedef struct group{
    int group_id;
    char group_name[50];
    person *members[5];
    int size;
    int ggoal;
    struct group* next;
    int sum;
}group;
person* person_head = NULL;
group* group_head = NULL;


person *create_person(int id, char name[], int age, int goal, int steps[]) {
    person *new = malloc(sizeof(struct person));
    new->id = id;
    strcpy(new->name, name);  
    new->age = age;
    new->goal = goal;  
    for (int i = 0; i < 7; i++) { 
        new->steps[i] = steps[i];
    }
    new->next = NULL;   
    return new;  
}
group *create_group(int id, char *name, int goal, int *mem) {
    group *new_group = (group *)malloc(sizeof(group));
    new_group->group_id = id;
    strcpy(new_group->group_name, name);
    new_group->ggoal = goal;
    new_group->next = NULL;

    int k = 0;
    person *pnode = person_head;

    while (pnode != NULL) {
        int flag = 0;
        for (int i = 0; i < 5; i++) {
            if (pnode->id == mem[i] && flag == 0) {
                new_group->members[k] = pnode;
                k++;
                flag = 1;  // Exit the loop once a member is found for the current person
            }else{
                new_group->members[k]=NULL;
            
            }
        }
        pnode = pnode->next;
    }

    new_group->size = k;

    return new_group;
}
void add_person(person **person_head, person *pnode) {
    if (*person_head == NULL || (*person_head)->id > pnode->id) {
        pnode->next = *person_head;
        *person_head = pnode;
    } else {
        person *current = *person_head;
        while (current->next != NULL && current->next->id < pnode->id) {
            current = current->next;
        }
        pnode->next = current->next;
        current->next = pnode;
    }
}
void insert_group(struct group **group_head, struct group *gnode) {
    if (*group_head == NULL || (*group_head)->group_id > gnode->group_id) {
        gnode->next = *group_head;
        *group_head = gnode;
    } else {
        struct group *curr = *group_head;
        while (curr->next != NULL && curr->next->group_id < gnode->group_id) {
            curr = curr->next;
        }
        gnode->next = curr->next;
        curr->next = gnode;
    }
}

// Add an existing person to a group
void add_person_to_group(person *person_head, group *group_head, int pid, int group_id) {
    person *pnode = person_head;
    int foundPerson = 0;
    while (pnode != NULL) {
        if (pnode->id == pid) {
            foundPerson = 1;
            break;
        }
        pnode = pnode->next;
    }
    if (!foundPerson) {
        printf("Person with ID %d does not exist.\n", pid);
        return;
    }

    group *curr = group_head;
    int foundGroup = 0;
    while (curr != NULL) {
        int foundMember = 0;
        for (int i = 0; i < curr->size; i++) {
            if (curr->members[i]->id == pid) {
                printf("Person with ID %d already belongs to group %s.\n", pid, curr->group_name
        );
                foundMember = 1;
                break;
            }
        }
        if (foundMember) {
            foundGroup = 1;
            break;
        }
        curr = curr->next;
    }
    if (foundGroup) {
        return;
    }

    group *gnode = group_head;
    while (gnode != NULL && gnode->group_id != group_id) {
        gnode = gnode->next;
    }
    if (gnode == NULL) {
        printf("Group with ID %d does not exist.\n", group_id);
        return;
    }
    if (gnode->size == 5) {
        printf("Group %s is full.\n", gnode->group_name
);
        return;
    }
    gnode->members[gnode->size] = pnode;
    gnode->size++;
    printf("Person %s added to group %s.\n", pnode->name, gnode->group_name);
}
person* top[3] = {NULL, NULL, NULL}; // Array to store the top 3 nodes
    int total[3] = {0, 0, 0};
void get_top_3(person* person_head) {
     // Array to store the total steps of the top 3 nodes
    person* curr = person_head; // Start from the next of the person list
 
    while (curr->next != NULL) { // Traverse the list
        int sum = 0; // Variable to store the sum of steps for the curr node
        int flag=0;
        for (int i = 0; i < 7; i++) { // Loop through the steps array
            sum += curr->steps[i]; // Add the steps to the sum
        }

        if (sum > curr->goal) {
            for (int i = 0; i < 3; i++) { // Loop through the top 3 arrays
                if (sum > total[i]&&flag==0) { // If the sum is greater than the total at the curr index
                    for (int j = 2; j > i; j--) { // Shift the elements to the right
                        top[j] = top[j - 1];
                        total[j] = total[j - 1];
                    }
                    top[i] = curr; // Insert the curr node at the curr index
                    total[i] = sum; // Insert the sum at the curr index
                    flag=1; // Exit the loop once insertion is done
                }
            }
        }

        curr = curr->next; // Move to the next node
    }

    printf("The top 3 persons are:\n");
    for (int i = 0; i < 3; i++) { // Loop through the top 3 arrays
        if (top[i] != NULL) { // If the node is not null
            printf("%d. %s with %d steps\n", i + 1, top[i]->name, total[i]); // Print the name and the steps
        }
    }
}

void check_group_achievement(int id1) {
    group *gnode = group_head; // Start from the next of the group list
    while (gnode != NULL && gnode->group_id != id1) { // Traverse the list until the group is found or the end is reached
        gnode = gnode->next;
    }
    if (gnode == NULL) { // If the group is not found
        printf("Group with ID %d does not exist.\n", id1);
        return;
    }
    int total_steps = 0;
    for (int i = 0; i < gnode->size; i++) {
        for (int j = 0; j < 7; j++) {
            total_steps += gnode->members[i]->steps[j];
        }
    }
    if (total_steps >= gnode->ggoal) {
        printf("Group %s has achieved its weekly goal of %d with %d steps.\n", gnode->group_name
, gnode->ggoal,total_steps);
    } else {
        printf("Group %s has not achieved its weekly goal of %d. Total steps: %d\n", gnode->group_name
,gnode->ggoal, total_steps);
    }
}
struct group *insert_sorted(struct group *group_head, struct group *node, int sum) {
    struct group *head = group_head;
    if (head == NULL || sum > head->sum) {
        node->next = head;
        return node;
    } else {
        struct group *curr = head;
        struct group *prev = NULL;
        while (curr != NULL && sum <= curr->sum) {
            prev = curr;
            curr = curr->next;
        }
        if (prev != NULL) {
            prev->next = node;
        } else {
            head = node;
        }
        node->next = curr;
        return head;
    }
}


// Sort the group list in descending order of the highest number of steps and display the group name with the number of steps based on rank
void generate_leader_board(struct group *group_head) {
    struct group *sorted = NULL;
    struct group *curr = group_head;
    while (curr!= NULL) {
        struct group *next = curr->next;
        int sum = 0;
        for (int i = 0; i < curr->size; i++) {
            for (int j = 0; j < 7; j++) {
                sum += curr->members[i]->steps[j];
            }
        }
        curr->sum=sum;
        curr->next = NULL;
        sorted = insert_sorted(sorted, curr, sum);
        curr = next;
    }
    group_head = sorted;
    printf("The leader board is:\n");
    int rank = 1;
    curr = group_head;
    while (curr->next!= NULL) {
        int sum = 0;
        for (int i = 0; i < curr->size; i++) {
            for (int j = 0; j < 7; j++) {
                sum += curr->members[i]->steps[j];
            }
        }
        printf("%d. %s with %d steps\n", rank, curr->group_name
, sum);
        rank++;
        curr = curr->next;
    }
}

void check_individual_rewards(struct person *person_head, int pid) {
    struct person *pnode = person_head; // Start from the head of the person list
    while (pnode != NULL && pnode->id != pid) { // Traverse the list until the person is found or the end is reached
        pnode = pnode->next;
    }
    if (pnode == NULL) { // If the person is not found
        printf("Person with ID %d does not exist.\n", pid);
        return;
    }
        int flag1 = 0; // Flag to indicate if the person has completed the daily goals
    for (int i = 0; i < 7; i++) { // Loop through the steps array
        if (pnode->steps[i] > pnode->goal&&flag1==0) { // If the steps are less than the goal
            flag1 = 1; // Set the flag to 0
                    }
    }
    if (flag1 == 0) { // If the person has not completed the daily goals
        printf("Person %s has not completed the daily goals.\n", pnode->name);
        return;
    }
    person* top[3] = {NULL, NULL, NULL}; // Array to store the top 3 nodes
    int total[3] = {0, 0, 0}; // Array to store the total steps of the top 3 nodes
    person* curr = person_head; // Start from the next of the person list
 
    while (curr->next != NULL) { // Traverse the list
        int sum = 0; // Variable to store the sum of steps for the curr node
        int flag=0;
        for (int i = 0; i < 7; i++) { // Loop through the steps array
            sum += curr->steps[i]; // Add the steps to the sum
        }

        if (sum > curr->goal) {
            for (int i = 0; i < 3; i++) { // Loop through the top 3 arrays
                if (sum > total[i]&&flag==0) { // If the sum is greater than the total at the curr index
                    for (int j = 2; j > i; j--) { // Shift the elements to the right
                        top[j] = top[j - 1];
                        total[j] = total[j - 1];
                    }
                    top[i] = curr; // Insert the curr node at the curr index
                    total[i] = sum; // Insert the sum at the curr index
                    flag=1; // Exit the loop once insertion is done
                }
            }
        }

        curr = curr->next; // Move to the next node
    }
    int rank = 0; // Variable to store the rank of the person
    int points = 0; // Variable to store the points of the person
    int flag2=0;
    for (int i = 0; i < 3; i++) { // Loop through the top 3 arrays
        if (top[i] == pnode&&flag2==0) { // If the person is found
            rank = i + 1; // Assign the rank
            if (rank == 1) { // If the rank is 1
                points = 100; // Assign 100 points
            } else if (rank == 2) { // If the rank is 2
                points = 75; // Assign 75 points
            } else if (rank == 3) { // If the rank is 3
                points = 50; // Assign 50 points
            }
            flag2=1; // Break the loop
        }
    }
    if (rank == 0) { // If the person is not in the top 3
        printf("Person %s completed the daily goals but not in the top 3 persons.\n", pnode->name);
    } else { // If the person is in the top 3
        printf("Person %s is in the rank %d and has earned %d points.\n", pnode->name, rank, points);
    }
}

void delete_person(person* person_head, int id) {
    person* temp = person_head, *prev = NULL;
    if (temp->next!= NULL && temp->id == id) {
        person_head = temp->next;
        free(temp);
        return;
    }
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    prev->next = temp->next;
    free(temp);
}


void delete_group(struct group *group_head, int group_id) {
    struct group *gnode = group_head; // Start from the head of the group list
    struct group *prev = NULL; // Pointer to the previous node
    while (gnode != NULL && gnode->group_id != group_id) { // Traverse the list until the group is found or the end is reached
        prev = gnode; // Update the previous node
        gnode = gnode->next; // Move to the next node
    }
    if (gnode == NULL) { // If the group is not found
        printf("group with ID %d does not exist.\n", group_id);
        return;
    }
    if (prev == NULL) { // If the group is the head of the list
        group_head = gnode->next; // Make the next node the head
    } else { // If the group is not the head of the list
        prev->next = gnode->next; // Link the previous node to the next node
    }
    group_head->size--; // Decrement the size
    
    free(gnode); // Free the memory of the group node
    gnode=NULL;
    printf("group with ID %d deleted.\n", group_id);
}
void merge_groups(struct group **group_head, struct person *person_head, int group_id1, int group_id2) {
    struct group *gnode1 = *group_head;
    struct group *prev1 = NULL;

    while (gnode1 != NULL && gnode1->group_id != group_id1) {
        prev1 = gnode1;
        gnode1 = gnode1->next;
    }

    if (gnode1 == NULL) {
        printf("group with ID %d does not exist.\n", group_id1);
        return;
    }

    struct group *gnode2 = *group_head;
    struct group *prev2 = NULL;

    while (gnode2 != NULL && gnode2->group_id != group_id2) {
        prev2 = gnode2;
        gnode2 = gnode2->next;
    }

    if (gnode2 == NULL) {
        printf("group with ID %d does not exist.\n", group_id2);
        return;
    }

    if (gnode1->size + gnode2->size > 5) {
        printf("The merged group will be too large.\n");
        return;
    }

    int new_id; // Generate a new ID for the merged group
    char new_name[20]; // Declare a new name for the merged group
    int new_goal; // Add the goals of the two groups
    printf("enter the new id for the new group:");
    scanf("%d", &new_id);
    printf("enter the new name for the new group:");
    scanf("%s", new_name);
    printf("enter the new goal for the new group:");
    scanf("%d", &new_goal);

    // Allocate memory for the new members array
    int new_mem[5] = {0, 0, 0, 0, 0};
      int k=0;
    // Copy members from the first group
    for (int i = 0; i < gnode1->size; i++) {
        new_mem[k] = gnode1->members[i]->id; // Copy member structures, not just pointers
        k++;
    }

    // Copy members from the second group
    for (int i = 0; i < gnode2->size; i++) {
        new_mem[k] = gnode2->members[i]->id;
        k++;
    }
    struct group *new_group = create_group(new_id, new_name, new_goal, new_mem);
    new_group->size=k;

    if (prev1 == NULL) {
        *group_head = gnode1->next;
    } else {
        prev1->next = gnode1->next;
    }

    if (prev2 == NULL) {
        *group_head = gnode2->next;
    } else {
        prev2->next = gnode2->next;
    }

    insert_group(group_head, new_group);
    printf("New group %s created by merging group %s and group %s.\n", new_group->group_name, gnode1->group_name, gnode2->group_name);
    printf("The groups with ids %d %d \nare deleted",gnode1->group_id,gnode2->group_id);
    free(gnode1);
    free(gnode2);
}



void display_group_info(struct group *group_head, int id1) {
    struct group *gnode = group_head; // Start from the head of the group list
    while (gnode != NULL && gnode->group_id != id1) { // Corrected loop condition
        gnode = gnode->next;
    }
    if (gnode == NULL) { // If the group is not found
        printf("Group with ID %d does not exist.\n", id1);
        return;
    }
    printf("Group name: %s\n", gnode->group_name); // Print the group name
    printf("Group ID: %d\n", gnode->group_id); // Print the group ID
    printf("Group goal: %d\n", gnode->ggoal); // Print the group goal
    printf("Group size: %d\n", gnode->size); // Print the group size
    printf("Group members:\n"); // Print the group members
    for (int i = 0; i < gnode->size; i++) { // Loop through the members array
        printf("%d. %s (ID: %d)\n", i + 1, gnode->members[i]->name, gnode->members[i]->id); // Print the name and ID of each member
    }
    int rank = 0; // Variable to store the rank of the group
    int sum = 0; // Variable to store the sum of steps for the group
    for (int i = 0; i < gnode->size; i++) { // Loop through the members array
        for (int j = 0; j < 7; j++) { // Loop through the steps array
            sum += gnode->members[i]->steps[j]; // Add the steps to the sum
        }
    }
    struct group *curr = group_head; // Start from the head of the group list
    while (curr != NULL) { // Traverse the list
        int total = 0; // Variable to store the total steps for the curr node
        for (int i = 0; i < curr->size; i++) { // Loop through the members array
            for (int j = 0; j < 7; j++) { // Loop through the steps array
                total += curr->members[i]->steps[j]; // Add the steps to the total
            }
        }
        if (total > sum) { // If the total is greater than the sum
            rank++; // Increment the rank
        }
        curr = curr->next; // Move to the next node
    }
    rank++; // Increment the rank
    printf("Group rank: %d\n\n", rank); // Print the group rank
}

// Suggest a daily goal update for an individual such that he/she can consistently appear in the top 3 persons
void suggest_goal_update(struct person *person_head, int pid) {
    struct person *pnode = person_head; // Start from the head of the person list
    while (pnode != NULL && pnode->id != pid) { // Traverse the list until the person is found or the end is reached
        pnode = pnode->next;
    }
    if (pnode == NULL) { // If the person is not found
        printf("Person with ID %d does not exist.\n", pid);    
        return;
    }
    int sum = 0; // Variable to store the sum of steps for the person
    int flag = 0; // Flag to indicate if the person has completed the daily goals
    for(int i = 0; i < 7; i++){ // Loop through the steps array
        sum += pnode->steps[i]; // Add the steps to the sum
        if (pnode->steps[i] > pnode->goal&& flag==0) { // If the steps are less than the goal
            flag = 1; // Set the flag to 0
        }
    }
    if(flag == 0){ // If the person has not completed the daily goals
        printf("Person %s has not completed the daily goals.\n", pnode->name);
        return;
    }
    int max = 0; // Variable to store the average steps among the top 3 persons
    for(int i=0;i<3;i++){
        max += total[i];
    }
     max=max/3;
    int new_goal = max + 1; // Calculate the new goal as one more than the minimum
    printf("The suggested daily goal update for person %s is %d.\n", pnode->name, new_goal); // Print the new goal
}

void read_data_from_file() {
    FILE *file = fopen("a.txt", "r");

    if (file == NULL) {
       printf("file is not opened");
    }

    int n, m;
    fscanf(file, "%d", &n);
    for (int i = 0; i < n; i++) {
        int id, age, goal, steps[7];
        char name[20];
        fscanf(file, "%d %s %d %d", &id, name, &age, &goal);
        for (int j = 0; j < 7; j++) {
            fscanf(file, "%d", &steps[j]);
        }
        person *pnode = create_person(id, name, age, goal, steps);
        if (person_head == NULL) {
            person_head = pnode;
        } else {
            pnode->next = person_head;
            person_head = pnode;
        }
    }

    fscanf(file, "%d", &m);
    for (int i = 0; i < m; i++) {
        int id, goal, members[5];
        char name[20];
        fscanf(file, "%d %s %d", &id, name, &goal);
        for (int j = 0; j < 5; j++) {
            fscanf(file, "%d", &members[j]);
        }
        group *gnode = create_group(id, name, goal, members);
    if (group_head == NULL) {
        group_head = gnode;
    } else {
        gnode->next = group_head;
        group_head = gnode;
    }
    
    }

    fclose(file);
}

int main() {
   person_head=malloc(sizeof(person));
   group_head=malloc(sizeof(group)); 

    if (person_head == NULL || group_head == NULL) {
        printf( "Memory allocation error\n");
        return 0;
    }
 
    group_head->size = 0;
    read_data_from_file(person_head, group_head); // Read data from file


    get_top_3(person_head); // Display the top 3 persons
   person* curr=person_head;
while(curr->next!=NULL){
    printf("%d %s\n",curr->id,curr->name);
    curr=curr->next;
}   
  suggest_goal_update(person_head, 17); // Suggest a goal update for person 102
    free(person_head);  // Don't forget to free allocated memory before exiting
    free(group_head);

    return 0;
}