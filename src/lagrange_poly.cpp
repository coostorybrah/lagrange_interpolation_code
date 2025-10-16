// LIBRARIES
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>

// NAMESPACE
using namespace std;

// GLOBAL VALUES
#define PRECISION 4                  // Decimal precision
#define FIRST_COLLUMN_WIDTH 8        // MUST BE DIVISIBLE BY 2
#define SECOND_COLLUMN_WIDTH 48      // MUST BE DIVISIBLE BY 4
#define THIRD_COLLUMN_WIDTH 32       // MUST BE DIVISIBLE BY 4
#define TOTAL_WIDTH 92               // TOTAL_COLLUMN_WIDTH + (COLLUMN_COUNT + 1)

string const DATA_FILE = "data.csv"; // csv file name

// GLOBAL FUNCTIONS
int dataCheck(vector<double> &X, vector<double> &Y, string testID);       // Check test case data
vector<double> lagrangeCoefficient(vector<double> &X, vector<double> &Y); // Find corresponding polynomial coefficients
string lagrangePolynomialForm(vector<double> &coefficients);              // Convert to readable polynomial form

// CLASSES
class TestCase
{
    public:
    string testID, testFunction;
    vector<double> X;
    vector<double> Y;
    TestCase()
    {
        testID = "", testFunction = "";
    }

    TestCase(string ID, double x, double y, string function)
    {
        testID = ID;
        X.push_back(x);
        Y.push_back(y);
        testFunction = function;
    }

    string runTestCase()
    {
        int status = dataCheck(X, Y, testID);
        if (status)
        {
            return "!ERROR-" + to_string(status);
        }
        vector<double> coefficients = lagrangeCoefficient(X, Y);
        return lagrangePolynomialForm(coefficients);
    }
};

// ----------------------------------------- MAIN PROGRAM ---------------------------------------------
int main()
{
    // Init test case vector and data file pointer
    vector<TestCase> testCases;
    ifstream TestData(DATA_FILE);
    if (!TestData.is_open()) 
    {
        TestData.open("../" + DATA_FILE);
        if (!TestData.is_open())
        {
            cerr << "Khong mo duoc file!" << endl;
            return 1;
        }
    }

    // Read csv file
    string line;
    getline(TestData, line); // Skip header

    while (getline(TestData, line))
    {
        stringstream ss(line);
        string ID = "", function = "", x_str = "", y_str = "";

        getline(ss, ID, ',');
        getline(ss, x_str, ',');
        getline(ss, y_str, ',');
        getline(ss, function, '\n');

        if (testCases.empty() || testCases[testCases.size() - 1].testID.compare(ID))
        {
            testCases.push_back(TestCase(ID, stod(x_str), stod(y_str), function));
            continue;
        }
        testCases[testCases.size() - 1].X.push_back(stod(x_str));
        testCases[testCases.size() - 1].Y.push_back(stod(y_str));
    }

    // UI
    char option = ' ';
    while (option != 'q' && option != 'Q')
    {
        cout << "+---------------------------- CHUONG TRINH NOI SUY LAGRANGE ----------------------------+" << "\n"
             << "[1] Chay 1 test case." << "\n"
             << "[2] Chay toan bo test case." << "\n"
             << "Nhap 'q' de thoat chuong trinh..." << "\n"
             << "-> Input: ";
        cin >> option;
        cin.ignore();

        if (option == '1')
        {
            // RUN ONE SELECTED TEST CASE
            // Show user available test cases
            int selectedTestCaseIndex = -1;
            string selectedTestCaseID = "";
            cout << "+---------------- AVAILABLE TEST CASES ----------------+" << "\n";
            cout << "+" << left << setw(FIRST_COLLUMN_WIDTH + THIRD_COLLUMN_WIDTH + 2) << setfill('-') << right << "+" << "\n" << setfill(' ');
            cout << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << "ID" 
            << "| " << left << setw(THIRD_COLLUMN_WIDTH / 4) << "" << left << setw(THIRD_COLLUMN_WIDTH * 3 / 4) << "Test Function"
            << "|\n";
            for (unsigned int i = 0; i < testCases.size(); i++)
            {
                cout << "|" << left << setw(FIRST_COLLUMN_WIDTH + THIRD_COLLUMN_WIDTH + 2) << setfill('-') << right << "|" << "\n" << setfill(' ')
                << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << testCases[i].testID
                << "| " << left << setw(THIRD_COLLUMN_WIDTH) << testCases[i].testFunction 
                << "|" << "\n";
            }
            cout << "+" << left << setw(FIRST_COLLUMN_WIDTH + THIRD_COLLUMN_WIDTH + 2) << setfill('-') << right << "+" << "\n" << setfill(' ');
            
            cout << "-> Input (Nhap Test ID): ";

            // Find test case
            cin >> selectedTestCaseID;
            cin.ignore();

            for (unsigned int i = 0; i < testCases.size(); i++)
            {
                if (!selectedTestCaseID.compare(testCases[i].testID))
                {
                    selectedTestCaseIndex = i;
                }
            }

            if (selectedTestCaseIndex == -1)
            {
                cout << "(!) ID khong hop le." << "\n";
                continue;
            }

            cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
            cout << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << "ID" 
                 << "| " << left << setw(SECOND_COLLUMN_WIDTH / 4) << "" << left << setw(SECOND_COLLUMN_WIDTH * 3 / 4) << "Lagrange Polynomial" 
                 << "| " << left << setw(THIRD_COLLUMN_WIDTH / 4) << "" << left << setw(THIRD_COLLUMN_WIDTH * 3 / 4) << "Actual Function" 
                 << "|" << "\n";
    
            cout << "|" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "|" << "\n" << setfill(' ');
            cout << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << testCases[selectedTestCaseIndex].testID 
                 << "| " << left << setw(SECOND_COLLUMN_WIDTH) << testCases[selectedTestCaseIndex].runTestCase()
                 << "| " << left << setw(THIRD_COLLUMN_WIDTH) << testCases[selectedTestCaseIndex].testFunction
                 << "|" << "\n";
            cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
            selectedTestCaseIndex = -1;
            cout << "\n";
            continue;
        }

        if (option == '2')
        {
            // RUN ALL TEST CASES 
            cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
            cout << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << "ID" 
                << "| " << left << setw(SECOND_COLLUMN_WIDTH / 4) << "" << left << setw(SECOND_COLLUMN_WIDTH * 3 / 4) << "Lagrange Polynomial" 
                << "| " << left << setw(THIRD_COLLUMN_WIDTH / 4) << "" << left << setw(THIRD_COLLUMN_WIDTH * 3 / 4) << "Actual Function" 
                << "|" << "\n";
            for (unsigned int i = 0; i < testCases.size(); i++)
            {
                cout << "|" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "|" << "\n" << setfill(' ');
                cout << "|" << left << setw(FIRST_COLLUMN_WIDTH / 2 - 1) << "" << left << setw(FIRST_COLLUMN_WIDTH / 2) << testCases[i].testID 
                    << "| " << left << setw(SECOND_COLLUMN_WIDTH) << testCases[i].runTestCase()
                    << "| " << left << setw(THIRD_COLLUMN_WIDTH) << testCases[i].testFunction 
                    << "|" << "\n";
            }
            cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
            cout << "\n";
            continue;
        }
        cout << "\n";
    }
}
// ---------------------------------------------------------------------------------------------------------

