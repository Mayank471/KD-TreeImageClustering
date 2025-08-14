#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif
// nlohmann::json single-header library placed as json.hpp in repo
#include "json.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using json = nlohmann::json;

// Function to calculate the color histogram of an image
vector<double> calculateColorHistogram(const string &imagePath)
{
    int width, height, channels;
    unsigned char *img = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    if (!img)
    {
        cerr << "Error loading image: " << stbi_failure_reason() << " for " << imagePath << endl;
        return vector<double>(3 * 256, 0); // Return zero histogram
    }

    int histogram[3][256] = {0};

    for (int i = 0; i < width * height; i++)
    {
        int r = img[i * channels + 0]; 
        int g = img[i * channels + 1];
        int b = img[i * channels + 2];

        histogram[0][r]++;
        histogram[1][g]++;
        histogram[2][b]++;
    }

    stbi_image_free(img);

    vector<double> normalizedHistogram(3 * 256);
    double totalPixels = width * height;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 256; i++)
        {
            normalizedHistogram[c * 256 + i] = histogram[c][i] / totalPixels;
        }
    }

    return normalizedHistogram;
}

bool hasValidImageExtension(const string &fileName)
{
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == string::npos)
    {
        return false;
    }

    string extension = fileName.substr(dotPos + 1);
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return (extension == "jpg" || extension == "png" || extension == "jpeg");
}

int main(int argc, char** argv)
{
    const string outputFilePath = "output.json"; // Output JSON file

    // Load existing JSON if present to allow incremental updates
    json root;
    {
        ifstream in(outputFilePath);
        if (in.is_open()) {
            try { in >> root; }
            catch (const std::exception& e) {
                cerr << "Warning: failed to parse existing " << outputFilePath << ": " << e.what() << "\nOverwriting it." << endl;
                root = json::object();
            }
        }
    }

    // If an argument is provided, process only that image; otherwise process all in Images/
    if (argc >= 2) {
        string imgPath(argv[1]);
        string fileName;
        size_t pos = imgPath.find_last_of("/\\");
        fileName = (pos == string::npos) ? imgPath : imgPath.substr(pos + 1);
        if (!hasValidImageExtension(fileName)) {
            cerr << "Not an image file: " << fileName << endl;
            return 1;
        }
        cout << "Processing image: " << imgPath << endl;
        vector<double> histogram = calculateColorHistogram(imgPath);
        if (!histogram.empty()) {
            root[fileName] = histogram;
        }
    } else {
        const string directoryPath = "Images";
#ifdef _WIN32
        // Windows: iterate files using Win32 ANSI API
        WIN32_FIND_DATAA ffd;
        std::string pattern = directoryPath + "\\*";
        HANDLE hFind = FindFirstFileA(pattern.c_str(), &ffd);
        if (hFind == INVALID_HANDLE_VALUE) {
            cerr << "Error: Images directory not found at: " << directoryPath << endl;
            return 1;
        }

        do {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            string fileName = ffd.cFileName;
            if (!hasValidImageExtension(fileName)) continue;
            string fullPath = directoryPath + "\\" + fileName;
            cout << "Processing image: " << fullPath << endl;
            vector<double> histogram = calculateColorHistogram(fullPath);
            if (!histogram.empty()) {
                root[fileName] = histogram;
            }
        } while (FindNextFileA(hFind, &ffd) != 0);
        FindClose(hFind);
#else
        // POSIX: iterate using dirent
        DIR* dir = opendir(directoryPath.c_str());
        if (!dir) {
            cerr << "Error opening directory: " << directoryPath << endl;
            return 1;
        }
        dirent* ent;
        while ((ent = readdir(dir)) != nullptr) {
            string fileName = ent->d_name;
            if (fileName == "." || fileName == ".." || fileName[0] == '.') continue;
            if (!hasValidImageExtension(fileName)) continue;
            string fullPath = directoryPath + "/" + fileName;
            cout << "Processing image: " << fullPath << endl;
            vector<double> histogram = calculateColorHistogram(fullPath);
            if (!histogram.empty()) {
                root[fileName] = histogram;
            }
        }
        closedir(dir);
#endif
    }

    // Write histogram data to JSON file (overwrite with updated content)
    ofstream outputFile(outputFilePath);
    if (outputFile.is_open())
    {
        outputFile << root.dump(4);
        outputFile.close();
        cout << "Histogram data saved to: " << outputFilePath << endl;
    }
    else
    {
        cerr << "Error opening output file: " << outputFilePath << endl;
        return 1;
    }

    cout << "Processing complete!" << endl;
    return 0;
}
