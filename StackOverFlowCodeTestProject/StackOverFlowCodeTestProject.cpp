#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
using namespace std;

int win_error()
{
    cout << "error" << endl;
    return 0;
}

int main()
{
    BOOL fSuccess = false;

    //�������� �����
    HANDLE hDisk = ::CreateFile(R"(\\.\D:)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDisk == INVALID_HANDLE_VALUE) { throw win_error(); };

    BYTE buf[512];
    //BYTE buffer[512];
    DWORD dwRead;
    //DWORD dwRead1;
    char chBuffer[512]="ABCDEFGHIJKlMNO"; 
    char chBuffer1[512] = "abcdefghijklmno";
    char chBuffer2[16] = "abcdefghijklmno";
    long SectorActual = 125;  // ����� �������
    long PosicionInicio = SectorActual * 512;
    long SectorMove = SectorActual * 512;
    BOOL size = true;

    long numberOfBytes = sizeof(chBuffer);
    int numberOfTries = 0;

    DWORD dwBytesWritten = 0;
    DWORD dwBytesWritten1 = 0;
    DWORD dwBytesWritten2 = 0;
    DWORD dwBytesWritten3 = 0;

    //������� ��������� �����
    DWORD dwPtr = SetFilePointer(hDisk,
        SectorMove,
        NULL,
        FILE_BEGIN);

    //������ ������ �� ����
    fSuccess = ::WriteFile(hDisk, chBuffer, sizeof(chBuffer), &dwBytesWritten1, NULL);
    DWORD dwPtr2 = SetFilePointer(hDisk,
        SectorMove,
        NULL,
        FILE_BEGIN);

    //������ ������ �����
    if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) { cout << "Error"; };
    ::CloseHandle(hDisk);

    ofstream out(R"(\\.\C:\\Users\\Alser\\Desktop\\fileoutput.txt)");

    for (int i = 0; i < 512; i++)
    {
        char f[3];  _itoa(buf[i], f, 16);
        out << i << "\t" << f << "\t" << buf[i] << '\n';
        cout <<buf[i];
    }

    out.close();
}