// Check test case data
int dataCheck(vector<double> &X, vector<double> &Y, string testID)
{
    if (X.empty() || Y.empty())
    {
        return 1;
    }
    if (X.size() != Y.size())
    {
        return 2;
    }

    for (unsigned int i = 0; i < X.size(); i++)
    {
        for (unsigned int j = i + 1; j < X.size(); j++)
        {
            if (X[i] == X[j])
            {
                return 3;
            }
        }
    }
    return 0;
}

// Find the coefficients of each polynomial term
vector<double> lagrangeCoefficient(vector<double> &X, vector<double> &Y)
{
    int n = X.size();
    vector<double> result(n, 0.0);

    for (int i = 0; i < n; i++) 
    {
        vector<double> term = {1.0};
        double wi = 1.0;

        for (int j = 0; j < n; j++) 
        {
            if (i != j)
            {
                vector<double> newTerm(term.size() + 1, 0.0);
                for (unsigned int k = 0; k < term.size(); k++) 
                {
                    newTerm[k] -= term[k] * X[j];
                    newTerm[k + 1] += term[k];
                }
                term = newTerm;
                wi *= (X[i] - X[j]);
            }
        }

        // Scale each term with Y[i] / wi 
        for (unsigned int h = 0; h < term.size(); h++)
        {
            result[h] += term[h] * Y[i] / wi;
        }
    }

    // Set float precision
    for (unsigned int i = 0; i < result.size(); i++)
    {
        result[i] = round(result[i] * pow(10, PRECISION)) / pow(10,PRECISION);
    }
    return result;
}

// Convert to readable polynomial form: P(x) = a0 + (a1)x^1 + (a2)x^2 +...
string lagrangePolynomialForm(vector<double> &coefficients)
{
    ostringstream polynomial;

    // a0
    if (coefficients[0] != 0.0)
    {
        polynomial << coefficients[0] << " ";
    }

    // (a1)x^1 + (a2)x^2 +...
    for (unsigned int i = 1; i < coefficients.size(); i++)
    {   
        if (coefficients[i] == 0)
        {
            continue;
        }

        if (polynomial.str().empty()) // In case a0 == 0
        {
            if (coefficients[i] == 1)
            {
                polynomial << "x^" << i << " ";
                continue;
            }
            polynomial << coefficients[i] << "x^" << i << " ";
            continue;
        }

        if (coefficients[i] != 1)
        {
            polynomial << "+ " << "(" << coefficients[i] << ")" << "x^" << i << " ";
            continue;
        }
        polynomial << "+ " << "x^" << i << " ";
    }
    
    ostringstream result;
    result << "P(x) = " << polynomial.str();

    return result.str();
}
