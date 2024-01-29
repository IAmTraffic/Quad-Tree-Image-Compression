#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat read_image(std::string path, int read_type){
    cv::Mat img = cv::imread(path, read_type);

    if(img.empty()){
        std::cout << "Failed to read image: " << path << std::endl;
    }else{
        std::cout << "Image read successfully" << std::endl;
    }
    
    return img;
}

int main(){

    std::cout << "Version :" << CV_VERSION << std::endl;

    //Open the original image
    std::string image_path = "C:\\Users\\Ivy\\Documents\\Programming\\C++\\Quad Tree Image Compression\\test.jpg";
    cv::Mat img = read_image(image_path, cv::IMREAD_COLOR);

    //Open B&W original
    cv::Mat bw_img = read_image(image_path, cv::IMREAD_GRAYSCALE);

    //Crop test
    cv::Mat crop = img(cv::Range(80, 500), cv::Range(150, 330));

    //CONVOLUTIONS
    //Identity (no change)
    cv::Mat identity_kernel = (cv::Mat_<double>(3, 3) << 0, 0, 0, 0, 1, 0, 0, 0, 0);
    cv::Mat identity;
    cv::filter2D(img, identity, -1, identity_kernel);

    //Box blur
    int box_blur_side_len = 20;
    cv::Mat box_blur_kernel = cv::Mat::ones(box_blur_side_len, box_blur_side_len, CV_64F);
    box_blur_kernel /= box_blur_side_len * box_blur_side_len;
    cv::Mat box_blur;
    cv::filter2D(img, box_blur, -1, box_blur_kernel);

    //Rescaling
    cv::Mat downsized;
    cv::resize(img, downsized, cv::Size(200, 200), cv::INTER_NEAREST);
    // cv::resize(downsized, downsized, cv::Size(800, 800), cv::INTER_NEAREST);

    //Thresholding
    cv::Mat thresholded;
    cv::threshold(bw_img, thresholded, 100, 200, cv::THRESH_BINARY);
    for(int i = 0; i < thresholded.rows; i++){
        for(int j = 0; j < thresholded.cols; j++){
            if(thresholded.at<uchar>(i, j) > 0){
                thresholded.at<uchar>(i, j) = 255;
            };
        }
    }


    //Display all the images that we've created
    // cv::imshow("Original", img);
    // std::cout << img.at<cv::Vec3b>(img.rows / 2.0, img.cols / 2.0) << std::endl;
    cv::imshow("Cropped", crop);
    // cv::imshow("Convol: Identity", identity);
    // cv::imshow("Convol: Box Blur", box_blur);
    // cv::imshow("Scaled Down", downsized);
    // cv::imshow("Thresholding", thresholded);
    int k = cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}