//
// Created by Aaron Zhang on 2022/10/6.
//
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/iostreams/device/mapped_file.hpp>
#include <assert.h>
#include <vector>

//remove empty automatically
#define REMOVE_EMPTY_LINE 0

// print the debug information
#define DEBUG 1

//test file path
const char* FILEPATH = "../test.txt";
const size_t LINESIZE = 64;

#define FUNCTION_CODE(func_sentence)\
	clock_t start,finish;\
	start=clock();\
	func_sentence;\
	finish=clock();\
	return double(finish-start)/CLOCKS_PER_SEC;


namespace READFILE {

    typedef long double LONGEST_FLOAT;
    template<typename T, typename Taug1>
    LONGEST_FLOAT gettime(T func, Taug1 argument1)
    {
        FUNCTION_CODE(func(argument1));
    }


    //cannot read empty line
    void m_ifstream_operator(int runNum) {
        size_t lineNum = 0;
        for (int i = 0; i < runNum; ++i)
        {
            std::string line;
            std::ifstream filestr(FILEPATH, std::ios::in);
            assert(filestr.is_open());
            lineNum = 0;
            while (filestr >> line)
            {
#if DEBUG
                std::cout << line << "\n";
                ++lineNum;
#endif
            }
            filestr.close();
        }
#if DEBUG
        std::cout << "number of line: "<< lineNum << "\n";
#endif

    }


    // read empty line
    void m_std_getline(int runNum) {
        size_t lineNum = 0;
        for (int i = 0; i < runNum; ++i)
        {
            std::string line;
            std::ifstream filestr;
            filestr.open(FILEPATH, std::ios::in);
            assert(filestr.is_open());
            lineNum = 0;
            while (std::getline(filestr, line))
            {
#if REMOVE_EMPTY_LINE
                // skip if file contains empty line
                if (!line.empty()) {
#endif

#if DEBUG
                    std::cout << line << "\n";
                    ++lineNum;
#endif

#if REMOVE_EMPTY_LINE
                }
#endif
            }
            filestr.close();
        }
#if DEBUG
        std::cout << "number of line: "<< lineNum << "\n";
#endif

    }



    void m_ifstream_getline(int runNum) {
        size_t lineNum = 0;
        for (int i = 0; i < runNum; ++i) {
            char line[LINESIZE];
            std::ifstream filestr;
            filestr.open(FILEPATH, std::ios::in);
            assert(filestr.is_open());
            lineNum = 0;
            while (filestr.getline(line, LINESIZE))
            {

#if REMOVE_EMPTY_LINE
                // skip if file contains empty line
                if (strlen(line)) {
#endif

#if DEBUG
                std::cout << line << "\n";
                ++lineNum;
#endif

#if REMOVE_EMPTY_LINE
                }
#endif
            }
            filestr.close();
        }
#if DEBUG
        std::cout << "number of line: "<< lineNum << "\n";
#endif
    }


    void m_file_fgets(int runNum) {
        size_t lineNum = 0;

        for (int i = 0; i < runNum; ++i)
        {
            FILE* fp;
            char line[LINESIZE];
            fp = fopen(FILEPATH, "r");
            assert(fp);
            while (fgets(line, LINESIZE, fp) != NULL) {
                unsigned short end = strlen(line) - 1;

#if REMOVE_EMPTY_LINE
                // skip if file contains empty line
                if (line[0] == '\n') continue;
#endif
                //remove the \n in last char
                if(line[end] == '\n')
                    line[end] = '\0';
#if DEBUG
                std::cout << line << "\n";
                ++lineNum;
#endif
            }
            fclose(fp);
        }

#if DEBUG
        std::cout << "number of line: "<< lineNum << "\n";
#endif
    }


    void m_ifstream_read(int runNum){
        std::ifstream filestr;
        filestr.open(FILEPATH, std::ios::in);
        assert(filestr.is_open());

        // compute stream size
        std::istream::pos_type current_pos = filestr.tellg();
        filestr.seekg(0,std::istream::end);
        std::istream::pos_type end_pos = filestr.tellg();
        filestr.seekg(current_pos);
        std::streamoff len = end_pos - current_pos;

        char* str = (char*) malloc (sizeof(char)*len);
        filestr.read(str,len);

        auto right = str;
        auto left = right;
        auto end = right + len;

        size_t lineNum = 0;

        while (right && right != end)
            if ((right = static_cast<char *>(memchr(right, '\n', end - right)))) {
                unsigned int len= right - left;

#if REMOVE_EMPTY_LINE
                // skip if file contains empty line
                if (len == 0){
                    right++,left++;
                    continue;
                }
#endif

                //save the line except last line character
                char line[len];
                strncpy(line, left, len);
                right++, left = right;
#if DEBUG
                std::cout << line << '\n' ;
                ++lineNum;
#endif

            }else{
                char* lastLine = left;
#if DEBUG
                std::cout << lastLine << '\n' ;
                ++lineNum;
#endif
            }
        filestr.close();
        free (str);

#if DEBUG
        std::cout << "number of line: "<< lineNum << "\n";
#endif
    }


    void methodBoostMmap(int runNum) {

        size_t lineNum = 0;

        for (int i = 0; i < runNum; ++i) {
            boost::iostreams::mapped_file mmap(FILEPATH, boost::iostreams::mapped_file::readonly);
            assert(mmap.is_open());
            auto right = mmap.const_data();
            auto left = right;
            auto end = right + mmap.size();

            lineNum = 0;
            while (right && right != end)
                if ((right = static_cast<const char*>(memchr(right, '\n', end - right)))) {
                    unsigned int len= right - left;
                    // skip if file contsains empty line
                    if (len == 0) {
                        right++,left++;
                        continue;
                    }
                    char line[len];
                    strncpy(line, left, len);
                    right++;
                    left = right;
#if DEBUG
                    std::cout << line << '\n' ;
                    ++lineNum;
#endif
                }
            const char* lastLine = left;
#if DEBUG
            std::cout << lastLine << '\n' ;
            ++lineNum;
            std::cout << "number of line: "<< lineNum << "\n";
#endif

        }
    }

}



int main() {

    int runNum = 20;

    std::cout << "METHOD: m_ifstream_operator\n" << READFILE::gettime<void (*)(int)>(READFILE::m_ifstream_operator, runNum) / runNum << "(Spend time)\n";

    std::cout << "METHOD: m_std_getline\n" << READFILE::gettime<void (*)(int)>(READFILE::m_std_getline, runNum) / runNum << "(Spend time)\n";

    std::cout << "METHOD: m_ifstream_getline\n" << READFILE::gettime<void (*)(int)>(READFILE::m_ifstream_getline, runNum) / runNum << "(Spend time)\n";

    std::cout << "METHOD: m_file_fgets\n" << READFILE::gettime<void (*)(int)>(READFILE::m_file_fgets, runNum) / runNum << "(Spend time)\n";

    std::cout << "METHOD: methodBoostMmap\n" << READFILE::gettime<void(*)(int)>(READFILE::methodBoostMmap, runNum) / runNum << "(Spend time)\n";

    std::cout << "METHOD: m_ifstream_read\n" << READFILE::gettime<void(*)(int)>(READFILE::m_ifstream_read, runNum) / runNum << "(Spend time)\n";

}
