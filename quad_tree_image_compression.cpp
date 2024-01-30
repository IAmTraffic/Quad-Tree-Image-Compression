#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

class QuadNode {
	public:
		//Properties
		cv::Mat original_img;
		cv::Mat img;
		std::vector<QuadNode> children;
		cv::Vec3b color = cv::Vec3b(0, 0, 0);
		float entropy;
		cv::Rect2d rect;

		//Constructor
		QuadNode(){}
		QuadNode(cv::Mat _img, cv::Rect2d _rect){
			original_img = _img.clone();
			img = _img;
			rect = _rect;
		}

		void paint_color(){
			// std::cout << rect << std::endl;
			double count = 0;
			cv::Vec3d average = cv::Vec3d(0, 0, 0);
			for(double x = rect.x; x < rect.width; x++){
				for(double y = rect.y; y < rect.height; y++){
					average += original_img.at<cv::Vec3b>(y, x);
					count++;
				}
			}

			average /= count;
			color = cv::Vec3b(average);

			float color_len = sqrt(color[0] * color[0] + color[1] * color[1] + color[2] * color[2]);
			cv::Vec3b norm_color = cv::Vec3b(color[0] / color_len, color[1] / color_len, color[2] / color_len);
			std::cout << norm_color[0] << ", " << norm_color[1] << ", " << norm_color[2] << std::endl;

			//We double dip in this 2d loop. We write to the pixels, and we calculate the entropy
			for(double x = rect.x; x < rect.width; x++){
				for(double y = rect.y; y < rect.height; y++){
					img.at<cv::Vec3b>(y, x) = color;

					
					// entropy += 
				}
			}
		}

		//True if this is a leaf node, false if it has children
		bool is_leaf(){
			return children.size() == 0;
		}

