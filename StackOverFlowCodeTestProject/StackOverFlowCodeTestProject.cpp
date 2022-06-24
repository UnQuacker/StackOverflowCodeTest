#include <iostream>
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
    BOOL fSuccess = false;
    HANDLE hDisk = ::CreateFile(R"(\\.\D:)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDisk == INVALID_HANDLE_VALUE) throw win_error();

    //DWORD  hDiskMoved = SetFilePointer(hDisk, 512, NULL, FILE_CURRENT);

    BYTE buf[512];
    /*char newBuf[468] = "Garbage Data";
    BYTE garbageData = 16;
    DWORD dwDataToWrite = strlen(newBuf) * sizeof(newBuf);
    DWORD dwDataWritten = 0;*/
    DWORD dwRead;
    unsigned char  chBuffer[16];
    long SectorActual = 3;
    long PosicionInicio = SectorActual * 512;

    for (int i = 0; i < 16; i++) // Garbage values to be written
    {
        chBuffer[i] = i % 16;
        if ((i / 16) % 2 == 0)
        {
            chBuffer[i] = '0';
        }
    }
    DWORD dwPtr = SetFilePointer(hDisk,
        PosicionInicio,
        NULL,
        FILE_BEGIN);
    DWORD dwBytesWritten = 0;

    //BOOL writeTest = WriteFile(hDisk, (void*)garbageData, dwDataToWrite, &dwDataWritten, NULL);
    fSuccess = WriteFile(hDisk, chBuffer, PosicionInicio, &dwBytesWritten, NULL);
    if (!fSuccess) {
        cout << "Fail" << endl;;
        cout <<"Last error code: " << GetLastError()<< endl;
    }
    if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error();
    ::CloseHandle(hDisk);
    ofstream out(R"(\\.\D:\\output.txt)");
    for (int i = 0; i < 512; i++)
    {
        char f[3];  _itoa(buf[i], f, 16);
        out << i << "\t" << f << "\t" << buf[i] << '\n';
        cout << buf[i];
    }
    out.close();
    //system(R"(\\.\D:\\2.txt)");
}

