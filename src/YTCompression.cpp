#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstdio> 

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#else
#include <unistd.h>
#endif

void run_command(const std::string &command) {
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Command failed: " << command << std::endl;
        exit(EXIT_FAILURE);
    }
}

void show_message_box(const std::string &message, const std::string &download_url) {
#ifdef _WIN32
    int msgboxID = MessageBox(
        NULL,
        message.c_str(),
        "FFmpeg Check",
        MB_ICONWARNING | MB_OKCANCEL | MB_DEFBUTTON1
    );

    if (msgboxID == IDCANCEL) {
        exit(EXIT_FAILURE);
    } else if (msgboxID == IDOK) {
        // Download and install winget
        std::string winget_url = download_url;
        std::string file_name = "Microsoft.DesktopAppInstaller_8wekyb3d8bbwe.msixbundle";

        std::cout << "Downloading " << file_name << "..." << std::endl;
        run_command("powershell -Command \"Invoke-WebRequest -Uri " + winget_url + " -OutFile " + file_name + "\"");

        std::cout << "Installing " << file_name << "..." << std::endl;
        run_command("powershell -Command \"Add-AppxPackage -Path .\\" + file_name + "\"");

        std::cout << "Installing FFmpeg using winget..." << std::endl;
        run_command("powershell -Command \"winget install ffmpeg\"");
        
        std::cout << "Installation complete." << std::endl;
        exit(EXIT_SUCCESS);
    }
#else
    std::cerr << message << std::endl;
    std::cerr << "Please download FFmpeg from https://ffmpeg.org/download.html" << std::endl;
    exit(EXIT_FAILURE);
#endif
}

void run_ffmpeg(const std::string &command) {
    //Comment out the two lines below, and uncomment the third one down for full output.
    std::string filtered_command = command + " 2>&1 | grep 'frame='";
    int result = std::system(filtered_command.c_str());
//    int result = std::system((command + " 2>&1").c_str());
    if (result != 0) {
        std::cerr << "FFmpeg command failed: " << command << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::pair<int, int> get_video_resolution(const std::string &input_file) {
    std::string command = "ffprobe -v error -select_streams v:0 -show_entries stream=width,height -of csv=s=x:p=0 " + input_file;
    std::string result;

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to run ffprobe");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);

    int width, height;
    sscanf(result.c_str(), "%dx%d", &width, &height);

    return {width, height};
}

std::string determine_codec(int width, int height) {
    if (width <= 1920 && height <= 1080) {
        return "libx264";
    } else {
        return "libvpx-vp9";
    }
}

void two_pass_encoding(const std::string &input_file) {
    const std::string video_bitrate = "2M";
    const std::string audio_bitrate = "128k";
    const std::string preset = "slow";
    const std::string audio_codec_high_bitrate = "libopus";
    const std::string audio_codec_low_bitrate = "aac";

    auto [width, height] = get_video_resolution(input_file);
    std::string codec = determine_codec(width, height);

    std::string first_pass_command;
    std::string second_pass_command;

    std::string output_file = input_file.substr(0, input_file.find_last_of('.')) + "-comp";

    if (codec == "libx264") {
        // H.264 encoding
        first_pass_command = "ffmpeg -y -i " + input_file + " -c:v " + codec + " -preset " + preset + " -b:v " + video_bitrate + " -pass 1 -an -f mp4 NUL";
        second_pass_command = "ffmpeg -y -i " + input_file + " -c:v " + codec + " -preset " + preset + " -b:v " + video_bitrate + " -pass 2 -c:a " + (audio_bitrate == "192k" ? audio_codec_high_bitrate : audio_codec_low_bitrate) + " -b:a " + audio_bitrate + " " + output_file + ".mp4";
    } else if (codec == "libvpx-vp9") {
        // VP9 encoding
        first_pass_command = "ffmpeg -y -i " + input_file + " -c:v " + codec + " -b:v " + video_bitrate + " -pass 1 -an -f webm NUL";
        second_pass_command = "ffmpeg -y -i " + input_file + " -c:v " + codec + " -b:v " + video_bitrate + " -pass 2 -c:a " + (audio_bitrate == "192k" ? audio_codec_high_bitrate : audio_codec_low_bitrate) + " -b:a " + audio_bitrate + " " + output_file + ".webm";
    }

    std::cout << "Running first pass" << std::endl;
    std::cout << "If the command line seems frozen, it is working in background." << std::endl;
    run_ffmpeg(first_pass_command);
    std::cout << "Running second pass" << std::endl;
    std::cout << "If the command line seems frozen, it is working in background." << std::endl;
    run_ffmpeg(second_pass_command);
}

void logo() {
    const std::string VERSION = "0.1.0.7.2430";
    const std::string TITLE = "YouTube Compression Tool";
    const std::string LOGO = TITLE + " " + VERSION;
    std::cout << "\033[34m" << LOGO << "\033[0m" << std::endl; // Blue text
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return EXIT_FAILURE;
    }

    logo(); // Display the logo at the start

    std::string input_file = argv[1];
    try {
        two_pass_encoding(input_file);
        std::cout << "\033[32mSuccess!\033[0m" << std::endl; // Green text for success
    } catch (const std::exception &e) {
        std::cerr << "\033[31mError: " << e.what() << "\033[0m" << std::endl; // Red text for errors
        return 1;
    }

    logo(); // Display the logo at the end

    return 0;
}
