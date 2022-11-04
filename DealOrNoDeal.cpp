#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <exception>
using namespace std;
vector< pair<int, double> > LoadCases(vector<double>, vector< pair<int, double> >);
void DisplayCases(vector< pair<int, double> >, int);
void DisplayCaseAmounts(vector< pair<int, double> >);
pair<int, double> FindCase(vector< pair<int, double> >, int);
bool CheckCase(vector< pair<int, double> >, int);
vector< pair<int, double> > RemoveCase(vector< pair<int, double> >, int);
vector< pair<int, double> > PlayRound(vector< pair<int, double> >, int, int, int, bool);
int Offer(vector< pair<int, double> >, int);
bool CounterOffer(int, int);
int Menu(vector< pair<int, double> >, int, pair<int, double>);
bool checkIfHigher(vector< pair<int, double> >, int);

vector< pair<int, double> > LoadCases(vector<double> amounts, vector< pair<int, double> > cases){
    srand(time(nullptr));
    for(int i = 1; i <= 26; i++){
        int position = rand() % amounts.size()-1 + 1;
        cases.push_back(make_pair(i, amounts.at(position)));
        amounts.erase(amounts.begin() + position);
    }
    /*
     * simply for testing knowing the amounts ahead of time
    cout << "Random shuffle" << endl;
    for(int i = 0; i < cases.size(); i++){
        cout << cases.at(i).first << " " << cases.at(i).second << endl;
    }
    */
    cout << "Cases loaded" << endl;
    return cases;
}
void DisplayCases(vector< pair<int, double> > cases, int UserCase){
    int counter = 1;
    for(int i = 0; i < cases.size(); i++){
        if(cases.at(i).first != UserCase){
            cout << left << setw(3) << cases.at(i).first;
            if(counter % 7 == 0){
                cout << endl;
            }
            counter++;
        }
    }
    if((cases.size()-1) % 7 != 0) {
        cout << endl;
    }
}
void DisplayCaseAmounts(vector< pair<int, double> > cases){
    cout << "Here are the amounts left in gameplay: " << endl;
    for(int i = 0; i < cases.size(); i++){ //sorts the dollar amounts
        for(int j = 0; j < cases.size()-1; j++){
            if(cases.at(j).second > cases.at(j+1).second){
                pair<int, double> temp = cases.at(j);
                cases.at(j) = cases.at(j + 1);
                cases.at(j + 1) = temp;
            }
        }
    }
    int counter = 1;
    for(int i = 0; i < cases.size(); i++){ //displays the dollar amounts
        if(int(cases.at(i).second) == 0){
            cout << left << "$" << setw(11) << "0.01";
        }else {
            cout << left << "$" << setw(11) << cases.at(i).second;
        }
        if(counter % 3 == 0){
            cout << endl;
        }
        counter++;
    }
    cout << endl;
    cout << "-------------------------------------" << endl;
}
pair<int, double> FindCase(vector< pair<int, double> > cases, int chosen){
    for(int i = 0; i < int(cases.size()); i++){ //returns the case that the player chose to open
        if(cases.at(i).first == chosen){
            return cases.at(i);
        }
    }
    return cases.at(0);
}
bool CheckCase(vector< pair<int, double> > cases, int chosen){
    for(int i = 0; i < int(cases.size()); i++){ //returns the amount in the case that the player chose to open
        if(cases.at(i).first == chosen){
            return true;
        }
    }
    return false;
}
vector< pair<int, double> > RemoveCase(vector< pair<int, double> > cases, int chosen){
    for(int i = 0; i < int(cases.size()); i++){ //once a case has been opened, it should be removed from the list
        if(cases.at(i).first == chosen) {
            cases.erase(cases.begin() + i);
            return cases;
        }
    }
    return cases;
}
vector< pair<int, double> > PlayRound(vector< pair<int, double> > cases, int roundNum, int casesToOpen, int userCase, bool done){
    if(!done)
        cout << "Round " << roundNum << endl;
    cout << "-------------------------------------" << endl;
    int caseChoice = 0;
    pair <int, double> currentCase;
    while(casesToOpen > 0) {
        cout << casesToOpen << " cases left to open" << endl;
        do {
            if(!done)
                cout << "Select a case (type the number): " << endl;
            else
                cout << "Your next case choice would have been (type the number): " << endl;
            DisplayCases(cases, userCase);
            cin >> caseChoice;
            if(!CheckCase(cases, caseChoice)){
                cout << "That case doesn't exist! Please try again" << endl;
            }
            if(caseChoice == userCase){
                cout << "You selected this case at the beginning of the game, you cannot select it again" << endl;
            }
        }while(!CheckCase(cases, caseChoice) || caseChoice == userCase);
        currentCase = FindCase(cases, caseChoice);
        if(int(currentCase.second) == 0){
            cout << "This case is worth: $0.01" << endl;
        }else {
            cout << "This case is worth: $" << currentCase.second << endl;
        }
        cout << "-------------------------------------" << endl;
        cases = RemoveCase(cases, caseChoice);
        casesToOpen--;
    }
    if(!done)
        cout << "Round " << roundNum << " is over!" << endl;
    return cases;
}
int Offer(vector< pair<int, double> > cases, int roundNum){
    int currentOffer = 0;
    for(int i = 0; i < cases.size(); i++){
        currentOffer += cases.at(i).second;
    }
    currentOffer /= cases.size();
    switch(roundNum){
        case 1:
            currentOffer *= .25;
            break;
        case 2:
            currentOffer *= .4;
            break;
        case 3:
            currentOffer *= .5;
            break;
        case 4:
            currentOffer *= .75;
            break;
        case 5:
            currentOffer *= .9;
        default:
            break;
    }
    if(currentOffer > 1000) {
        while (currentOffer % 1000 != 0) {
            currentOffer++;
        }
    }else if(currentOffer > 100){
        while (currentOffer % 100 != 0) {
            currentOffer++;
        }
    }else{
        while(currentOffer % 10 != 0){
            currentOffer++;
        }
    }
    return currentOffer;
}
bool CounterOffer(int currentOffer, int proposal){ //ideally, the counter offer should be rejected more often when there are fewer high amounts
    return currentOffer * 1.25 > proposal;
}
bool checkIfHigher(vector< pair<int, double> > cases, int offer){
    for(int i = 0; i < cases.size(); i++){ //sorts the dollar amounts
        for(int j = 0; j < cases.size()-1; j++){
            if(cases.at(j).second > cases.at(j+1).second){
                pair<int, double> temp = cases.at(j);
                cases.at(j) = cases.at(j + 1);
                cases.at(j + 1) = temp;
            }
        }
    }
    return cases.at(cases.size() - 1).second > offer;
}
int Menu(vector< pair<int, double> > cases, int UserCaseNum, pair<int, double> UserCase) {
    int choice = 0;
    int roundNum = 1;
    int currentOffer = 0;
    int casesToOpen = 6;
    bool hasCounterOffer = true;
    char playOn = ' ';
    while (choice != 1) {
        if (roundNum > 5) {
            casesToOpen = 1;
        }
        cases = PlayRound(cases, roundNum, casesToOpen, UserCaseNum, false);
        currentOffer = Offer(cases, roundNum);
        DisplayCaseAmounts(cases);
        if (cases.size() == 2) {
            cout << "This is your FINAL offer!" << endl;
        }
        cout << "The banker's offer is: $" << currentOffer << endl;
        cout << "What would you like to do? (please enter the number)" << endl;
        if(cases.size() != 2) {
            cout << "If you say NO DEAL, you will have to open up " << (casesToOpen > 1 ? casesToOpen - 1 : 1)
                 << " more cases." << endl;
        }
        if (hasCounterOffer) {
            do {
                cout << "1. DEAL!" << endl;
                cout << "2. NO DEAL!" << endl;
                cout << "3. COUNTER-OFFER!" << endl;
                //cout << "4. SEE RULES" << endl;
                //try {
                cin >> choice;
                //    if (choice != int(choice))
                //        throw exception();
                //}catch (exception &e){
                //    cout << "Please enter an integer" << endl;
                //}
            } while (choice < 1 || choice > 3);
            if (choice == 3) {
                hasCounterOffer = false;
                int counterOffer = 0;
                do {
                    cout << "What would you like to counter at? (please enter the number without any dollar signs, commas, or decimal points)" << endl;
                    cin >> counterOffer;
                    if (counterOffer <= currentOffer) {
                        cout << "You cannot counter for less than or equal to the current offer!" << endl;
                    }
                } while (counterOffer <= currentOffer);
                if (!CounterOffer(currentOffer, counterOffer)) {
                    cout << "The banker rejected your offer!" << endl;
                } else {
                    cout << "The banker accepted your offer!" << endl;
                    //do{
                    //    cout << "Would you like to see what would've happened if the banker REJECTED your offer? (y or n)" << endl;
                    //    cin >> playOn;
                    //}while(!(tolower(playOn) == 'y' || tolower(playOn) == 'n') );
                    //if(playOn == 'y'){
                    //
                    //}else {
                    cout << "Congrats, you're going home with: $" << counterOffer << endl;
                    cout << "But, was it a good deal?" << endl;
                    cout << "Had the banker said NO DEAL, you would have had to open " <<
                        (casesToOpen > 1 ? casesToOpen : 1) << " more cases." << endl;
                    do{
                        if(casesToOpen == 0 and roundNum > 5){
                            casesToOpen = 1;
                        }
                        cases = PlayRound(cases, roundNum, casesToOpen, UserCaseNum, true);
                        //if(casesToOpen > 1)
                        //    casesToOpen--;
                        roundNum++;
                        casesToOpen--;
                        DisplayCaseAmounts(cases);
                        currentOffer = Offer(cases, roundNum);
                        if(!checkIfHigher(cases, counterOffer) or cases.size() < 2)
                            break;
                        cout << "And the banker's offer would have been: $" << currentOffer << endl;
                        cout << "Had you said NO DEAL, you would have had to open " <<
                            (casesToOpen > 1 ? casesToOpen : 1) << " more cases." << endl;
                    }while(checkIfHigher(cases, counterOffer) or cases.size() > 2);
                    cout << "Here are the amounts in the remaining cases: " << endl;
                    for(int i = 0; i < cases.size(); i++){
                        cout << cases.at(i).first << " " << "$" << cases.at(i).second << endl;
                    }
                    return counterOffer;
                    //}
                }
            }
        } else {
            do {
                cout << "1. DEAL!" << endl;
                cout << "2. NO DEAL!" << endl;
                //cout << "4. SEE RULES" << endl;
                cin >> choice;
                //if(choice == 3) {
                //    cout << "You already used your counter offer!" << endl;
                //}
            } while (choice < 1 || choice > 2);
        }
        roundNum++;
        casesToOpen--;
        if (cases.size() == 2) {
            if(choice == 1)
                return currentOffer;
            char swapChoice = ' ';
            do {
                cout << "You are down to two cases. Would you like to SWAP your case with the remaining one and go home with what is in there?" << endl;
                cout << "(enter y for YES or n for NO)" << endl;
                cin >> swapChoice;
                if (tolower(swapChoice) == 'y') {
                    pair<int, double> remainingCase;
                    cout << cases.size() << endl;
                    for (int i = 0; i < cases.size(); i++) {
                        if (cases.at(i).first != UserCaseNum) {
                            remainingCase = cases.at(i);
                        }
                    }
                    swap(UserCase, remainingCase);
                    cout << "You swapped with case: " << UserCase.first << endl;
                    cout << "You're going home with what you swapped with." << endl;
                    return UserCase.second;
                } else if(tolower(swapChoice) == 'n'){
                    cout << "You're going home with what's in your case." << endl;
                    return -1;
                }
            } while (!(tolower(swapChoice) == 'n' || tolower(swapChoice) == 'y'));
        }
    }
    cout << "Congrats, you're going home with: $" << currentOffer << endl;
    cout << "But, was it a good deal?" << endl;
    cout << "Had you said NO DEAL, you would have had to open " <<
        (casesToOpen > 1 ? casesToOpen : 1) << " more cases." << endl;
    int offer;
    do{
        if(casesToOpen == 0 and roundNum > 5){
            casesToOpen = 1;
        }
        cases = PlayRound(cases, roundNum, casesToOpen, UserCaseNum, true);
        roundNum++;
        casesToOpen--;
        //if(casesToOpen > 1)
        //    casesToOpen--;
        DisplayCaseAmounts(cases);
        offer = Offer(cases, roundNum);
        if(!checkIfHigher(cases, offer) or cases.size() < 2)
            break;
        cout << "And the banker's offer would have been: $" << offer << endl;
        cout << "Had you said NO DEAL, you would have had to open " <<
            (casesToOpen > 1 ? casesToOpen : 1) << " more cases." << endl;
    }while(checkIfHigher(cases, offer) or cases.size() > 2);
    cout << "Here are the amounts in the remaining cases: " << endl;
    for(int i = 0; i < cases.size(); i++){
        cout << cases.at(i).first << " " << "$" << cases.at(i).second << endl;
    }
    return currentOffer;
}

