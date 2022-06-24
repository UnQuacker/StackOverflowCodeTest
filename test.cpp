#include <fstream>
#include <windows.h>
using namespace std;

int win_error()
{
    cout << "error" << endl;
    return 0;
}

int main()
{
    HANDLE hDisk = ::CreateFile("\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDisk == INVALID_HANDLE_VALUE) throw win_error();

    BYTE buf[512];
    DWORD dwRead;
    if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error();
    ::CloseHandle(hDisk);

    ofstream out("1.txt");
    for (int i = 0; i < 512; i++)
    {
        char f[3];  itoa(buf[i], f, 16);
        out << i << "\t" << f << "\t" << buf[i] << '\n';
        cout << buf[i];
    }
    out.close();
    system("1.txt");
    //cin.get();
}