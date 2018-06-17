#include "InputYUV.hpp"
#include "inputFilePath.hpp"
#include "direct.h"
// #define DEBUG

enum PARAM
{
    PARAM_FILEPATH = 1,
    PARAM_HEIGHT,
    PARAM_WIDTH,
    PARAM_CHROMAFORMAT,
    PARAM_OUTPUTPATH,
    PARAM_SIZE
};

int main(int argc, char** argv)
{
    int height = 0, width = 0, chromaFormat = -1;
    std::string inputPath, outputPath, _CHROMAFORMAT;

    #ifndef DEBUG
        if (argc < PARAM_SIZE)
        {
			std::cerr << "Some Parameters missed, USAGE: " << argv[0] << "<input YUV File Path or Directory Path> <height> <width> <chromaFormat> <ouput Frame File Path>" << std::endl;
            exit(EXIT_FAILURE);
        }else{
            inputPath.assign(argv[PARAM_FILEPATH]);
            height = std::stoi(argv[PARAM_HEIGHT]);
            width = std::stoi(argv[PARAM_WIDTH]);
            outputPath.assign(argv[PARAM_OUTPUTPATH]);
            _CHROMAFORMAT.assign(argv[PARAM_CHROMAFORMAT]);
        }
    #else
        // read from single YUV file
        inputPath.assign("X:/Database/CSIQ/BasketballDrive/BasketballDrive_832x480_dst_00.yuv");
        // read from directory containg YUV files
        inputPath.assign("X:/Database/CSIQ/");

        height = 480;
        width = 832;
        // for <chromaFormat>:
        // YUV400 = "400"
        // YUV411 = "411"
        // YUV420 = "420"
        // YUV422 = "422"
        // YUV444 = "444"
		_CHROMAFORMAT = "420";
        outputPath.assign("X:/Output/");
    #endif

	if (_CHROMAFORMAT.compare("400") == 0)
		chromaFormat = 0;
	else if (_CHROMAFORMAT.compare("411") == 0)
		chromaFormat = 1;
	else if (_CHROMAFORMAT.compare("420") == 0)
		chromaFormat = 2;
	else if (_CHROMAFORMAT.compare("422") == 0)
		chromaFormat = 3;
	else if (_CHROMAFORMAT.compare("444") == 0)
		chromaFormat = 4;
	else {
		std::cerr << "Wrong Chrom Format, please note that" << std::endl
			<< "YUV400\:400" << std::endl
			<< "YUV411\:411" << std::endl
			<< "YUV420\:420" << std::endl
			<< "YUV422\:422" << std::endl
			<< "YUV444\:444" << std::endl;
		exit(EXIT_FAILURE);
	}

    inputFilePath yuvFileList(inputPath, "yuv");
    if (inputPath.substr(inputPath.size() - 4, 4) == ".yuv")
    {
        if (_access(inputPath.c_str(), 4))
        {
            std::cerr << "Problem accessing input yuv file: " << inputPath << std::endl;
            exit(EXIT_FAILURE);
        }
		checkPath(inputPath);
        auto pos = inputPath.rfind('/');
        yuvFileList.fileName.push_back(inputPath.substr(pos + 1, inputPath.size() - pos - 5));
        yuvFileList.filePath.insert(std::make_pair(yuvFileList.fileName.back(), inputPath));
    }else{
        // change 'true' to 'false' if do not want to check sub directory
        yuvFileList.initial(true);
    }

    for (auto file_name = yuvFileList.fileName.begin(); file_name != yuvFileList.fileName.end(); ++file_name)
    {
        std::string inputYUVPath(yuvFileList.filePath.at(*file_name));
        std::cout << "Extract frames from " << inputYUVPath << std::endl;
        InputYUV yuv_file(inputYUVPath, height, width, chromaFormat);

        for (auto frame_idx = 0; frame_idx < yuv_file.nframes; ++frame_idx)
        {
            auto frame_image = yuv_file.readOneFrame();

            // change this part if you want to rename the output frame image files
            std::string savePath(savefile(outputPath, *file_name, "frame_", frame_idx, ".png"));
            cv::imwrite(savePath.c_str(), *frame_image);
        }
    }

    return 0;
}