// ---------------------------------------------------------------------------
// Assignment #1 The Subset-Sum Problem Part C
//Alexiy Buynitsky

#include <iostream>
#include <string>
#include <vector>
# include "/Users/abuynits/CLionProjects/CS2CIntro/CS_2C_Files/iTunes.h"
#include <time.h>

using namespace std;

// global scope function prototypes ----------------
int operator+(int n, const iTunesEntry &tune);

ostream &operator<<(ostream &out, const iTunesEntry &tune);

// global scope helpergit remote add origin https://github.com/Abuynits/sumSubsetProblem.git
template<typename T>
int computeMasterSum(const vector<T> *data);

template<typename T>
void showEntireVector(int sum, vector<T> *data);

template<typename T>
void runCalc(vector<T> *dataSet, int target);

template<typename T>
bool checkIfNeedAlgorithm(vector<T> *dataSet, int target);

// --------------- Sublist Prototype ---------------
template<class Object>
class Sublist {

public:
    Sublist(const vector<Object> *orig) : sum(0), originalObjects(orig) {}

    Sublist(const Sublist &oldList) : sum(oldList.sum), indices(oldList.indices),
                                      originalObjects(oldList.originalObjects) {}

    int getSum() const { return sum; }

    Sublist addItem(int indexOfItemToAdd);

    void showSublist() const;

private:
    int sum;
    const vector<Object> *originalObjects;
    vector<int> indices;
};

// ------------------ main ------------------
int main() {

    vector<int> dataSetInt;

    cout << "For int:" << endl;
    int intTarget;
    cin >> intTarget;

    dataSetInt.push_back(20);
    dataSetInt.push_back(12);
    dataSetInt.push_back(22);
    dataSetInt.push_back(15);
    dataSetInt.push_back(25);
    dataSetInt.push_back(19);
    dataSetInt.push_back(29);
    dataSetInt.push_back(18);
    dataSetInt.push_back(11);
    dataSetInt.push_back(13);
    dataSetInt.push_back(17);

    // time the algorithm -------------------------
//    clock_t startTime, stopTime;
//    startTime = clock();

    //run the method for ints
    runCalc(&dataSetInt, intTarget);

    cout << "-------------------" << endl;
    //setup itunes
    int target = 0, arraySize;
    vector<iTunesEntry> dataSet;


    // read the data
    iTunesEntryReader tunesInput(
            "/Users/abuynits/CLionProjects/CS2CIntro/CS_2C_Client_Support/iTunes Folder/itunes_new.txt");
    if (tunesInput.readError()) {
        cout << "couldn't open " << tunesInput.getFileName()
             << " for input.\n";
        exit(1);
    }

    arraySize = tunesInput.getNumTunes();//get size
    dataSet.reserve(arraySize);//preallocated defined array size

    for (int k = 0; k < arraySize; k++)
        dataSet.push_back(tunesInput[k]);

    cin >> target;
    cout << "itunes time: " << target << endl;

    runCalc(&dataSet, target);//run on the itunes

    return 0;
}

