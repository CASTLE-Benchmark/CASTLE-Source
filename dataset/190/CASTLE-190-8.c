/*
============================================================================
dataset: CASTLE-Benchmark
name: CASTLE-190-8.c
author: Krisztofer Zoltan Horvat
version: 1.1
compile: gcc CASTLE-190-8.c -o CASTLE-190-8
vulnerable: false
description: There is no arithmetic overflow in any of the the functions.
cwe: 190
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>


#define ROW 5
#define COL 5

int grid[ROW][COL];
int visited[ROW][COL];

// Define a struct to hold the x, y coordinates of a cell
typedef struct {
    int x;
    int y;
} Point;

// Define a queue to hold points
struct Queue {
    Point arr[ROW*COL];
    int front, rear;
};

// Create a new queue
struct Queue* newQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        return NULL;
    }

    queue->front = queue->rear = -1;
    return queue;
}

// Check if the queue is empty
int isEmpty(struct Queue* queue) {
    return queue->front == -1;
}

// Add a point to the queue
void enqueue(struct Queue* queue, Point point) {
    queue->rear = (queue->rear + 1) % (ROW*COL);
    queue->arr[queue->rear] = point;
    if(queue->front == -1) {
        queue->front++;
    }
}

typedef struct {
    Point p;
    int hasValue;
} OptionalPoint;

// Remove a point from the queue
OptionalPoint dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        Point p = {0, 0};
        OptionalPoint op = {p, 0};
        return op;
    }

    OptionalPoint point = { queue->arr[queue->front], 1 };
    if(queue->front == queue->rear) {
        queue->front = queue->rear = -1;
    } else {
        queue->front = (queue->front + 1) % (ROW*COL);
    }
    return point;
}

// Check if a given point is valid
int isValid(int x, int y) {
    if(x < 0 || x >= ROW || y < 0 || y >= COL) {
        return 0;
    }
    if(grid[x][y] == 1) {
        return 0;
    }
    if(visited[x][y] == 1) {
        return 0;
    }
    return 1;
}

// Find the shortest path between two points on the grid
void findPath(Point start, Point end) {
    struct Queue* queue = newQueue();
    if (queue == NULL) {
        return;
    }

    visited[start.x][start.y] = 1;
    enqueue(queue, start);
    while(!isEmpty(queue)) {
        OptionalPoint op = dequeue(queue);
        if (op.hasValue == 0) {
            break;
        }
        Point curr = op.p;
        if(curr.x == end.x && curr.y == end.y) {
            printf("Path found!\n");
            free(queue);
            return;
        }
        Point up = {curr.x-1, curr.y};
        Point down = {curr.x+1, curr.y};
        Point left = {curr.x, curr.y-1};
        Point right = {curr.x, curr.y+1};
        if(isValid(up.x, up.y)) {
            visited[up.x][up.y] = 1;
            enqueue(queue, up);
        }
        if(isValid(down.x, down.y)) {
            visited[down.x][down.y] = 1;
            enqueue(queue, down);
        }
        if(isValid(left.x, left.y)) {
            visited[left.x][left.y] = 1;
            enqueue(queue, left);
        }
        if(isValid(right.x, right.y)) {
            visited[right.x][right.y] = 1;
            enqueue(queue, right);
        }
    }
    printf("No path found.\n");
    free(queue);
}

// Initialize the grid and visited arrays
void initialize() {
    for(int i=0; i<ROW; i++) {
        for(int j=0; j<COL; j++) {
            grid[i][j] = 0;
            visited[i][j] = 0;
        }
    }
    grid[1][0] = 1;
    grid[1][1] = 1;
    grid[1][2] = 1;
    grid[2][1] = 1;
    grid[3][1] = 1;
    grid[3][2] = 1;
}

int main() {
    initialize();
    Point start = {0, 0};
    Point end = {4, 4};
    findPath(start, end);
    return 0;
}