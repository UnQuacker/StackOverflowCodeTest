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
// что по задумке должен программа должны была делать:
// открыть внешний носитель -> создать временный файл -> заполнить данными -> считать его байты -> удалить временный файл -> переместить указатель записи внешнего носителя (hDisk) -> записать туда сохранённый байты

// но программа выдаёт ошибку "Unhandled exception at 0x75C4C3A2 in StackOverFlowCodeTestProject.exe: Microsoft C++ exception: int at memory location 0x009CF2F4." на 47 строке
{
    BOOL fSuccess = false;

    //открытие диска
    HANDLE hDisk = ::CreateFile(R"(\\.\D:)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDisk == INVALID_HANDLE_VALUE) throw win_error();

    BYTE buf[512];
    BYTE buffer[1024];
    DWORD dwRead;
    DWORD dwRead1;
    char chBuffer[16]="abcdefghijklmno";
    long SectorActual = 100;  // номер сектора
    long PosicionInicio = SectorActual * 512;
    long SectorMove = SectorActual * 1024;

    long numberOfBytes = sizeof(chBuffer);

    //создание временного файла
    HANDLE hFile = ::CreateFile(R"(\\.\D:\\WriteHere.txt)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) { throw win_error(); return 0; };

    DWORD dwBytesWritten = 0;
    DWORD dwBytesWritten1 = 0;

    //запись данных в временный файл
    fSuccess = WriteFile(hFile, chBuffer, numberOfBytes, &dwBytesWritten, NULL);

    //попытка прочитать байты файла, тут и выходит ошибка
    try {
        if (!::ReadFile(hFile, buffer, sizeof(buffer), &dwRead1, NULL)) { cout << GetLastError()<<endl; return 0; };
    }

    catch (exception e) {
        cout << GetLastError();
    }

    ::CloseHandle(hFile);

    //удаление временного файла
    //fSuccess = DeleteFile("D:\\WriteHere.txt"); 
    /*if (!fSuccess)
    {
        printf("DeleteFile failed (%d)\n", GetLastError());
        return (6);
    }*/

    //двигаем указатель диска
    DWORD dwPtr = SetFilePointer(hDisk,
        SectorMove,
        NULL,
        FILE_BEGIN);

    //запись сохранённых данных на диск
    fSuccess = ::WriteFile(hDisk, buffer, sizeof(buffer), &dwBytesWritten1, NULL);
    if (!fSuccess) {
        cout << "Fail" << endl;;
        cout << "Last error code: " << GetLastError() << endl;
    }

    //чтение байтов диска
    if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) { throw win_error(); return 0; };
    ::CloseHandle(hDisk);

    ofstream out(R"(\\.\D:\\output.txt)");

    for (int i = 0; i < 512; i++)
    {
        char f[3];  _itoa(buf[i], f, 16);
        out << i << "\t" << f << "\t" << buf[i] << '\n';
        cout << buf[i];
    }

    out.close();
}

