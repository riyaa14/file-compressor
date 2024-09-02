#include <iostream>

using namespace std;

void compress();
void decompress();

int main()
{
    char mode;
    cout << "Type 'c' for compression and 'd' for decomprssion" << endl;
    cin >> mode;

    if (mode != 'c' and mode != 'd')
        cout << "Invalid mode. Type 'c' for compression and 'd' for decomprssion" << endl;

    else if (mode == 'c')
        compress();
    else
        decompress();
}