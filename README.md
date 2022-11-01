## A simple test for reading line in txt file

IMPORTANT:   
&#160;&#160;&#160;&#160;Read file *line by line*!  
TEST FILE:  
&#160;&#160;&#160;&#160;A txt file(708.89M, 20956536 lines)  
ENVIRONMENT:  
&#160;&#160;&#160;&#160;Apple M1 Pro with 8 CPU, 14 GPU, 32G and 1T ssd

The following methods I test and summarize:

| Method                  | Time(s)   | Read Empty Line | Remove '\n' | Fix Size |
|-------------------------|-----------|-----------------|-------------|----------|
| std::ifstream <<        | 6.24657   | &cross;         | &check;     | &cross;  |
| std::getline()          | 2.33548   | &check;         | &check;     | &cross;  |
| std::ifstream.getline() | 2.18787   | &check;         | &check;     | &cross;  |
| fgets()                 | 0.844756  | &check;         | &cross;     | &check;  |
| boost::mapped_file()    | 0.354146  | &check;         | &cross;     | &cross;  |
| std::ifstream.read()    | 0.0262306 | &check;         | &cross;     | &cross;  |

*Time: Read 20 times *test.txt* file.  
*Read Empty Line: Empty Line would be read or not?  
*Remove '\n': New line character'\n' would be removed automatically or not?  
*Fix Size: Has the limit size of one line?
