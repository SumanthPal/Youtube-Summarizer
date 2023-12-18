#ifndef HELLOSFML_TRANSCRIPTSTEALER_H
#define HELLOSFML_TRANSCRIPTSTEALER_H

#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

namespace  fs = std::__fs::filesystem;
class TranscriptStealer {
public:

std::string findVttFile(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".vtt") {
            return entry.path().string();
        }
    }
    return ""; // Empty string if no matching file is found
}
std::string removeVttFile(const std::string path) {
    std::string("rm " + path);
}
    std::string getTranscript(const std::string& videoUrl) {
        // Use youtube-dl to download subtitles in VTT format
        std::string command = "youtube-dl --skip-download --write-sub --sub-lang en --output \"%(title)s.%(ext)s\" " + videoUrl;
        std::system(command.c_str());

        std::string path = findVttFile("/Users/sumanth/CLionProjects/youtube++/cmake-build-debug");

        // Read and parse the VTT file
        std::ifstream vttFile(path);
        if (!vttFile.is_open()) {
            std::cerr << "Error opening VTT file." << std::endl;
            return "";
        }

        std::stringstream transcript;
        bool inTranscript = false;
        std::string line;

        while (std::getline(vttFile, line)) {
            if (line.empty()) {
                inTranscript = false;
            } else if (line.find("-->") != std::string::npos) {
                inTranscript = true;
            } else if (inTranscript) {
                transcript << line << ' ';
            }
        }

        vttFile.close();

        return transcript.str();
    }
};



#endif //HELLOSFML_TRANSCRIPTSTEALER_H