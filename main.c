#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

unsigned int dim;

typedef struct {
    unsigned int index;
    unsigned int distance;
} Node;

typedef struct {
    Node* array;
    unsigned int capacity;
    unsigned int size;
} MinHeap;

struct NodeCar{
    unsigned int autonomy;
    struct NodeCar* left;
    struct NodeCar* right;
};

struct NodeBST{
    unsigned int stationID;
    struct NodeCar* car;
    struct NodeBST* left;
    struct NodeBST* right;
};

struct Station{
    unsigned int stationID;
    struct NodeCar* car;
};

struct NodeCar* createNodeCar(unsigned int value) {
    struct NodeCar* node = (struct NodeCar*)malloc(sizeof(struct NodeCar));
    node->autonomy = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct NodeCar* insertNodeCar(struct NodeCar* root,unsigned int value) {
    struct NodeCar* ph=NULL;
    struct NodeCar* node=root;
    while (node != NULL) {
        ph=node;
        if(value < node->autonomy)
            node=node->left;
        else
            node=node->right;
    }
    struct NodeCar* new=createNodeCar(value);
    if(ph==NULL) root=new;
    else if(value < ph->autonomy)
        ph->left=new;
    else
        ph->right=new;
    return root;
}

struct NodeBST* createNode(unsigned int value) {
    struct NodeBST* node = (struct NodeBST*)malloc(sizeof(struct NodeBST));
    node->stationID = value;
    node->car = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

struct NodeBST* insertNode(struct NodeBST* root, unsigned int value, unsigned int numCars) {
    struct NodeBST* ph=NULL;
    struct NodeBST* node=root;
    while (node != NULL) {
        ph=node;
        if(value < node->stationID)
            node=node->left;
        else if(value > node->stationID)
            node=node->right;
        else{
            return NULL;
        }
    }
    struct NodeBST* new=createNode(value);
    for (unsigned int i = 0; i < numCars; i++) {
        char s3[32];
        unsigned int autonomy;
        if (scanf("%s", s3) == EOF)return NULL;
        autonomy = strtol(s3,NULL,10);
        new->car=insertNodeCar(new->car, autonomy);
    }
    dim++;
    if(ph==NULL) root=new;
    else if(value < ph->stationID)
        ph->left=new;
    else
        ph->right=new;
    return root;
}
struct NodeBST* minValueNode(struct NodeBST* node) {
    struct NodeBST* current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct NodeCar* minValueNodeCar(struct NodeCar* node) {
    struct NodeCar* current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct NodeCar* maxValueNodeCar(struct NodeCar* node) {
    struct NodeCar* current = node;

    while (current && current->right != NULL)
        current = current->right;

    return current;
}

void inorderTraversal(struct NodeBST* root, unsigned int* stations,unsigned int* cars,unsigned  int* index,unsigned int start,unsigned int end) {
    if (root == NULL) {
        return;
    }
    if(root->stationID>start)inorderTraversal(root->left, stations,cars, index, start, end);
    if(root->stationID>=start && root->stationID<=end){
        stations[*index] = root->stationID;
        cars[*index] = maxValueNodeCar(root->car)->autonomy;
        *index = *index + 1;
    }
    if(root->stationID<end)inorderTraversal(root->right, stations,cars, index,start, end);
}

struct NodeBST* searchNode(struct NodeBST* root,unsigned int value) {
    struct NodeBST* node=root;
    while (node != NULL) {
        if(value < node->stationID)
            node=node->left;
        else if(value > node->stationID)
            node=node->right;
        else{
            return node;
        }
    }
    return NULL;
}



struct NodeBST* deleteNode(struct NodeBST* root, unsigned int value) {
    if (root == NULL){
        printf("non demolita\n");
        return root;
    }
    if (value < root->stationID)
        root->left = deleteNode(root->left, value);
    else if (value > root->stationID)
        root->right = deleteNode(root->right, value);
    else {
        if (root->left == NULL) {
            struct NodeBST* temp = root->right;
            free(root);
            dim--;
            printf("demolita\n");
            return temp;
        } else if (root->right == NULL) {
            struct NodeBST* temp = root->left;
            free(root);
            dim--;
            printf("demolita\n");
            return temp;
        }

        struct NodeBST* temp = minValueNode(root->right);

        root->stationID = temp->stationID;

        root->right = deleteNode(root->right, temp->stationID);
    }

