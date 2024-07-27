#include "TaskManager.h"
#include "Task.h"
#include "SortedList.h"
#include "Person.h"

TaskManager::TaskManager() : numOfPersons(0), numOfTasks(0) {}

void TaskManager::assignTask(const std::string &personName, const Task &task) {

    Task tempTask = task;
    tempTask.setId(numOfTasks++);
    for (int i = 0; i < numOfPersons; i++) {
        if (personName == persons[i].getName()) {
            persons[i].assignTask(tempTask);
            return;
        }
    }

    if (numOfPersons >= 10) {
        throw std::runtime_error("too many persons in Task-Manager");
    }
    Person temp(personName);
    SortedList<Task> newTask;
    newTask.insert(tempTask);
    temp.setTasks(newTask);
    persons[numOfPersons] = temp;
    numOfPersons++;
}

void TaskManager::completeTask(const std::string &personName) {
    for (int i = 0; i < numOfPersons; i++) {
        if (personName == persons[i].getName()) {
            persons[i].completeTask();
            return;
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    if (priority <= 0) {
        return;
    }
    for (int i = 0; i < numOfPersons; i++) {
        //creating a new applied SortedList and using it to set tasks for each person
        SortedList<Task> temp;
        temp = persons[i].getTasks().apply(
                [&](Task &task) -> Task {
                    if (task.getType() == type) {
                        Task temp(task.getPriority() + priority, type,
                                  task.getDescription());
                        temp.setId(task.getId());
                        return temp;
                    }
                    return task;
                });
        persons[i].setTasks(temp);
    }
}

void TaskManager::printAllEmployees() const {
    for (int i = 0; i < numOfPersons; i++) {
        std::cout << persons[i] << std::endl;
    }
}

void TaskManager::printAllTasks() const {
    //printing all tasks from highest to lowest priority
    for (int p = minMaxPriority(maxIndex); p >= minMaxPriority(minIndex); p--) {
        for (int i = 0; i < numOfPersons; i++) {
            SortedList<Task> tasks = persons[i].getTasks().filter(
                    [&](Task &task) -> bool {
                        return (task.getPriority() == p);
                    });
            for (int j = 0; j < tasks.length(); j++) {
                std::cout << tasks[j] << std::endl;
            }
        }
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    //creating a temporary filtered taskManager and printing all tasks of it
    TaskManager temp;
    temp.numOfPersons = numOfPersons;
    for (int i = 0; i < numOfPersons; i++) {
        temp.persons[i].setTasks(persons[i].getTasks().filter(
                [&](Task &task) -> bool {
                    return (task.getType() == type);
                }));
    }
    temp.printAllTasks();
}

int TaskManager::minMaxPriority(int lookingFor) const {
    int res = lookingFor == maxIndex ? 0 : minMaxPriority(maxIndex);
    for (int i = 0; i < numOfPersons; i++) {
        for (int j = 0; j < persons[i].getTasks().length(); j++) {
            int temp = persons[i].getTasks()[j].getPriority();
            if (lookingFor == maxIndex) {
                res = temp > res ? temp : res;
            } else if (lookingFor == minIndex) {
                res = temp < res ? temp : res;
            }
        }
    }
    return res;
}