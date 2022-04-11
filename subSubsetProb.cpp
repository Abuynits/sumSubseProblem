// ---------------------------------------------------------------------------
// Assignment #1 The Subset-Sum Problem Part B - iTunes Version

#include <iostream>
#include <string>
#include <vector>
# include "/Users/abuynits/CLionProjects/CS2CIntro/CS_2C_Files/iTunes.h"
#include <time.h>

using namespace std;

// global scope function prototypes ----------------
int operator+(int n, const iTunesEntry &tune);

ostream &operator<<(ostream &out, const iTunesEntry &tune);

// global scope helper
int computeMasterSum(const vector<iTunesEntry> &data);

void showEntireVector(vector<iTunesEntry> *data);

// --------------- Sublist Prototype ---------------
class Sublist {

public:
    Sublist(const vector<iTunesEntry> *orig) : sum(0), originalObjects(orig) {}

    Sublist(const Sublist &oldList) : sum(oldList.sum), indices(oldList.indices),
                                      originalObjects(oldList.originalObjects) {}

    int getSum() const { return sum; }

    Sublist addItem(int indexOfItemToAdd);

    void showSublist() const;

private:
    int sum;
    const vector<iTunesEntry> *originalObjects;
    vector<int> indices;
};

// ------------------ main ------------------

