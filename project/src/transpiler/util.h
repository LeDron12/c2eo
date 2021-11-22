#ifndef __UTIL__
#define __UTIL__

// Заголовочный файл с описанием вспомогательных утилит
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Запись строки символов в указанный файл
void str2file(std::string str, std::string fileName);

// Генерации строки из вектора строк через разделитель.
std::string strjoin(std::vector<std::string> vec,std::string sep = " ")

// Чтение из файла в вектор строк
void file2vector(std::string fileName, std::vector<std::string> &text);

// Формирование текста с глобальными объектами
// Пока формируется только для одной единицы компиляции
// В дальнейшем нужно будет собирать множество разных файлов с одинаковыми расширениями.
void createGlobal(std::vector<std::string> &text, std::string filename);

// Формирование текста со статическими переменными
// Генерируется файл filename.eo
void createStatic(std::vector<std::string> &text, std::string filename);



// Запись сформированного файла с глобальными объектами
void text2file(std::vector<std::string> &text, std::string fileName);

#endif // __UTIL__
