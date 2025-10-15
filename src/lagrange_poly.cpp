// LIBRARIES
#include <bits/stdc++.h>

// NAMESPACE
using namespace std;

// GLOBAL VALUES
#define data_file "data.csv" // Ten file csv
#define PRECISION 4          // Gia tri lam tron so thap phan
#define FIRST_COLLUMN_WIDTH 8 // setw()
#define SECOND_COLLUMN_WIDTH 40 // setw()
#define TOTAL_WIDTH 52 // setw()

// GLOBAL FUNCTIONS
int dataFileCheck(vector<double> &X, vector<double> &Y, string testID); // Kiem tra du lieu dau vao
vector<double> lagrangeCoefficient(vector<double> &X, vector<double> &Y); // Tim cac he so cua ham noi suy
string lagrangePolynomialForm(vector<double> &coefficients); // Xau chuoi

// CLASSES
class TestCase
{
    private:
    static int totalTestCases;

    public:
    string testID, testFunction;
    vector<double> X;
    vector<double> Y;
    TestCase()
    {
        testID = "", testFunction = "";
        totalTestCases++;
    }

    TestCase(string ID, double x, double y, string function)
    {
        testID = ID;
        X.push_back(x);
        Y.push_back(y);
        testFunction = function;
        totalTestCases++;
    }

    string runTestCase()
    {
        if (dataFileCheck(X, Y, testID))
        {
            return "ERROR!";
        }
        vector<double> coefficients = lagrangeCoefficient(X, Y);
        return lagrangePolynomialForm(coefficients);
    }

    static int getTotalTestCases()
    {
        return totalTestCases;
    }
};
int TestCase::totalTestCases = 0; // Init tong so test case

// ----------------------------------------- CHAY CHUONG TRINH ---------------------------------------------
int main()
{
    // Init du lieu chung
    vector<TestCase> testCases;
    ifstream TestData(data_file);
    if (!TestData)
    {
        cerr << "Khong mo duoc file du lieu!";
        return 1;
    }

    // Doc file csv
    string line;
    getline(TestData, line); // Skip header

    while (getline(TestData, line))
    {
        // Doc tung dong du lieu
        stringstream ss(line);
        string ID = "", function = "", x_str = "", y_str = "";

        // Luu du lieu tuong ung voi tung cot
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

    // RUN ALL TEST CASES 
    cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
    cout << "|" << left << setw(FIRST_COLLUMN_WIDTH) << "   ID" << "|  " << left << setw(SECOND_COLLUMN_WIDTH) << "         Lagrange Polynomial" << "|" << "\n";
    for (int i = 0; i < TestCase::getTotalTestCases(); i++)
    {
        cout << "|" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "|" << "\n" << setfill(' ');
        cout << "|   " << left << setw(FIRST_COLLUMN_WIDTH - 3) << testCases[i].testID << "|  " << left << setw(SECOND_COLLUMN_WIDTH) << testCases[i].runTestCase() << "|" << "\n";
    }
    cout << "+" << left << setw(TOTAL_WIDTH) << setfill('-') << right << "+" << "\n" << setfill(' ');
}
// ---------------------------------------------------------------------------------------------------------

// Kiem tra du lieu dau vao
int dataFileCheck(vector<double> &X, vector<double> &Y, string testID)
{
    if (X.empty() || Y.empty())
    {
        cerr << "ERROR: Du lieu test case " << testID << " loi!";
        return 1;
    }
    if (X.size() != Y.size())
    {
        cerr << "ERROR: So luong toa do X va Y trong test case " << testID << " khong trung nhau!";
        return 2;
    }

    for (int i = 0; i < X.size(); i++)
    {
        for (int j = i + 1; j < X.size(); j++)
        {
            if (X[i] == X[j])
            {
                cerr << "ERROR: Toa do X trong test case " << testID << " co gia tri bi lap lai!";
                return 3;
            }
        }
    }
    return 0;
}

// Tim cac he so cua ham noi suy
vector<double> lagrangeCoefficient(vector<double> &X, vector<double> &Y)
{
    int n = X.size();
    vector<double> result(n, 0.0);

    for (int i = 0; i < n; i++) 
    {
        vector<double> term = {1.0};
        double wi_prime = 1.0; // Wi'

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
                wi_prime *= (X[i] - X[j]);
            }
        }

        // Scale by Y[i] / wi_prime
        for (unsigned int k = 0; k < term.size(); k++)
        {
            result[k] += term[k] * Y[i] / wi_prime;
            result[k] = round(result[k] * pow(10, PRECISION)) / pow(10,PRECISION);
        }
    }

    return result;
}

// Xau chuoi ham noi suy ve dang: P(x) = a0 + (a1)x^1 + (a2)x^2 +...
string lagrangePolynomialForm(vector<double> &coefficients)
{
    ostringstream polynomial;

    // a0
    if (coefficients[0] != 0.0)
    {
        polynomial << coefficients[0] << " ";
    }

    // (a1)x^1 + (a2)x^2 +...
    for (int i = 1; i < coefficients.size(); i++)
    {   
        if (polynomial.str().empty())
        {
            polynomial  << coefficients[i] << "x^" << i << " ";
            continue;
        }

        if (coefficients[i] == 1)
        {
            polynomial << "+ " << "x^" << i << " ";
            continue;
        }
        else if (coefficients[i] != 0)
        {
            polynomial << "+ " << "(" << coefficients[i] << ")" << "x^" << i << " ";
        }
    }
    
    ostringstream result;
    result << "P(x) = " << polynomial.str();

    return result.str();
}

