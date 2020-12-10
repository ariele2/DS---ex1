#ifndef COURSES_MANAGER_H
#define COURSES_MANAGER_H
#include <stdlib.h>
#include "d_linked_list.h"
#include "AVL_v2.h"

class CoursesManager {
    AVL_v2_t<node<AVL_v2_t<AVL_v2_t<int>*>*>**>* coursesData; //avl tree that each node contain array of pointers to nodes, that contains avl of avl trees.
    d_linked_list<AVL_v2_t<AVL_v2_t<int>*>*>* timeList; //linked list that wach node of it contains avl inside avl.
    node<AVL_v2_t<AVL_v2_t<int>*>*>* max_time_cell; //pointer to the max time cell in the linked list.
    public:
    CoursesManager (); //constructor
    ~CoursesManager(); //destructor
    void AddCourse(int courseID, int numOfClasses);
    void RemoveCourse(int courseID);
    void WatchClass(int courseID, int classID, int time);
    void TimeViewed(int courseID, int classID, int *timeViewed);
    void GetMostViewedClasses(int numOfClasses, int *courses, int *classes);
    class AllocationError{};
    class InvalidInput{};
    class Failure{};
};

CoursesManager::CoursesManager(){
    coursesData = new AVL_v2_t<node<AVL_v2_t<AVL_v2_t<int>*>*>**>();
    if (!coursesData) {
        throw AllocationError();
    }
    timeList = new d_linked_list<AVL_v2_t<AVL_v2_t<int>*>*>();
    if (!timeList) {
        throw AllocationError();
    }
    timeList->add_head(0); //makes a 0 cell in the time list
    timeList->get_head()->data = new AVL_v2_t<AVL_v2_t<int>*>();
    if (!timeList->get_head()->data) {
        throw AllocationError();
    }
    max_time_cell = timeList->get_head(); //sets max to the head
}

CoursesManager::~CoursesManager() {
    auto curr_cell = timeList->get_head();
    while (curr_cell) {
         for (auto it_coursesList = curr_cell->data->begin(); it_coursesList != curr_cell->data->end(); ++it_coursesList) { //clean inner tree from trees
            delete (*it_coursesList).data;
         }
         delete curr_cell->data; //free the tree in the list
         curr_cell = curr_cell->next;
    }
    for (auto it_coursesData = coursesData->begin(); it_coursesData != coursesData->end(); ++it_coursesData) { //clean the arrays coursesData 
        delete[] (*it_coursesData).data;
    }
    delete timeList;
    delete coursesData;
}

void CoursesManager::AddCourse(int courseID, int numOfClasses) {
    if (coursesData->getNode(courseID)) {
        throw Failure();
    }
    auto init_ptr = timeList->get_head(); 
    //coursesData update
    node<AVL_v2_t<AVL_v2_t<int>*>*>** classes_tree = new node<AVL_v2_t<AVL_v2_t<int>*>*>*[numOfClasses]; //dont forget to remove them at the destructor!!
    if (!classes_tree) {
        throw AllocationError();
    }
    for (int i=0; i<numOfClasses; i++) {
        classes_tree[i] = init_ptr; //sets all the classses to point to the head of the list (cell 0)
    }
    coursesData->insert(courseID, classes_tree);
    //timeList update
    AVL_v2_t<int>* classes_list = new AVL_v2_t<int>(); //dont forget to remove them at the destructor!!
    if (!classes_list) {
        throw AllocationError();
    }
    classes_list->makeTree(numOfClasses, numOfClasses);
    (init_ptr->data)->insert(courseID, classes_list); 
}

void CoursesManager::RemoveCourse(int courseID) {
    auto to_remove = coursesData->getNode(courseID);
    if (!to_remove) {
        throw Failure();
    }
    //the value of numOfClasses is inside the pointer (to zero)-> linked list-> courses tree-> classes tree ->data (of zero).
    int numOfClasses = ((to_remove->data[0]->data)->getNode(courseID)->data)->getNode(0)->data;
    for (int i=0; i<numOfClasses; i++) {
        auto curr_ptr = to_remove->data[i];
        if (!curr_ptr) { //if the ptr points to null
            continue;
        }
        else { //its not null
            auto courseNode_in_list = curr_ptr->data->getNode(courseID);
            //iterate over the nodes (the classes) inside the course
            for (auto it = courseNode_in_list->data->begin(); it != courseNode_in_list->data->end(); ++it) {
                to_remove->data[(*it).id] = nullptr; //updates the cell corresponds to the id inside the array to be null
            }
            delete curr_ptr->data->getNode(courseID)->data; //delete the space allocated for the tree inside the tree
            curr_ptr->data->remove(courseID); //remove the course from the cell of the list that contains the tree
            if (curr_ptr->data->isEmpty()) { //checks if the cell is now empty
                //now its time to check if we removed the max time cell
                if (max_time_cell == curr_ptr && max_time_cell != timeList->get_head()) {
                    max_time_cell = curr_ptr->prev; //moves max time cell to the previous node of the list
                }
                // now we should remove the cell from the list, cause its empty
                if (curr_ptr != timeList->get_head()) { //we dont want to remove cell 0.
                    delete curr_ptr->data;
                    timeList->remove(curr_ptr);
                }
            }
        }
    }
    //now we can remove the node from coursesData as well...
    delete[] to_remove->data;
    coursesData->remove(courseID);
}