    return root;
}
struct NodeCar* deleteNodeCar(struct NodeCar* root, unsigned int value) {
    if (root == NULL){
        printf("non rottamata\n");
        return root;
    }
    if (value < root->autonomy)
        root->left = deleteNodeCar(root->left, value);
    else if (value > root->autonomy)
        root->right = deleteNodeCar(root->right, value);
    else {
        if (root->left == NULL) {
            struct NodeCar* temp = root->right;
            free(root);
            printf("rottamata\n");
            return temp;
        } else if (root->right == NULL) {
            struct NodeCar* temp = root->left;
            free(root);
            printf("rottamata\n");
            return temp;
        }

        struct NodeCar* temp = minValueNodeCar(root->right);

        root->autonomy = temp->autonomy;

        root->right = deleteNodeCar(root->right, temp->autonomy);
    }

    return root;
}

void freeCar(struct NodeCar* root) {
    if (root == NULL) {
        return;
    }

    freeCar(root->left);
    freeCar(root->right);
    free(root);
}

void freeBST(struct NodeBST* root) {
    if (root == NULL) {
        return;
    }

    freeBST(root->left);
    freeBST(root->right);
    freeCar(root->car);
    free(root);
    dim=0;
}

MinHeap createMinHeap(unsigned int capacity) {
    MinHeap minHeap;
    minHeap.capacity = capacity;
    minHeap.size = 0;
    minHeap.array = malloc(capacity * sizeof(Node));
    return minHeap;
}

void swapNodes(Node* a, Node* b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap,unsigned int idx,unsigned int* distances) {
    unsigned  int smallest = idx;
    unsigned int leftChild = 2 * idx + 1;
    unsigned int rightChild = 2 * idx + 2;

    if (leftChild < minHeap->size && distances[minHeap->array[leftChild].index] < distances[minHeap->array[smallest].index])
        smallest = leftChild;

    if (rightChild < minHeap->size && distances[minHeap->array[rightChild].index] < distances[minHeap->array[smallest].index])
        smallest = rightChild;

    if (smallest != idx) {
        swapNodes(&minHeap->array[idx], &minHeap->array[smallest]);
        minHeapify(minHeap, smallest, distances);
    }
}

unsigned int isEmpty(MinHeap* minHeap) {
    return minHeap->size == 0;
}

Node extractMin(MinHeap* minHeap,unsigned int* distances) {
    Node minNode = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0, distances);
    return minNode;
}

void decreaseDistance(MinHeap* minHeap,unsigned  int index,unsigned  int distance,unsigned  int* distances) {
    int i;
    for (i = 0; i < minHeap->size; i++) {
        if (minHeap->array[i].index == index) {
            break;
        }
    }

    distances[index] = distance;

    while (i && distances[minHeap->array[(i - 1) / 2].index] > distances[minHeap->array[i].index]) {
        swapNodes(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

unsigned int* getShortestPath(unsigned int* prev, unsigned  int start, unsigned int end,unsigned  int* pathSize) {
    unsigned int* path = NULL;
    unsigned  int size = 0;
    unsigned int current = end;

    while (current != start) {
        size++;
        path = realloc(path, size * sizeof(unsigned int));
        path[size - 1] = current;
        current = prev[current];
    }

    size++;
    path = realloc(path, size * sizeof(unsigned int));
    path[size - 1] = start;

    *pathSize = size;

    return path;
}
unsigned int* dijkstrawithbst(struct NodeBST* root, unsigned int* pathSize,unsigned int start,unsigned int end) {
    if(start==end){
        pathSize=1;
        return start;
    }
    unsigned int* stations= (unsigned int*) malloc(dim * sizeof(unsigned int));
    unsigned int* cars= (unsigned int*) malloc(dim * sizeof(unsigned int));
    unsigned int dimArray=0;
    inorderTraversal(root,stations,cars,&dimArray,start,end);

    //vector = realloc(vector, dimArray * sizeof(int));
    unsigned int* distances = malloc(dimArray * sizeof(unsigned int));
    unsigned int* prev = malloc(dimArray * sizeof(unsigned int));
    unsigned int* visited = malloc(dimArray * sizeof(unsigned int));
    unsigned int range;
    for (unsigned int i = 0; i < dimArray; i++) {
        distances[i] = UINT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    distances[0] = 0;

    MinHeap minHeap = createMinHeap(dimArray);
    minHeap.size = dimArray;

    for (unsigned int i = 0; i < dimArray; i++) {
        minHeap.array[i].index = i;
        minHeap.array[i].distance = distances[i];
    }

    while (!isEmpty(&minHeap)) {
        Node minNode = extractMin(&minHeap, distances);
        unsigned int min_index = minNode.index;

        visited[min_index] = 1;

        range=cars[min_index]+stations[min_index];
        unsigned int j=min_index+1;
            if(j<dimArray){
                while(stations[j]<=range){
                    if (visited[j] == 0 && distances[min_index] + j-min_index+1 < distances[j]) {
                        distances[j] = distances[min_index] + j-min_index+1;
                        prev[j] = min_index;
                        decreaseDistance(&minHeap, j, distances[j], distances);
                    }
                    j++;
                }
            }
    }
    unsigned int* path=NULL;
    if(distances[dimArray-1]==UINT_MAX){
        *pathSize=0;
        return NULL;
    }
    else path = getShortestPath(prev, 0, dimArray-1, pathSize);


    free(distances);
    free(prev);
    free(visited);
    free(minHeap.array);

    return path;
}

int main() {
    dim=0;
    struct NodeBST* root = NULL;
    struct NodeBST* tempStation= NULL;
    char command[32];
    char s1[32];
    char s2[32];
    char s3[32];
    unsigned int distance, numCars, autonomy;

    while (1) {
        if (scanf("%s", command) == EOF)break;
        if (strcmp(command, "aggiungi-stazione") == 0) {

            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s2) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            numCars = strtol(s2,NULL,10);


            tempStation=insertNode(root, distance, numCars);
            if(tempStation==NULL)printf("non aggiunta\n");
            else{
                root=tempStation;
                printf("aggiunta\n");
            }

        }

        else if (strcmp(command, "demolisci-stazione") == 0) {

            if(scanf("%s", s1) == EOF)return 0;
            distance = strtol(s1,NULL,10);

            root=deleteNode(root, distance);

        }
        else if (strcmp(command, "aggiungi-auto") == 0) {

            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            autonomy = strtol(s3,NULL,10);

            tempStation=searchNode(root,distance);
            if(tempStation==NULL)printf("non aggiunta\n");
            else{
                tempStation->car=insertNodeCar(tempStation->car, autonomy);
                printf("aggiunta\n");
            }
        }
        else if (strcmp(command, "rottama-auto") == 0) {
            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            distance = strtol(s1,NULL,10);
            autonomy = strtol(s3,NULL,10);

            tempStation=searchNode(root,distance);
            if(tempStation==NULL)printf("non rottamata\n");
            else{
                tempStation->car=deleteNodeCar(tempStation->car, autonomy);
            }
        }
        else if (strcmp(command, "pianifica-percorso") == 0) {

            unsigned int startDistance, endDistance;
            if (scanf("%s", s1) == EOF)return 0;
            if (scanf("%s", s3) == EOF)return 0;
            startDistance = strtol(s1,NULL,10);
            endDistance = strtol(s3,NULL,10);
            unsigned int pathSize;
            unsigned int* shortestPath = dijkstrawithbst(root, &pathSize, startDistance, endDistance);
            if (pathSize == 0) {
                printf("nessun percorso\n");
            }
            else{
                pathSize--;
                for (; pathSize >= 0; pathSize--) {
                    printf("%u", shortestPath[pathSize]);
                    if(pathSize!=0)printf(" ");
                }
                printf("\n");
                free(shortestPath);
            }
        }
    }
    return 0;
}



