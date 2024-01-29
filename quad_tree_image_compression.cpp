#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

class QuadNode {
    public:
        //Properties
        cv::Mat img;
        std::vector<QuadNode> children;
        cv::Vec3b color = cv::Vec3b(0, 0, 0);
        float entropy;
        cv::Rect2d rect;

        //Constructor
        QuadNode(cv::Mat _img, cv::Rect2d _rect){
            img = _img;
            rect = _rect;

            // std::cout << rect << std::endl;
            
            double count = 0;
            cv::Vec3d average = cv::Vec3d(0, 0, 0);
            for(double x = rect.x; x < rect.width; x++){
                for(double y = rect.y; y < rect.height; y++){
                    average += img.at<cv::Vec3b>(y, x);
                    // color += img.at<cv::Vec3b>(y, x);
                    // cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
                    // average_r += 
                    count++;
                }
                // std::cout << color << std::endl;
            }

            average /= count;
            // color /= count;
            // std::cout << average << std::endl;
            color = cv::Vec3b(average);
            // std::cout << color << std::endl;

            // color = cv::Vec3b(average_r / count, average_b / count, average_g / count);

            for(double x = rect.x; x < rect.width; x++){
                for(double y = rect.y; y < rect.height; y++){
                    // img.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
                    img.at<cv::Vec3b>(y, x) = color;
                }
            }
        }

        //True if this is a leaf node, false if it has children
        bool is_leaf(){
            return children.size() == 0;
        }

        //Adds a child to this node
        void add_child(QuadNode new_child){
            children.push_back(new_child);
        }

        //Gets the "color" property as a string
        std::string colorString(){
            std::string text("[");
            for(int i = 0; i < color.channels; i++){
                text += std::to_string(color[i]);
                text += " ";
            }
            text.pop_back();
            text += "]";
            return text;
        }

        //A string interpretation of this node
        std::string toString(){
            std::string text(colorString());
            text += " - ";

            for(int i = 0; i < 4; i++){
                if(i < children.size()){
                    text += children[i].colorString();
                }else{
                    text += "#";
                }
                text += " ";
            }
            
            text.pop_back();

            return text;
        }
};

int main(){
    cv::Mat img = cv::imread("C:\\Users\\Ivy\\Documents\\Programming\\C++\\Quad Tree Image Compression\\test.jpg", cv::IMREAD_COLOR);

    std::cout << img.size().width * img.size().height << std::endl;
    // cv::Rect2d(10, )
    // QuadNode root(img, cv::Rect2d(80, 500, 80+150, 500+330));
    QuadNode root(img, cv::Rect2d(0, 0, img.size().width, img.size().height));
    std::vector<QuadNode> quad_tree = {root};


    // QuadNode foo;
    // root.color = cv::Vec3b(1, 2, 3);
    // std::cout << root.toString() << std::endl;
    // root.add_child(foo);
    // std::cout << root.toString() << std::endl;
    cv::imshow("", img);
    int k = cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}