int main() {
    int target = 0;
    vector<iTunesEntry> dataSet;
    vector<Sublist> choices;
    int arraySize, masterSum;
    bool needAlgorithm;

    // read the data
    iTunesEntryReader tunesInput(
            "/Users/abuynits/CLionProjects/CS2CIntro/CS_2C_Client_Support/iTunes Folder/itunes_new.txt");
    if (tunesInput.readError()) {
        cout << "couldn't open " << tunesInput.getFileName()
             << " for input.\n";
        exit(1);
    }

    // time the algorithm -------------------------
    clock_t startTime, stopTime;
    startTime = clock();

    // create a vector of objects for our own use:
    arraySize = tunesInput.getNumTunes();
    dataSet.reserve(arraySize);//preallocated defined array size
    for (int k = 0; k < arraySize; k++)
        dataSet.push_back(tunesInput[k]);

    cin >> target;
    cout << "Target time: " << target << endl;

    int preAllocatedSpace = 100;
    if (target > 3000) {
        preAllocatedSpace = 100000000;//allocate memory to prevent wasteful redoubling of vector
    }
    choices.reserve(preAllocatedSpace);


    // dispose of easy case immediately to save lots of time
    masterSum = (int) computeMasterSum(dataSet);
    if (target >= masterSum) {
        cout << "Solution is entire master set with sum = " << masterSum << endl;
        showEntireVector( &dataSet );

        needAlgorithm = false;
    } else
        needAlgorithm = true;
    //--------------------------// algorithm
    Sublist initial(&dataSet);//create a new sublist
    if (needAlgorithm) {
        choices.push_back(initial);//add an empty sublist to choices

        bool foundPerfectSublist = false;
        int maxValue = 0;
        int bestSublistIndex = 0;
        int dataLoc = 0;
        while (!foundPerfectSublist && (dataLoc < dataSet.size())) {//loop over all data points

            int fixedChoicesLength = choices.size();//get the size of choices
            cout<<choices.size()<<","<<endl;

            for (int listLoc = 0; listLoc < fixedChoicesLength; listLoc++) {//loop over choices (only current members)

                Sublist list = choices.at(listLoc);//get the current Sublist
                int potentialSum = dataSet.at(dataLoc).getTime() + list.getSum();//get the potential sum (sum(L)+x)

                if (potentialSum == target) {//check if sum=target. If so, break out of all loops

                    Sublist nextList = list.addItem(dataLoc);//add the solution to choices
                    choices.push_back(nextList);

                    foundPerfectSublist = true;//break the loop

                    maxValue = target;
                    bestSublistIndex = choices.size() - 1;//set the choice index to the most recent addition

                } else if (potentialSum < target) {//if sum(L)+x < target, make a new sublist

                    Sublist nextList = list.addItem(dataLoc);
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
//
//
//
////        cout << endl << choices.size() << endl;
////
////        // determine the time elapsed -------------------
//        stopTime = clock();
        cout << "\nAlgorithm Elapsed Time: "
             << (double) (stopTime - startTime) / (double) CLOCKS_PER_SEC
             << " seconds." << endl << endl;
//        cout<<"maxChoiceCount: "<<preAllocatedSpace<<endl;
//        cout<<"sizeNumber: "<<choices.size();

    }
    return 0;
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
int computeMasterSum(const vector<iTunesEntry> &data) {
    int sum = 0;
    for (const iTunesEntry &entry: data) {
        sum += entry.getTime();
    }
    return sum;
}

void showEntireVector(vector<iTunesEntry> *data) {
    Sublist s(data);


        for (int i = 0; i < data->size() - 1; i++) {
            iTunesEntry dataPoint = data->at(i);
            s=s.addItem(i);
//            cout << "array[" << i << "] = " << dataPoint << " ,   ";
        }
//        cout << "array[" << (data->size() - 1) << "] = " << data->at((data->size() - 1)) << " " << endl << endl;
        s.showSublist();
//
}

// --------------- Sublist Method Definitions ---------------

//void Sublist::showSublist() const {
//    int k;
//
//    cout << "Sublist ----------------------------- " << endl;
//    cout << "  sum: " << sum << endl;
//    int lastLoc = indices.size() - 1;
//    if (lastLoc > 0) {
//    cout << "  ";
//    for (k = 0; k < (int) indices.size() - 1; k++) {
//        cout << "array[" << indices[k] << "] = ";
//        cout << (*originalObjects)[indices[k]] << " ,   ";
//    }
//        cout << "array[" << indices[lastLoc] << "] = " << (*originalObjects)[indices[lastLoc]] << " ";
//    }
//    cout<<endl << endl;
//}
void Sublist::showSublist() const
{
    int k;

    cout << "Sublist ----------------------------- " << endl;
    cout << " sum: " << sum << endl;
    for ( k = 0; k < (int)indices.size(); k++)
        cout << " array[" << indices[k] << "] = "
             << (*originalObjects)[ indices[k] ]
             << ( (k == (int)indices.size() - 1)? "\n\n" : ", " );
}
Sublist Sublist::addItem(int indexOfItemToAdd) {

    Sublist nextList = *this;//copy constructor
    nextList.sum += (originalObjects->at(indexOfItemToAdd)).getTime();
    nextList.indices.push_back(indexOfItemToAdd);//update indices vector

    return nextList;
}
/*
================RUNS BELOW================
0
Target time: 0
Sublist -----------------------------
  sum: 0
1200
Target time: 1200
Sublist -----------------------------
  sum: 1200
  array[0] = Cowboy Casanova by Carrie Underwood(236) ,   array[1] = Quitter by Carrie Underwood(220) ,   array[2] = Russian Roulette by Rihanna(228) ,   array[6] = Bad Love by Eric Clapton(308) ,   array[11] = Hot Cha by Roy Buchanan(208)
3600
Target time: 3600
Sublist -----------------------------
  sum: 3600
  array[0] = Cowboy Casanova by Carrie Underwood(236) ,   array[1] = Quitter by Carrie Underwood(220) ,   array[2] = Russian Roulette by Rihanna(228) ,   array[4] = Monkey Wrench by Foo Fighters(230) ,   array[5] = Pretending by Eric Clapton(283) ,   array[6] = Bad Love by Eric Clapton(308) ,   array[7] = Everybody's In The Mood by Howlin' Wolf(178) ,   array[8] = Well That's All Right by Howlin' Wolf(175) ,   array[9] = Samson and Delilah by Reverend Gary Davis(216) ,   array[11] = Hot Cha by Roy Buchanan(208) ,   array[12] = Green Onions by Roy Buchanan(443) ,   array[13] = I'm Just a Prisoner by Janiva Magness(230) ,   array[14] = You Were Never Mine by Janiva Magness(276) ,   array[15] = Hobo Blues by John Lee Hooker(187) ,   array[16] = I Can't Quit You Baby by John Lee Hooker(182)
4799
Target time: 4799
Sublist -----------------------------
  sum: 4799
  array[0] = Cowboy Casanova by Carrie Underwood(236) ,   array[1] = Quitter by Carrie Underwood(220) ,   array[2] = Russian Roulette by Rihanna(228) ,   array[3] = All My Life by Foo Fighters(263) ,   array[4] = Monkey Wrench by Foo Fighters(230) ,   array[5] = Pretending by Eric Clapton(283) ,   array[6] = Bad Love by Eric Clapton(308) ,   array[8] = Well That's All Right by Howlin' Wolf(175) ,   array[9] = Samson and Delilah by Reverend Gary Davis(216) ,   array[10] = Twelve Sticks by Reverend Gary Davis(194) ,   array[11] = Hot Cha by Roy Buchanan(208) ,   array[12] = Green Onions by Roy Buchanan(443) ,   array[13] = I'm Just a Prisoner by Janiva Magness(230) ,   array[14] = You Were Never Mine by Janiva Magness(276) ,   array[15] = Hobo Blues by John Lee Hooker(187) ,   array[16] = I Can't Quit You Baby by John Lee Hooker(182) ,   array[17] = That's The Homie by Snoop Dogg(343) ,   array[19] = Ladies and Gentleman by The Rubyz(201) ,   array[20] = Watch the Girl by The Rubyz(192) ,   array[21] = Donuts for Benny by Veggie Tales(184)
100000
Target time: 100000
Solution is entire master set with sum = 22110
  array[0] = Cowboy Casanova by Carrie Underwood(236) ,   array[1] = Quitter by Carrie Underwood(220) ,   array[2] = Russian Roulette by Rihanna(228) ,   array[3] = All My Life by Foo Fighters(263) ,   array[4] = Monkey Wrench by Foo Fighters(230) ,   array[5] = Pretending by Eric Clapton(283) ,   array[6] = Bad Love by Eric Clapton(308) ,   array[7] = Everybody's In The Mood by Howlin' Wolf(178) ,   array[8] = Well That's All Right by Howlin' Wolf(175) ,   array[9] = Samson and Delilah by Reverend Gary Davis(216) ,   array[10] = Twelve Sticks by Reverend Gary Davis(194) ,   array[11] = Hot Cha by Roy Buchanan(208) ,   array[12] = Green Onions by Roy Buchanan(443) ,   array[13] = I'm Just a Prisoner by Janiva Magness(230) ,   array[14] = You Were Never Mine by Janiva Magness(276) ,   array[15] = Hobo Blues by John Lee Hooker(187) ,   array[16] = I Can't Quit You Baby by John Lee Hooker(182) ,   array[17] = That's The Homie by Snoop Dogg(343) ,   array[18] = Gangsta Luv by Snoop Dogg(257) ,   array[19] = Ladies and Gentleman by The Rubyz(201) ,   array[20] = Watch the Girl by The Rubyz(192) ,   array[21] = Donuts for Benny by Veggie Tales(184) ,   array[22] = Our Big Break by Veggie Tales(69) ,   array[23] = Brahms: Symphony No. 1 in C Minor Op. 68 by Berliner Philharmoniker(839) ,   array[24] = Brahms: Symphony No. 4 in E Minor Op. 98 by Berliner Philharmoniker(800) ,   array[25] = Bach: Suite for Cello No. 1 in G Major Prelude by Yo-yo Ma(141) ,   array[26] = Simple Gifts by Yo-yo Ma(154) ,   array[27] = Alimony by Ry Cooter(175) ,   array[28] = France Chance by Ry Cooter(168) ,   array[29] = The Road by Aaron Watson(204) ,   array[30] = Clone by Terra Incognita(298) ,   array[31] = Lizard Skin by Terra Incogni(270) ,   array[32] = Bullhead's Psalm by Blue Record(79) ,   array[33] = Ogeechee Hymnal by Blue Record(155) ,   array[34] = Oblivion by Mastadon(348) ,   array[35] = The Bit by Mastadon(295) ,   array[36] = Fire Burning by Sean Kingston(239) ,   array[37] = My Girlfriend by Sean Kingston(204) ,   array[38] = Take Your Shirt Off by T-Pain(228) ,   array[39] = Give It All U Got by Lil Jon(218) ,   array[40] = What We Talkin' About by Jay-Z(243) ,   array[41] = Empire State of Mind by Jay-Z(276) ,   array[42] = Think About It by Snoop Dog(217) ,   array[43] = Lil' Crips by Snoop Dog(195) ,   array[44] = Shuffleboard by Jeff Golub(210) ,   array[45] = Goin' On by Jeff Golub(356) ,   array[46] = Fish Fare by Jeff Golub(299) ,   array[47] = Noites Cariocas by Caraivana(252) ,   array[48] = Tico-Tico No Fuba by Caraivana(147) ,   array[49] = Monk/Trane by John Patitucci(434) ,   array[50] = Sonny Side by John Patitucci(445) ,   array[51] = Pirate Jenny by Nina Simone(402) ,   array[52] = The Other Woman by Nina Simone(186) ,   array[53] = Feeling Good by Nina Simone(177) ,   array[54] = A Love Supreme Part 1 by John Coltrane(462) ,   array[55] = In a Sentimental Mood by John Coltrane(256) ,   array[56] = Dime Si te Vas Con El by AOL Dejando Huellas(204) ,   array[57] = Te Amo Tanto by AOL Dejando Huella(192) ,   array[58] = Blues On the Corner by McCoy Tyner(367) ,   array[59] = Afro Blue by McCoy Tyner(742) ,   array[60] = Stronger by Kanye West(311) ,   array[61] = Good Life by Kanye West(207) ,   array[62] = Black Cow by Steely Dan(310) ,   array[63] = Kid Charlemagne by Steely Dan(278) ,   array[64] = Haitian Divorce by Steely Dan(351) ,   array[65] = Nefertiti by Herbie Hancock(451) ,   array[66] = Rockit by Herbie Hancock(325) ,   array[67] = Chameleon by Herbie Hancock(941) ,   array[68] = Medieval Overture by Return to Forever(313) ,   array[69] = Luka by Suzanne Vega(231) ,   array[70] = Small Blue Thing by Suzanne Vega(235) ,   array[71] = Something to Talk About by Bonnie Raitt(227) ,   array[72] = I Can't Make You Love Me by Bonnie Raitt(331) ,   array[73] = This Will Be by Natalie Cole(171) ,   array[74] = Unforgettable by Natalie Cole(211) ,   array[75] = Timothy by Jet(260) ,   array[76] = Rip It Up by Jet(200) ,   array[77] = Where Did Your Heart Go? by Was (Not Was)(347) ,   array[78] = I Blew Up The United States by Was (Not Was)(230)

 */