int main() {
    //vector<double> amounts = {0.01, 1, 5, 10, 25, 50, 75, 100, 200, 300, 400, 500, 750,
                              //1000, 5000, 10000, 25000, 50000, 75000, 100000, 200000,
                              //300000, 400000, 500000, 750000, 1000000};
    string name;
    cout << "Enter your name:" << endl;
    getline(cin, name);
    cout << "Welcome to Deal or No Deal, " << name << "!" << endl;
    cout << "I'm your host Howie Mandel and you could walk away today with one million dollars!" << endl;
    char ruleChoice = ' ';
    do{
        cout << "Would you like to see the rules (enter y) or start the game (enter n)?" << endl;
        cin >> ruleChoice;
        if(tolower(ruleChoice) == 'y'){
            cout << "RULES:" << endl;
            cout << " - There are 26 different dollar amounts packed inside 26 cases. Each case is holding a different dollar amount." << endl;
            cout << " - You will select one case to hold onto. You may end up keeping this case or eventually selling it." << endl;
            cout << "     When the game is over, we will see what your case was worth." << endl;
            cout << " - After you make your selection, you will select 6 cases to open." << endl;
            cout << "     After that you will receive an offer from the banker. The banker wants to send you home with as little money as possible." << endl;
            cout << " - You can choose one of three options: " << endl;
            cout << "    - DEAL: You go home with the amount of money the banker offers" << endl;
            cout << "    - NO DEAL: You reject the offer and open up 5 more cases" << endl;
            cout << "        (after each successive round, the number of cases you have to open goes down by 1)" << endl;
            cout << "    - COUNTER-OFFER: You can negotiate a price with the banker. It must be a REASONABLE price, or she will REJECT it." << endl;
            cout << "        Also, you can only do this ONCE in the game." << endl;
            cout << "       - If she says DEAL, you go home with your counter offer" << endl;
            cout << "       - If she says NO DEAL, you must play another round and open cases" << endl;
            cout << " - After each round, you will be prompted with the above options." << endl;
            cout << " - After round 5, you only have to open one case at a time before the banker gives you another offer." << endl;
            cout << " - If you get down to two cases, the last offer will be presented to you." << endl;
            cout << " - If you reject the last offer, you can either go home with what is in your case OR SWAP with the remaining case and go home with what is in there." << endl;
            cout << "----------------------------------------------------------------------------------------------------------" << endl;
            cout << "Press n when you are ready to start the game" << endl;
            do {
                cin >> ruleChoice;
            }while(ruleChoice != 'n');
        }
    }while(!(tolower(ruleChoice) == 'n' || tolower(ruleChoice) == 'y'));
    cout << "There are 26 dollar amounts and 26 cases." << endl;
    cout << "Each case was randomly packed by a third party so we have no idea what's in each case." << endl;
    cout << "Amounts:" << endl;
    int counter = 1;
    for(int i = 0; i < amounts.size(); i++) {
        cout.imbue(locale(""));
        if(int(amounts.at(i)) == 0) {
            cout << left << setw(12) << "$0.01 ";
        }else {
            cout << left << fixed << setprecision(0) << "$" << setw(10) << amounts.at(i) << " ";
        }
        if(counter % 7 == 0){
            cout << endl;
        }
        counter++;
    }
    cout << endl;
    cout << "----------------------------------------------------------------------------------------------------------" << endl;
    //vector< pair<int, double> > cases;
    cases = LoadCases(amounts, cases);
    int chosenCase = 0;
    DisplayCases(cases, chosenCase);
    do{
        cout << "Select a case to hold onto until the end of the game (type a number between 1 and 26): " << endl;
        cin >> chosenCase;
    }while(chosenCase < 1 || chosenCase > 26);
    cout << "You chose case " << chosenCase << ". Should you choose to go all the way to the end, you may end up going home with what's in here." << endl;
    cout << "------------------------------------------" << endl;
    pair<int, double> userCase;
    userCase = FindCase(cases, chosenCase);
    int amountWon = Menu(cases, chosenCase, userCase);
    if(amountWon == -1) { //player turns down every offer and goes home with the case they chose
        if (int(userCase.second) == 0) {
            cout << "Your case was worth: $0.01" << endl;
            cout << "You're going home with: $0.01" << endl;
        } else {
            cout << "Your case was worth: $" << userCase.second << endl;
            cout << "You're going home with: $" << userCase.second << endl;
            if(userCase.second == 1000000)
                cout << "CONGRATULATIONS, YOU ARE A MILLIONAIRE!!!" << endl;
        }
    }else { //player takes the deal or the banker accepts their counter offer
        cout << "You're going home with: $" << amountWon << endl;
        //only open cases if the player would only need to open 1 case at a time
        //only show offers until the user knocks out the largest amount, then STOP
        if(int(userCase.second) == 0){
            cout << "Your case was worth: $0.01" << endl;
        }else {
            cout << "Your case was worth: $" << userCase.second << endl;
        }
        if(amountWon > userCase.second){
            cout << "You made a good deal!" << endl;
        }else if(amountWon < userCase.second){
            cout << "You made a bad deal!" << endl;
        }else{
            cout << "You broke even! You sold this case for what it was worth!" << endl;
        }
    }
    cout << "Thanks for playing Deal or No Deal " << name << "!" << endl;
    return 0;
}
