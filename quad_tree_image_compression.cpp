#include <iostream>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>

//Some images used from NASA, the Smithsonian Institute, or Pixabay

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
		QuadNode(cv::Mat _img, cv::Mat _original_img, cv::Rect2d _rect){
			original_img = _original_img;
			img = _img;
			rect = _rect;

			paint_color();
		}

		void paint_color(){

			// std::cout << std::endl;
			// std::cout << "Painting Color" << std::endl;
			// std::cout << &entropy << std::endl;
			entropy = 0;
			// std::cout << rect << std::endl;
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

			// std::cout << original_img.at<cv::Vec3b>(300, 900) << std::endl;

			// cv::Vec3b normalized_color = normalize_vec3(color);
				// cv::Vec3f temp_norm_color = cv::Vec3f((float) color[0] / color_len, (float) color[1] / color_len, (float) color[2] / color_len);
				// std::cout << color.col(0) << std::endl;
				// std::cout << color.cols << std::endl;
				// std::cout << temp_norm_color.col(0) << std::endl;
				// std::cout << temp_norm_color[0] << std::endl;
				// std::cout << (int) color[0] << std::endl;
				// std::cout << (int) color[1] << std::endl;
				// std::cout << (int) color[2] << std::endl;
				// std::cout << (float) color[0] << std::endl;



				// std::cout << std::endl;

				// // normalized_color[0] = temp_norm_color[0];
				
				// std::cout << normalized_color.col(0) << std::endl;
				// std::cout << normalized_color[0] << std::endl;
				// std::cout << normalized_color[1] << std::endl;
				// std::cout << normalized_color[2] << std::endl;

				// std::cout << 10 * normalized_color[0] << std::endl;

				// std::cout << std::endl;
			// }
			// std::cout << color[0] << ", " << color[1] << ", " << color[2] << std::endl;
			// std::cout << norm_color[0] << ", " << norm_color[1] << ", " << norm_color[2] << std::endl;
			// std::cout << colorString() << std::endl;
			// std::cout << norm_color.col(0) << std::endl;
			// std::cout << color_len << std::endl;
			// std::cout << sqrt(color[0] * color[0] + color[1] * color[1] + color[2] * color[2]) << std::endl;

			//We double dip in this 2d loop. We write to the pixels, and we calculate the entropy
			for(double x = rect.x; x < rect.x + rect.width; x++){
				for(double y = rect.y; y < rect.y + rect.height; y++){
					img.at<cv::Vec3b>(y, x) = color;

					// cv::Vec3f normalized_pixel = normalize_vec3(original_img.at<cv::Vec3b>(y, x));
					// std::cout << (double)normalized_color.ddot(normalized_pixel) << std::endl;
					// entropy += 

					// std::cout << std::endl;
					cv::Vec3b original_pixel = original_img.at<cv::Vec3b>(y, x);
					// std::cout << (int) original_pixel[0] << std::endl;
					// std::cout << (int) color[0] << std::endl;
					// std::cout << abs((int) (original_pixel[0] - color[0])) << std::endl;
					// std::cout << entropy << std::endl;
					entropy += abs((int) (original_pixel[0] - color[0]));
					// std::cout << entropy << std::endl;
					entropy += abs((int) (original_pixel[1] - color[1]));
					// std::cout << entropy << std::endl;
					entropy += abs((int) (original_pixel[2] - color[2]));
					// std::cout << entropy << std::endl;
					// entropy += (abs((int) original_pixel[0] - (int) color[0]) + abs((int) original_pixel[1] - (int) color[1]) + abs((int) original_pixel[2] - (int) color[2]));
				}
			}

			// float entropy_scaling = (rect.width * 100);
			// if(entropy_scaling > 0){
			// 	entropy /= entropy_scaling;
			// }else{
			// 	entropy = -1;
			// }
			// entropy = rect.width * rect.height + rect.x + rect.y + rand() % 100;
			// entropy = rand() % 10;

			// std::cout << toString() << std::endl;
		}

		// cv::Vec3f normalize_vec3(cv::Vec3b vector){
		// 	float len = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
		// 	if(len == 0){
		// 		return cv::Vec3b(0, 0, 0);
		// 	}

		// 	return cv::Vec3f((float) vector[0] / len, (float) vector[1] / len, (float) vector[2] / len);
		// }

		//True if this is a leaf node, false if it has children
		bool is_leaf(){
			//Edge Case - Child would be 0px in size
			// if(rect.width / 2 == 0 || rect.height / 2 == 0){
			// 	entropy = -1;
			// 	return false;
			// }

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
		cv::Mat original_img;
		cv::Mat img;
		// int max_iterations;
		// int iterations;

		QuadTree(cv::Mat _img) {
			original_img = _img.clone();
			img = _img;
			// max_iterations = _max_iterations;

			// root.original_img = original_img;
			root.original_img = original_img;
			root.img = img;
			root.rect = cv::Rect2d(0, 0, img.size().width, img.size().height);
			root.paint_color();
		}

		void iterate(int num_iterations){
			for(int i = 0; i < num_iterations; i++){
				// std::cout << std::endl << std::endl << "Iterating" << std::endl;
				//Choose next quad (must be leaf)
				QuadNode* h_e_n;   //Highest Entropy Node
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
							h_e_n = next;
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
				// root.entropy = 16;
				// std::cout << root.toString() << std::endl;
				// std::cout << "Highest Entropy Node: " << h_e_n->toString() << std::endl;
				// std::cout << h_e_n->rectString() << std::endl;
				

				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				// h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y, h_e_n->rect.width / 2, h_e_n->rect.height / 2)));
				// h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y, h_e_n->rect.width / 2 + h_e_n->rect.x / 2, h_e_n->rect.height / 2)));
				// h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2, h_e_n->rect.height / 2 + h_e_n->rect.y / 2)));
				// h_e_n->add_child(QuadNode(img, original_img, cv::Rect2d(h_e_n->rect.x + h_e_n->rect.width / 2, h_e_n->rect.y + h_e_n->rect.height / 2, h_e_n->rect.width / 2 + h_e_n->rect.x / 2, h_e_n->rect.height / 2 + h_e_n->rect.y / 2)));
				// std::cout << h_e_n->toString() << std::endl;
				// std::cout << root.toString() << std::endl;


				// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x, next->rect.y, next->rect.width / 2, next->rect.height / 2)));
				// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x + next->rect.width / 2, next->rect.y, next->rect.width, next->rect.height / 2)));
				// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x, next->rect.y + next->rect.height / 2, next->rect.width / 2, next->rect.height)));
				// // next->add_child(QuadNode(img, cv::Rect2d(next->rect.x + next->rect.width / 2, next->rect.y + next->rect.height / 2, next->rect.width, next->rect.height)));


				// std::cout << &root << std::endl;
				// std::cout << &h_e_n << std::endl;


				// for(int i = 0; i < 2; i++){
				// 	for(int j = 0; j < 2; j++){
				// 		QuadNode new_quad(img, cv::Rect2d(next.rect.x + next.rect.width));
				// 	}
				// }

				// iterations++;	
			}
		}

		// void write_to_img(){
		// 	std::vector<QuadNode> frontier = {root};
		// 	while(frontier.size() > 0){
		// 		// std::cout << std::endl;
		// 		// std::cout << frontier.size() << std::endl;
		// 		QuadNode next = frontier[frontier.size() - 1];
		// 		frontier.pop_back();
		// 		// i--;
		// 		// std::cout << next.toString() << std::endl;
		// 		// std::cout << frontier.size() << std::endl;
		// 		if(next.is_leaf()){
		// 			next.paint_color();
		// 		}else{
		// 			for(int i = 0; i < next.children.size(); i++){
		// 				std::cout << next.children[i].toString() << std::endl;
		// 				frontier.push_back(next.children[i]);
		// 			}
		// 		}
		// 	}
		// }
};

int main(){
	cv::Mat img = cv::imread("C:\\Users\\Ivy\\Documents\\Programming\\C++\\Quad Tree Image Compression\\trees-8512979_1280.jpg", cv::IMREAD_COLOR);
	// int test_size = 10;
	// cv::resize(img, img, cv::Size(test_size, test_size));
	cv::resize(img, img, cv::Size((img.size[1] / 700) * img.size[0], 700));	

	QuadTree quad_tree(img);
	quad_tree.iterate(1000);

	
	// cv::resize(img, img, cv::Size(500, 500), 0.0, 0.0, 0);	
	cv::imshow("", img);
	int k = cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}