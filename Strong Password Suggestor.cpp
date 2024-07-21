#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <regex>
using namespace std;

void shufflePassword(string& password);
string passwordGenerator(int length);
void isStrong(const string& userPassword);
vector<int> hasRepeatedChars(const string& userPassword);
vector<int> hasSequentialChars(const string& userPassword);
string enhancePassword(const string& userPassword, bool& hasLower, bool& hasUpper, bool& hasNumber, bool& hasSpecial, bool& isLong, vector<int>& repeatedIndices, vector<int>& sequentialIndices);

const string specialChar = "[_()#@!$&/]";

// Time Complexity of the whole program ~ O(n)
// n being the number of characters of the password
int main() {
    srand(time(0));

    int choice;
    string password;
    do {
        cout << "Choose:" << endl;
        cout << "1. Generate a strong password.\n" << "2. Check if a password is strong.\n";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number (1 or 2)." << endl;
            choice = 0;
        }
    } while (choice != 1 && choice != 2);

    if (choice == 1) {
        cout << endl;
        cout << "Suggestions:" << endl;
        
        password = passwordGenerator(12);

        cout << "Your generated Password is: " << password << endl;
        isStrong(password);
    }
    else if (choice == 2) {
        cout << "Enter your password:" << endl;
        cin.ignore(1, '\n');
        getline(cin, password);
        isStrong(password);
    }
}

// Generates a totally random password 
// Time Complexity ~ O(1) || O(n) after calling the shufflePassword function
string passwordGenerator(int length) {
    string password = "";

    for (int i = 0; i < 3; i++) {
        password += 'a' + (rand() % 26);
        password += 'A' + (rand() % 26);
        password += '0' + (rand() % 10);
        password += specialChar[rand() % 11];
    }

    shufflePassword(password);
    return password;
}

// Shuffles password's characters using Fisher Yates Algorithm
// Time Complexity ~ O(n)
void shufflePassword(string& password) {
    for (int j = password.length() - 1; j > 0; j--) {
        int index = rand() % (j + 1);
        char temp = password[j];
        password[j] = password[index];
        password[index] = temp;
    }
}

// Classification funciton of how Strong - weak the password is according to given criteria
// Time Complexity ~ O(n)
void isStrong(const string& userPassword) {
    regex lowercaseRegex("[a-z]");
    regex uppercaseRegex("[A-Z]");
    regex numericRegex("[0-9]");
    regex specialRegex("[_\\(\\)\\[\\]#@!$&/]");

    bool hasLower = regex_search(userPassword, lowercaseRegex);
    bool hasUpper = regex_search(userPassword, uppercaseRegex);
    bool hasNumber = regex_search(userPassword, numericRegex);
    bool hasSpecial = regex_search(userPassword, specialRegex);

    bool isLong = (userPassword.length() >= 12);

    vector<int> repeatedIndices = hasRepeatedChars(userPassword);
    vector<int> sequentialIndices = hasSequentialChars(userPassword);
    bool hasRepeated = !(repeatedIndices.empty());
    bool hasSequential = !(sequentialIndices.empty());

    int score = hasLower + hasUpper + hasNumber + hasSpecial + isLong + !hasRepeated + !hasSequential;

    string message;
    if (score == 7) {
        message = "Your password is strong.";
        cout << message << endl;
    }
    else {
        message = score > 4 ? "Your password is moderate. We can:" : "Your password is weak. We can:";
        if (!hasLower) message += "\n- Add some lowercase letters.";
        if (!hasUpper) message += "\n- Add some uppercase letters.";
        if (!hasNumber) message += "\n- Add some numbers.";
        if (!hasSpecial) message += "\n- Add some special characters.";
        if (!isLong) message += "\n- Make the password at least 12 characters long.";
        if (hasRepeated) message += "\n- Avoid repeated characters.";
        if (hasSequential) message += "\n- Avoid sequential characters.";

        string newPassword = enhancePassword(userPassword, hasLower, hasUpper, hasNumber, hasSpecial, isLong, repeatedIndices, sequentialIndices);
        message += "\n\nEnhanced your password:\nYour old password: " + userPassword + "\nEnhanced Strong password: " + newPassword;
        cout << message << endl << endl;

        isStrong(newPassword);
    }
   
}

// Function that checks if the password has any repeated characters
// Example : "aaa", "555"
// Time Complexity ~ O(n)
vector<int> hasRepeatedChars(const string& userPassword) {
    const int repeatedPatternLength = 3;
    vector<int> repeatedIndices;

    if (userPassword.length() < repeatedPatternLength) return repeatedIndices;

    for (size_t i = 0; i <= userPassword.length() - repeatedPatternLength;) {
        if (userPassword[i] == userPassword[i + 1] && userPassword[i] == userPassword[i + 2]) {
            repeatedIndices.push_back(i + repeatedPatternLength - 1);
            i += repeatedPatternLength; 
        }
        else ++i;
        
    }
    return repeatedIndices;
}

