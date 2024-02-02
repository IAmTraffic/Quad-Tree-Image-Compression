#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

//Some images used from NASA, the Smithsonian Institute, or Pixabay

class QuadNode {
	public:
		cv::Mat original_img;
		cv::Mat img;
		std::vector<QuadNode> children;
		cv::Vec3b color = cv::Vec3b(0, 0, 0);
		float entropy;
		cv::Rect2d rect;

		QuadNode(){}
		QuadNode(cv::Mat _img, cv::Mat _original_img, cv::Rect2d _rect){
			original_img = _original_img;
			img = _img;
			rect = _rect;

			paint_color();
		}

		//Calculates the average color of this quad, writes that color to the image in the space occupied by this quad's rect, and calcualtes this quad's entropy
		void paint_color(){
			entropy = 0;

			//Calculate the average color of this quad
			double count = 0;
			cv::Vec3d average = cv::Vec3d(0, 0, 0);
			for(double x = rect.x; x < rect.x + rect.width; x++){
				for(double y = rect.y; y < rect.y + rect.height; y++){
					average += original_img.at<cv::Vec3b>(y, x);
					count++;
				}
			}
			average /= count;
			color = cv::Vec3b(average);

			//We double dip in this 2d loop. We write to the pixels, and we calculate the entropy
			for(double x = rect.x; x < rect.x + rect.width; x++){
				for(double y = rect.y; y < rect.y + rect.height; y++){
					//Write the average color to every pixel in this quad
					img.at<cv::Vec3b>(y, x) = color;

					//While we're here, calculate the entropy of this quad
					cv::Vec3b original_pixel = original_img.at<cv::Vec3b>(y, x);
					entropy += abs((int) (original_pixel[0] - color[0]));
					entropy += abs((int) (original_pixel[1] - color[1]));
					entropy += abs((int) (original_pixel[2] - color[2]));
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

		//Gets the "color" property as a string, since OpenCV's string representations of its classes are lacking
		//Used for debugging
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

		//Gets the "rect" property as a string, since OpenCV's string representations of its classes are lacking
		//Used for debugging
		std::string rectString(){
			std::string text("(");
			text += std::to_string(rect.x) + ", " + std::to_string(rect.y);
			text += " - ";
			text += std::to_string(rect.width) + ", " + std::to_string(rect.height);
			text += ") ";

			return text;
		}

		//A string interpretation of this node
		//Used for debugging
		std::string toString(){
			std::string text(colorString());
			text += " " + std::to_string(entropy) + " - ";
			text += rectString();

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

class QuadTree {
	public:
		QuadNode root;
		cv::Mat original_img;	//Stored for later, since the quads will be writing to img, but future quads will need access to the original image for reading
		cv::Mat img;

		QuadTree(cv::Mat _img) {
			original_img = _img.clone();
			img = _img;
			
			//Setup the root node
			root.original_img = original_img;
			root.img = img;
			root.rect = cv::Rect2d(0, 0, img.size().width, img.size().height);
			root.paint_color();
		}

		//The method called from outside this class in order to generate the image
		//It will split the image up into sub-quads a number of times equal to num_iterations
		void iterate(int num_iterations){
			for(int i = 0; i < num_iterations; i++){
				//Choose next quad (must be leaf)
				QuadNode* h_e_n;   //Highest Entropy Node
				float highest_entropy = -1;
				//Recursively search from the root node to all children, looking for the leaf node with the highest entropy
				std::vector<QuadNode *> frontier = {&root};
				while(frontier.size() > 0){
					QuadNode *next = frontier[frontier.size() - 1];
					frontier.pop_back();
					if(next->is_leaf()){
						if(next->entropy > highest_entropy){
							highest_entropy = next->entropy;
							h_e_n = next;
						}
					}else{
						for(int i = 0; i < next->children.size(); i++){
							frontier.push_back(&next->children[i]);
						}
					}
				}
				
				
				//Split the quad
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));	
			}
		}

		//Outlines each quad (optional)
		void draw_outlines(){
			int color_bw = 20;
			cv::Scalar color = cv::Scalar(color_bw, color_bw, color_bw);
			int thickness = 1;
			std::vector<QuadNode *> frontier = {&root};
			while(frontier.size() > 0){
				QuadNode *next = frontier[frontier.size() - 1];
				frontier.pop_back();
				if(next->is_leaf()){
					cv::line(img, cv::Point(next->rect.x, next->rect.y), cv::Point(next->rect.x, next->rect.y + next->rect.height), color, thickness);
					cv::line(img, cv::Point(next->rect.x, next->rect.y), cv::Point(next->rect.x + next->rect.width, next->rect.y), color, thickness);
					// cv::line(img, cv::Point(next->rect.x + next->rect.width, next->rect.y), cv::Point(next->rect.x + next->rect.width, next->rect.y + next->rect.height), color, thickness);
				}else{
					for(int i = 0; i < next->children.size(); i++){
						frontier.push_back(&next->children[i]);
					}
				}
			}
			// cv::line(img, cv::Point(100, 100), cv::Point(500, 500), cv::Scalar(255, 255, 0), 10);
		}
};

int main(){
	//Open the image in OpenCV and resize it to fit my monitor with room to spare
	cv::Mat img = cv::imread("C:\\Users\\Ivy\\Documents\\Programming\\C++\\Quad Tree Image Compression\\boat-8515980_1280.jpg", cv::IMREAD_COLOR);
	float image_dest_height = 600.0;
	cv::resize(img, img, cv::Size(), image_dest_height / img.size[0], image_dest_height / img.size[0]);	

	//Initialize the quad tree
	QuadTree quad_tree(img);

	//Split the image up a number of times
	quad_tree.iterate(500);

	//Draw the outlines of the quads (optional)
	quad_tree.draw_outlines();

	//Display the quad-compressed image
	cv::imshow("", img);
	int k = cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}