#pragma once

#include "Task.h"
#include "SortedList.h"
#include "Person.h"

/**
 * @brief Class managing tasks assigned to multiple persons.
 */
class TaskManager {
private:
    /**
     * @brief Maximum number of persons the TaskManager can handle.
     */
    static const int MAX_PERSONS = 10;
    int numOfPersons;
    Person persons[10];
    int numOfTasks;
    const static int minIndex = 0;
    const static int maxIndex = 1;

    // Note - Additional private fields and methods can be added if needed.
    int minMaxPriority(int lookingFor) const;

public:
    /**
     * @brief Default constructor to create a TaskManager object.
     *
     * Note - you may add =default if needed.
     */
    TaskManager();

    /**
     * @brief Deleted copy constructor to prevent copying of TaskManager objects.
     */
    TaskManager(const TaskManager &other) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent assignment of TaskManager objects.
     */
    TaskManager &operator=(const TaskManager &other) = delete;

    /**
     * @brief Assigns a task to a person.
     *
     * @param personName The name of the person to whom the task will be assigned.
     * @param task The task to be assigned.
     */
    void assignTask(const std::string &personName, const Task &task);

    /**
     * @brief Completes the highest priority task assigned to a person.
     *
     * @param personName The name of the person who will complete the task.
     */
    void completeTask(const std::string &personName);

    /**
     * @brief Bumps the priority of all tasks of a specific type.
     *
     * @param type The type of tasks whose priority will be bumped.
     * @param priority The amount by which the priority will be increased.
     */
    void bumpPriorityByType(TaskType type, int priority);

    /**
     * @brief Prints all employees and their tasks.
     */
    void printAllEmployees() const;

    /**
     * @brief Prints all tasks of a specific type.
     *
     * @param type The type of tasks to be printed.
     */
    void printTasksByType(TaskType type) const;

    /**
     * @brief Prints all tasks assigned to all employees.
     */
    void printAllTasks() const;
};
