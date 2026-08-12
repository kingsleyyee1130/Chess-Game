#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Save 1D vector   ******
std::string save1D(const std::vector<int>& arr) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        ss << arr[i];
        if (i < arr.size() - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

// Save 2D vector   ******
template<size_t N>
std::string save2D(const std::vector<int>(&arr)[N]) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < N; ++i) {
        ss << save1D(arr[i]);
        if (i < N - 1) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

// Parse a 1D array from JSON string: [1, 2, 3, 4]  
std::vector<int> parse1DArray(const std::string& str, size_t& pos) {
    std::vector<int> result;
    pos++; // Skip '['

    while (pos < str.length() && str[pos] != ']') {
        // Skip whitespace and commas
        while (pos < str.length() && (str[pos] == ' ' || str[pos] == ',' || str[pos] == '\n' || str[pos] == '\t')) {
            pos++;
        }

        if (pos < str.length() && str[pos] == ']') break;

        // Parse number
        int num = 0;
        bool negative = false;
        if (str[pos] == '-') {
            negative = true;
            pos++;
        }
        while (pos < str.length() && isdigit(str[pos])) {
            num = num * 10 + (str[pos] - '0');
            pos++;
        }
        if (negative) num = -num;
        result.push_back(num);
    }

    if (pos < str.length() && str[pos] == ']') {
        pos++; // Skip ']'
    }

    return result;
}


// Parse 2D array from JSON string: [[1,2],[3,4]]   
std::vector<std::vector<int>> parse2DArray(const std::string& str, size_t& pos) {
    std::vector<std::vector<int>> result;
    pos++; // Skip '['

    while (pos < str.length() && str[pos] != ']') {
        // Skip whitespace and commas
        while (pos < str.length() && (str[pos] == ' ' || str[pos] == ',' || str[pos] == '\n' || str[pos] == '\t')) {
            pos++;
        }

        if (pos < str.length() && str[pos] == ']') break;

        if (str[pos] == '[') {
            // Parse inner array
            std::vector<int> inner = parse1DArray(str, pos);
            result.push_back(inner);
        }
        else {
            pos++; // Skip unexpected character
        }
    }

    if (pos < str.length() && str[pos] == ']') {
        pos++; // Skip ']'
    }

    return result;
}

// Read JSON from file and extract the data array   
std::vector<std::vector<int>> readJSON(const std::string& filename, const std::string& dataKey) {
    std::ifstream file(filename);
    std::string content;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return {};
    }

    // Read entire file
    std::string line;
    while (std::getline(file, line)) {
        content += line;
    }
    file.close();

    // Find the data key
    std::string searchKey = "\"" + dataKey + "\"";
    size_t pos = content.find(searchKey);

    if (pos == std::string::npos) {
        std::cerr << "Error: '" << dataKey << "' not found!" << std::endl;
        return {};
    }

    // Find array start
    pos = content.find('[', pos);
    if (pos == std::string::npos) {
        std::cerr << "Error: Array not found for '" << dataKey << "'" << std::endl;
        return {};
    }

    // Find the start of the array
    pos = content.find('[', pos);
    if (pos == std::string::npos) {
        std::cerr << "Error: Array not found" << std::endl;
        return {};
    }

    // Parse the 2D array
    return parse2DArray(content, pos);
}

int main() {
    char filename;
    std::cout << "Enter filename : ";
    std::cin >> filename;
    int i = 1;
    char just_for_loop;
    std::cin >> just_for_loop;
    while (std::cin)
    {
        std::string move = std::to_string(i);

        // Array of 8 vectors
        std::vector<int> data[8] = {
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i },
            { i, i, i, i, i, i, i, i }
        };
         
        // the things that will save in file
        std::string jsonString = "\n\n{ \"move" + move + "\": " + save2D(data) + " }";
        
        // Saving
        std::ofstream file("data_manual.json", std::ios::out | std::ios::app);
        if (file.is_open()) {
           file << jsonString << "\n";
           file.close();
           std::cout << "JSON saved: " << jsonString << std::endl;
        }

        // Ask for Reading  **when undo or call history**
        char wantread;
        std::cout << "Do u want to undo or view history (u or h)";
        std::cin >> wantread;
        switch (wantread)
        {
        case 'u': {
            std::string choice = "move"+move;
            // Reading
            std::vector<std::vector<int>> dataread = readJSON("data_manual.json", choice);

            if (dataread.empty()) {
                std::cout << "No data read or file empty!" << std::endl;
                return 1;
            }

            // Display the data
            std::cout << "Data read from file:\n";
            for (size_t i = 0; i < dataread.size(); ++i) {
                std::cout << "Row " << i << ": ";
                for (size_t j = 0; j < dataread[i].size(); ++j) {
                    std::cout << dataread[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }break;
        case 'h': {
            for (int j = 1;j <= i;j++) {
                std::string move = std::to_string(j);
                std::string choice = "move" + move;
                // Reading
                std::vector<std::vector<int>> dataread = readJSON("data_manual.json", choice);

                if (dataread.empty()) {
                    std::cout << "No data read or file empty!" << std::endl;
                    return 1;
                }

                // Display the data
                std::cout << "Data read from file:\n";
                for (size_t i = 0; i < dataread.size(); ++i) {
                    std::cout << "Row " << i << ": ";
                    for (size_t j = 0; j < dataread[i].size(); ++j) {
                        std::cout << dataread[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
            }
        }break;
        default:return 0;
        }
        i++;
    }
    return 0;
}