		//Adds a child to this node
		void add_child(QuadNode new_child){
			// std::cout << new_child.toString() << std::endl;
			children.push_back(new_child);
			// std::cout << toString() << std::endl;
			// std::cout << children.size() << std::endl;
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

		//Gets the "rect" property as a string
		std::string rectString(){
			std::string text("(");
			text += std::to_string(rect.x) + ", " + std::to_string(rect.y);
			text += " - ";
			text += std::to_string(rect.width) + ", " + std::to_string(rect.height);
			text += ") ";

			return text;
		}

		//A string interpretation of this node
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
		// std::vector<QuadNode> quads;
		// cv::Mat original_img;
		cv::Mat img;
		int max_iterations;
		int iterations;

		QuadTree(cv::Mat _img, int _max_iterations) {
			// original_img = _img.clone();
			img = _img;
			max_iterations = _max_iterations;

			// root.original_img = original_img;
			root.original_img = img.clone();
			root.img = img;
			root.rect = cv::Rect2d(0, 0, img.size().width, img.size().height);
		}

		void iterate(){
			//Choose next quad (must be leaf)
			QuadNode* highest_entropy_node;   //TODO
			// QuadNode* highest_entropy_node = &root;   //TODO
			float highest_entropy = -1;
			std::vector<QuadNode *> frontier = {&root};
			// std::vector<int> test = {0, 1, 2, 3, 4};
			while(frontier.size() > 0){
				// std::cout << test[0]
				// int *ptr = &test[0];


				// QuadNode *test = &(frontier[0]);
				// QuadNode* ptr_to_first = frontier.data();

				// ptr_to_first->entropy = 12;

				// std::vector<QuadNode>::iterator iterator = frontier.begin();

				// *iterator.entropy = 11;

				// QuadNode *quad = &frontier[0];
				// QuadNode *quad = &root;
				// QuadNode test = *iterator;
				// std::cout << quad << std::endl;
				// std::cout << &test << std::endl;
				// std::cout << &ptr_to_first << std::endl;
				// std::cout << &root << std::endl;
				// test->entropy = 10;
				// std::cout << quad->toString() << std::endl;
				// std::cout << test->toString() << std::endl;
				// std::cout << ptr_to_first->toString() << std::endl;
				// std::cout << root.toString() << std::endl;
				// std::cout << std::endl;
				


				// QuadNode* next = &frontier[frontier.size() - 1];
				QuadNode *next = frontier[frontier.size() - 1];
				frontier.pop_back();
				// (*next).entropy = 14;
				// std::cout << (*next).toString() << std::endl;
				// std::cout << (*frontier[frontier.size() - 1]).toString() << std::endl;
				// std::cout << root.toString() << std::endl;
				// (*ptr).entropy = 13;
				if(next->is_leaf()){
					if(next->entropy > highest_entropy){
						// std::cout << next->toString() << std::endl;
						highest_entropy = next->entropy;
						highest_entropy_node = next;
					}
				}else{
					for(int i = 0; i < next->children.size(); i++){
						frontier.push_back(&next->children[i]);
					}
				}
			}
			
			
			//Split the quad
			// std::cout << std::endl;
			// std::cout << std::endl;
			// std::cout << std::endl;
			root.entropy = 16;
			// std::cout << root.toString() << std::endl;
			// std::cout << highest_entropy_node->toString() << std::endl;
			highest_entropy_node->add_child(QuadNode(img, cv::Rect2d(highest_entropy_node->rect.x, highest_entropy_node->rect.y, highest_entropy_node->rect.width / 2, highest_entropy_node->rect.height / 2)));
			highest_entropy_node->add_child(QuadNode(img, cv::Rect2d(highest_entropy_node->rect.x + highest_entropy_node->rect.width / 2, highest_entropy_node->rect.y, highest_entropy_node->rect.width, highest_entropy_node->rect.height / 2)));
			highest_entropy_node->add_child(QuadNode(img, cv::Rect2d(highest_entropy_node->rect.x, highest_entropy_node->rect.y + highest_entropy_node->rect.height / 2, highest_entropy_node->rect.width / 2, highest_entropy_node->rect.height)));
			highest_entropy_node->add_child(QuadNode(img, cv::Rect2d(highest_entropy_node->rect.x + highest_entropy_node->rect.width / 2, highest_entropy_node->rect.y + highest_entropy_node->rect.height / 2, highest_entropy_node->rect.width, highest_entropy_node->rect.height)));
			// std::cout << highest_entropy_node->toString() << std::endl;
			// std::cout << root.toString() << std::endl;


			// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x, next->rect.y, next->rect.width / 2, next->rect.height / 2)));
			// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x + next->rect.width / 2, next->rect.y, next->rect.width, next->rect.height / 2)));
			// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x, next->rect.y + next->rect.height / 2, next->rect.width / 2, next->rect.height)));
			// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x + next->rect.width / 2, next->rect.y + next->rect.height / 2, next->rect.width, next->rect.height)));


			// std::cout << &root << std::endl;
			// std::cout << &highest_entropy_node << std::endl;


			// for(int i = 0; i < 2; i++){
			// 	for(int j = 0; j < 2; j++){
			// 		QuadNode new_quad(img, cv::Rect2d(next.rect.x + next.rect.width));
			// 	}
			// }

			iterations++;
		}

		void write_to_img(){
			std::vector<QuadNode> frontier = {root};
			while(frontier.size() > 0){
				// std::cout << std::endl;
				// std::cout << frontier.size() << std::endl;
				QuadNode next = frontier[frontier.size() - 1];
				frontier.pop_back();
				// i--;
				// std::cout << next.toString() << std::endl;
				// std::cout << frontier.size() << std::endl;
				if(next.is_leaf()){
					next.paint_color();
				}else{
					for(int i = 0; i < next.children.size(); i++){
						std::cout << next.children[i].toString() << std::endl;
						frontier.push_back(next.children[i]);
					}
				}
			}
		}
};

int main(){
	cv::Mat img = cv::imread("C:\\Users\\Ivy\\Documents\\Programming\\C++\\Quad Tree Image Compression\\test.jpg", cv::IMREAD_COLOR);
	cv::resize(img, img, cv::Size(700, 700));

	// std::vector<int> test = {0, 1, 2, 3, 4};
	// int *ptr = &test[0];
	// for(int i : test){
	// 	std::cout << i << std::endl;
	// }
	// *ptr = 9;
	// std::cout << std::endl;
	// for(int i : test){
	// 	std::cout << i << std::endl;
	// }

	// QuadNode node = QuadNode(img, cv::Rect2d(0, 0, img.size().width, img.size().height));
	// QuadNode node2 = QuadNode(img, cv::Rect2d(0, 0, img.size().width, img.size().height));
	// std::vector<QuadNode> test = {node, node2};
	// QuadNode *ptr = &test[1];
	// for(QuadNode i : test){
	// 	std::cout << i.toString() << std::endl;
	// }
	// (*ptr).entropy = 13;
	// std::cout << std::endl;
	// for(QuadNode i : test){
	// 	std::cout << i.toString() << std::endl;
	// }

	// return 0;
	

	QuadTree quad_tree(img, 4);
	quad_tree.iterate();
	quad_tree.write_to_img();


	cv::imshow("", img);
	int k = cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}