#include <iostream>
using namespace std;

int main()
{
    char option = ' ';
    while (option != 'q' && option != 'Q')
    {
        cout << "--------------------------CHUONG TRINH NOI SUY LAGRANGE--------------------------" << "\n"
             << "1. Chay 1 test case." << "\n"
             << "2. Chay toan bo test case." << "\n"
             << "Nhap q de thoat chuong trinh..." << "\n"
             << "Input: ";

        cin >> option;
        switch (option)
        {
            case '1':
                cout << "1\n";
                break;
            case '2':
                cout << "2\n";
                break;
            case 'q':
                break;
        }
    }
}