// Function that checks if the password has any sequential characters
// Example : "abc", "123"
// Time Complexity ~ O(n)
vector<int> hasSequentialChars(const string& userPassword) {
    const int sequentialPatternLength = 3;
    vector<int> sequentialIndices;

    if (userPassword.length() < sequentialPatternLength) return sequentialIndices;

    for (size_t i = 0; i <= userPassword.length() - sequentialPatternLength;) {
        bool isSequential = true;

        for (size_t j = 1; j < sequentialPatternLength; j++) {
            if (userPassword[i + j] != userPassword[i + j - 1] + 1) {
                isSequential = false;
                break;
            }
        }

        if (isSequential) {
            sequentialIndices.push_back(i + sequentialPatternLength - 1);
            i += sequentialPatternLength;
        }
        else ++i;
    }
    return sequentialIndices;
}

// Function that enhances the password and adds on it according to missing criteria
// Time Complexity ~ O(n)
string enhancePassword(const string& userPassword, bool& hasLower, bool& hasUpper, bool& hasNumber,
    bool& hasSpecial, bool& isLong, vector<int>& repeatedIndices, vector<int>& sequentialIndices) {
    string enhancment = "";

    string newPassword = userPassword;

    if (!hasLower) {
        int count = 0;
        for (int i = 0; i < newPassword.length(); i++) {
            if (isupper(newPassword[i])) {
                count++;
            }
        }
        if (count > 0) {
            int toggleChar = rand() % count;
            for (int i = 0; i < newPassword.length(); i++) {
                if (isupper(newPassword[i])) {
                    if (toggleChar == 0) {
                        newPassword[i] = tolower(newPassword[i]);
                        break;
                    }
                    toggleChar--;
                }
            }
            hasLower = true;
        }
    }

    if (!hasUpper) {
        int count = 0;
        for (int i = 0; i < newPassword.length(); i++) {
            if (islower(newPassword[i])) {
                count++;
            }
        }
        if (count > 0) {
            int toggleChar = rand() % count;
            for (int i = 0; i < newPassword.length(); i++) {
                if (islower(newPassword[i])) {
                    if (toggleChar == 0) {
                        newPassword[i] = toupper(newPassword[i]);
                        break;
                    }
                    toggleChar--;
                }
            }
            hasUpper = true;
        }
    }

    if (!hasLower) enhancment += 'a' + (rand() % 26);
    if (!hasUpper) enhancment += 'A' + (rand() % 26);
    if (!hasNumber) enhancment += '0' + (rand() % 10);
    if (!hasSpecial) enhancment += specialChar[rand() % 11];

    while (userPassword.size() + enhancment.size() < 12) {
        enhancment += 'a' + (rand() % 26);
        enhancment += 'A' + (rand() % 26);
        enhancment += '0' + (rand() % 10);
        enhancment += specialChar[rand() % 11];
    }

    shufflePassword(enhancment);
    newPassword += enhancment;

    for (int repeatedIndex : repeatedIndices) {
        if (repeatedIndex >= 0 && repeatedIndex < static_cast<int>(newPassword.size())) {
            char& charToReplace = newPassword[repeatedIndex];
            char replacementChar;

            if (islower(charToReplace)) {
                do {
                    replacementChar = 'a' + (rand() % 26);
                } while (replacementChar == charToReplace);
            }
            else if (isupper(charToReplace)) {
                do {
                    replacementChar = 'A' + (rand() % 26);
                } while (replacementChar == charToReplace);
            }
            else if (isdigit(charToReplace)) {
                do {
                    replacementChar = '0' + (rand() % 10);
                } while (replacementChar == charToReplace);
            }
            else {
                do {
                    replacementChar = specialChar[rand() % 11];
                } while (replacementChar == charToReplace);
            }

            charToReplace = replacementChar;
        }
    }

    for (int sequentialIndex : sequentialIndices) {
        if (sequentialIndex >= 0 && sequentialIndex < static_cast<int>(newPassword.size())) {
            char& charToReplace = newPassword[sequentialIndex];
            char replacementChar;

            if (islower(charToReplace)) {
                do {
                    replacementChar = 'a' + (rand() % 26);
                } while (replacementChar == charToReplace);
            }
            else if (isupper(charToReplace)) {
                do {
                    replacementChar = 'A' + (rand() % 26);
                } while (replacementChar == charToReplace);
            }
            else if (isdigit(charToReplace)) {
                do {
                    replacementChar = '0' + (rand() % 10);
                } while (replacementChar == charToReplace);
            }
            else {
                do {
                    replacementChar = specialChar[rand() % 11];
                } while (replacementChar == charToReplace);
            }

            charToReplace = replacementChar;
        }
    }

    return newPassword;
}