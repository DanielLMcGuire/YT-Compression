
# YouTube Compression Tool

## Overview

The YouTube Compression Tool is a command-line application designed to compress video files using FFmpeg in an extremely similar way to the YouTube Compression Algorithm.

## What the YouTube Algorithm does: (Based on Research)

- Video codec conversion based on video resolution (H.264 for 1080p and below; VP9 for higher resolutions).
- Audio codec conversion based on audio bitrate.
- Two-pass encoding for improved video and audio quality and compression.

## Prerequisites

- **Windows Users:** You need `winget` installed. If it's not available, the tool will prompt to download and install it automatically.
- **Linux Users:** Ensure that `ffmpeg` and `ffprobe` are installed on your system.

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/yourusername/YouTubeCompressionTool.git
   cd YouTubeCompressionTool
   ```

2. **Build the project:**

   Use the provided `Makefile` to compile the code. You can build for Windows or Linux: (Haven't tested Linux yet)

   ```bash
   make 
   ```

## Usage

To use the tool, run the compiled executable with the path to your video file as an argument:

```bash
./ytcomp <input_file>
```

### Example

```bash
./YouTubeCompressionTool video.mp4
```

## How It Works

1. **Codec Determination:** Based on the video resolution, the appropriate codec is selected:
   - **libx264** for resolutions up to 1080p.
   - **libvpx-vp9** for resolutions above 1080p.
2. **Two-Pass Encoding:** The tool performs a two-pass encoding process for better quality and compression:
3. **Windows Installation:** If `winget` is not found, the tool will prompt to download and install it, then use it to install FFmpeg.

## Troubleshooting

- **Command Failed Errors:** If you encounter issues with commands failing, ensure FFmpeg is properly installed and accessible from your system's PATH.
- **FFmpeg Not Found on Windows:** If `winget` fails to install FFmpeg, you can manually download and install FFmpeg from [FFmpeg's official website](https://ffmpeg.org/download.html).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Feel free to submit issues or pull requests. For major changes, please open an issue first to discuss what you would like to change.

## Contact

For any inquiries or support, please open an issue in the repository or contact danielmcguire23@icloud.com.

## Legal Notice

This project is an independent tool for video compression and is not affiliated with, endorsed by, or sponsored by YouTube, Google, or any of their subsidiaries. The use of the term "YouTube" in the project's name or description is solely for descriptive purposes and does not imply any official connection or partnership with YouTube or Google. All trademarks, logos, and other identifiers associated with YouTube and Google are the property of their respective owners.
