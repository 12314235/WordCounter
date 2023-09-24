#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser
{
public:
    vector<string> ParseFilenames(int argc, vector<string> argv) {
        vector<string> input_files_names;
        for (int i = 1; i < argc; i++)
        {
            if (argv[i].size() < 4) { continue; }
            if (argv[i][0] != '-') { input_files_names.push_back(argv[i]); }
        }
        return input_files_names;
    }

    vector<string> ParseOptions(int argc, vector<string> argv) {
        vector<string> options;
        for (int i = 1; i < argc; i++)
        {
            if (argv[i][0] == '-' && argv[i][1] != '-')
            {
                for (int z = 1; z < argv[i].size(); z++) {
                    options.push_back("-" + string(1, argv[i][z]));
                }
            }
            if (argv[i].substr(0, 2) == "--") options.push_back(argv[i]);
        }
        return options;
    }
};

size_t LinesCount(fstream& file, string& path) {
    file.open(path, ios_base::in, ios_base::binary);
    char ch;
    if (!file.get(ch))
    {
        file.close();
        return 0;
    }

    int counter = 1;
    do
    {
        if (ch == '\n') counter++;
    } while (file.get(ch));

    file.close();
    return counter;
}

size_t WordsCount(fstream& file, string& path) {
    file.open(path, ios_base::in, ios_base::binary);
    int counter = 0;
    char ch;
    bool is_prev_space = false;

    if (!file.get(ch))
    {
        file.close();
        return 0;
    }

    file.seekg(0, ios::beg);

    while (file.get(ch))
    {
        if (isspace(ch) && !is_prev_space)
        {
            is_prev_space = true;
        }
        else if (!isspace(ch) && is_prev_space) {
            is_prev_space = false;
            counter++;
        }
    }
    if (!is_prev_space) counter++;
    file.close();
    return counter;
}

size_t CharsCount(fstream& file, string& path) {
    file.open(path, ios_base::in, ios_base::binary);
    int counter = 0;
    char ch;
    while (file.get(ch))
    {
        if (isprint(ch)) counter++;
    }
    file.close();
    return counter;
}

size_t SizeInBytes(fstream& file, string& path) {
    file.open(path, ios_base::in, ios_base::binary);
    file.seekg(0, ios::end);
    int size = file.tellg();
    file.close();
    return size;
}

int main(int argc, char* argv[]) {
    fstream file;
    vector <string> argv_str;

    for (int i = 0; i < argc; i++) {
        argv_str.push_back(argv[i]);
    }

    Parser parser;
    vector<string> filenames = parser.ParseFilenames(argc, argv_str);
    vector<string> options = parser.ParseOptions(argc, argv_str);

    for (int i = 0; i < filenames.size(); i++) {
        string filename = filenames[i];

        if (options.size() == 0) {
            cout << LinesCount(file, filename) << ' '
                << WordsCount(file, filename) << ' '
                << SizeInBytes(file, filename) << ' '
                << filename << ' ';
        }

        for (int z = 0; z < options.size(); z++) {
            if (options[z] == "--lines" || options[z] == "-l") cout << LinesCount(file, filename) << ' ';
            else if (options[z] == "--words" || options[z] == "-w") cout << WordsCount(file, filename) << ' ';
            else if (options[z] == "--chars" || options[z] == "-m") cout << CharsCount(file, filename) << ' ';
            else if (options[z] == "--bytes" || options[z] == "-c") cout << SizeInBytes(file, filename) << ' ';
        }

        cout << filename << ' ';
    }
}