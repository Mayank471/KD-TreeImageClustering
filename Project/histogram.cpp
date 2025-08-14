#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
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
    namespace fs = std::filesystem;
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
        fs::path imgPath(argv[1]);
        if (!fs::exists(imgPath)) {
            cerr << "Image not found: " << imgPath.string() << endl;
            return 1;
        }
        string fileName = imgPath.filename().string();
        if (!hasValidImageExtension(fileName)) {
            cerr << "Not an image file: " << fileName << endl;
            return 1;
        }
        cout << "Processing image: " << imgPath.string() << endl;
        vector<double> histogram = calculateColorHistogram(imgPath.string());
        if (!histogram.empty()) {
            root[fileName] = histogram;
        }
    } else {
        const fs::path imagesDir = fs::path("Images");
        if (!fs::exists(imagesDir) || !fs::is_directory(imagesDir)) {
            cerr << "Error: Images directory not found at: " << imagesDir.string() << endl;
            return 1;
        }
        for (const auto& entry : fs::directory_iterator(imagesDir)) {
            if (!entry.is_regular_file()) continue;
            const fs::path p = entry.path();
            string fileName = p.filename().string();
            if (!hasValidImageExtension(fileName)) continue;
            cout << "Processing image: " << p.string() << endl;
            vector<double> histogram = calculateColorHistogram(p.string());
            if (!histogram.empty()) {
                root[fileName] = histogram;
            }
        }
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