void CoursesManager::WatchClass(int courseID, int classID, int time) {
    if (!coursesData->getNode(courseID)) {
        throw Failure();
    }
    auto curr_class_time = coursesData->getNode(courseID)->data[classID];
    int numOfClasses = ((curr_class_time->data)->getNode(courseID)->data)->getNode(classID)->data;
    if (numOfClasses < classID+1) {
        throw InvalidInput();
    }
    int curr_time = curr_class_time->id;
    //go to the tree, find the course node, then go to the tree inside and remove classID from it
    curr_class_time->data->getNode(courseID)->data->remove(classID); 
    if (curr_class_time->data->getNode(courseID)->data->isEmpty()) { //after we removed the node, the tree is empty
        delete curr_class_time->data->getNode(courseID)->data; //free the allocated space
        curr_class_time->data->remove(courseID); //remove the course from the tree
        if(curr_class_time->data->isEmpty() && curr_class_time != timeList->get_head()) { //in case the tree inside the list is empty
            if (max_time_cell == curr_class_time) {
                //we just updating the current node in the list 
                curr_class_time->id += time;
                AVL_v2_t<int>* new_max_tree = new AVL_v2_t<int>();
                if (!new_max_tree) {
                    throw AllocationError();
                }
                new_max_tree->insert(classID, numOfClasses);
                curr_class_time->data->insert(courseID, new_max_tree);
            }
            else {
                auto temp = curr_class_time->prev;
                timeList->remove(curr_class_time);
                curr_class_time = temp;
            }
        }
    }
    bool needs_update = true;
    while (curr_class_time && needs_update) { //reaches one time ahead.
        if (!curr_class_time->next || curr_class_time->next->id > time) { //we stopped cause it doesnt exist
            AVL_v2_t<AVL_v2_t<int>*> *new_courses_tree = new AVL_v2_t<AVL_v2_t<int>*>();
            if (!new_courses_tree) {
                throw AllocationError();
            }
            AVL_v2_t<int>* new_classes_tree = new AVL_v2_t<int>();
            if (!new_classes_tree) {
                throw AllocationError();
            }
            new_classes_tree->insert(classID, numOfClasses);
            new_courses_tree->insert(courseID, new_classes_tree);
            timeList->add_after(curr_time + time, new_courses_tree, curr_class_time);
            if (!curr_class_time->next->next) {
                max_time_cell = curr_class_time->next;
            }
            needs_update = false;
        }
        else if (curr_class_time->next->id == time) { //if there is a cell with this time
            auto courses_tree = curr_class_time->next->data->getNode(courseID);
            if (!courses_tree) { //there is no such tree for the course yet
                AVL_v2_t<int>* new_classes_tree = new AVL_v2_t<int>();
                if (!new_classes_tree) {
                    throw AllocationError();
                }
                new_classes_tree->insert(classID, numOfClasses);
                curr_class_time->next->data->insert(courseID, new_classes_tree);
            }
            else { //the tree exists
                courses_tree->data->insert(classID, numOfClasses);
            }
            needs_update = false;
        }
        curr_class_time = curr_class_time->next; //keep looking
    }
    coursesData->getNode(courseID)->data[classID] = curr_class_time;
}

void CoursesManager::TimeViewed(int courseID, int classID, int* timeViewed) {
    if (!coursesData->getNode(courseID)) {
        throw Failure();
    }
    auto curr_class_time = coursesData->getNode(courseID)->data[classID];
    int numOfClasses = ((curr_class_time->data)->getNode(courseID)->data)->getNode(classID)->data;
    if (numOfClasses < classID+1) {
        throw InvalidInput();
    }
    *timeViewed = curr_class_time->id;
}

void CoursesManager::GetMostViewedClasses(int numOfClasses, int *courses, int *classes) {
    if (numOfClasses <= 0) {
        throw InvalidInput();
    }
    int i=0;
    auto curr_cell = max_time_cell;
    while (curr_cell) {
        for (auto it_courses = curr_cell->data->begin(); it_courses != curr_cell->data->end(); ++it_courses) {
            for (auto it_classes = (*it_courses).data->begin(); it_classes != (*it_courses).data->end(); ++it_classes) {
                courses[i] = (*it_courses).id;
                classes[i] = (*it_classes).id;
                i++;
                if (i == numOfClasses) {
                    return;
                }
            }
        }
        curr_cell = curr_cell->prev;
    }
    if (i+1 < numOfClasses) { 
        throw Failure();
    }
}

#endif //COURSES_MANAGER_H