template<typename T>
void runCalc(vector<T> *dataSet, int target) {
    vector<Sublist<T>> choices;//create choices and allocate memory
    int preAllocatedSpace = 100;
    if (target > 3000) {
        preAllocatedSpace = 100000000;//allocate memory to prevent wasteful redoubling of vector
    }
    choices.reserve(preAllocatedSpace);

    //start algorithm
    bool needAlgorithm = checkIfNeedAlgorithm(dataSet, target);

    //--------------------------// algorithm
    Sublist<T> initial(dataSet);//create a new sublist
    if (needAlgorithm) {
        choices.push_back(initial);//add an empty sublist to choices

        bool foundPerfectSublist = false;
        int maxValue = 0;
        int bestSublistIndex = 0;
        int dataLoc = 0;
        while (!foundPerfectSublist && (dataLoc < dataSet->size())) {//loop over all data points

            int fixedChoicesLength = choices.size();//get the size of choices

            for (int listLoc = 0; listLoc < fixedChoicesLength; listLoc++) {//loop over choices (only current members)

                Sublist<T> list = choices.at(listLoc);//get the current Sublist
                int potentialSum =
                        choices.at(listLoc).getSum() + dataSet->at(dataLoc);//get the potential sum (sum(L)+x)

                if (potentialSum == target) {//check if sum=target. If so, break out of all loops

                    Sublist<T> nextList = list.addItem(dataLoc);//add the solution to choices
                    choices.push_back(nextList);

                    foundPerfectSublist = true;//break the loop

                    maxValue = target;
                    bestSublistIndex = choices.size() - 1;//set the choice index to the most recent addition

                } else if (potentialSum < target) {//if sum(L)+x < target, make a new sublist

                    Sublist<T> nextList = list.addItem(dataLoc);
                    choices.push_back(nextList);

                    if (potentialSum > maxValue) {//update the max and bestSublist

                        maxValue = potentialSum;
                        bestSublistIndex = choices.size() - 1;

                    }

                }
                if (foundPerfectSublist) {//break out of for loop if a perfect value is found
                    break;
                }
            }

            dataLoc++;//increment the data points
        }
        choices[bestSublistIndex].showSublist();
        // cout<<endl;

//        cout << endl << choices.size() << endl;
//
//        // determine the time elapsed -------------------
//        stopTime = clock();
//        cout << "\nAlgorithm Elapsed Time: "
//             << (double) (stopTime - startTime) / (double) CLOCKS_PER_SEC
//             << " seconds." << endl << endl;
//        cout<<"maxChoiceCount: "<<preAllocatedSpace<<endl;
//        cout<<"sizeNumber: "<<choices.size();

    }
}

template<typename T>
bool checkIfNeedAlgorithm(vector<T> *dataSet, int target) {
    int masterSum;
    // dispose of easy case immediately to save lots of time
    masterSum = (int) computeMasterSum(dataSet);
    if (target >= masterSum) {

        showEntireVector(masterSum, dataSet);
        return false;
    } else
        return true;
}

// global scope functions ---------------------------
int operator+(int n, const iTunesEntry &tune) {
    return n + tune.getTime();
}

ostream &operator<<(ostream &out, const iTunesEntry &tune) {
    //array[0] = Cowboy Casanova by Carrie Underwood(236) ,   array[1] = Quitter by Carrie Underwood(220) ,
    out << tune.getTitle() << " by ";
    cout << tune.getArtist() << "(";
    cout << tune.getTime() << ")";
    return out;
}

// Helper method to compute full sum
template<typename T>
int computeMasterSum(const vector<T> *data) {
    int sum = 0;
    for (int i = 0; i < data->size(); i++) {
        sum = sum + data->at(i);
    }

    return sum;
}

template<typename T>
void showEntireVector(int sum, vector<T> *data) {
    cout << "Solution is entire master set with sum = " << sum << endl;
    cout << "  ";
    for (int i = 0; i < data->size() - 1; i++) {
        T dataPoint = data->at(i);

        cout << "array[" << i << "] = " << dataPoint << " ,   ";
    }
    cout << "array[" << (data->size() - 1) << "] = " << data->at((data->size() - 1)) << " " << endl << endl;
//
}

// --------------- Sublist Method Definitions ---------------
template<typename T>
void Sublist<T>::showSublist() const {
    int k;

    cout << "Sublist ----------------------------- " << endl;
    cout << "  sum: " << sum << endl;
    cout << "  ";
    for (k = 0; k < (int) indices.size() - 1; k++) {
        cout << "array[" << indices[k] << "] = ";
        cout << (*originalObjects)[indices[k]] << " ,   ";
    }
    int lastLoc = indices.size() - 1;
    cout << "array[" << indices[lastLoc] << "] = " << (*originalObjects)[indices[lastLoc]] << endl << endl;
}

template<typename T>
Sublist<T> Sublist<T>::addItem(int indexOfItemToAdd) {
    Sublist nextList = *this;//copy constructor
    nextList.sum = nextList.sum + originalObjects->at(indexOfItemToAdd);
    nextList.indices.push_back(indexOfItemToAdd);//update indices vector

    return nextList;
}
