#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;

int main() {
    // Load the image
    string path;
    cout<<"Enter the image path"<<endl;
    cin>>path;
    
    cv::Mat img = cv::imread(path);
    
    if (img.empty()) {
        cout<<"Error in image"<<endl;
        return -1;
    }

    cv::Mat rgb_mat(img.rows, img.cols, CV_8UC3);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            cv::Vec3b pix = img.at<cv::Vec3b>(i, j);

            // Store RGB values in the 3D matrix
            rgb_mat.at<cv::Vec3b>(i, j) = pix;
        }
    }
   
   int imp_mat[img.rows][img.cols];
   int row = img.rows;
   int col = img.cols;
   
    for(int i=0;i<row;i++)
    {	
    	for(int j=0;j<col;j++)
    	{
    		int leftindex;
    		int rightindex;
    		int upindex;
    		int downindex;
    		
    		j==0?leftindex=col-1:leftindex=j-1;
    		rightindex = (j+1)%col;
    		i==0?upindex=row-1:upindex=i-1;
    		downindex = (i+1)%row;
    		
    		cv::Vec3b left = rgb_mat.at<cv::Vec3b>(i, leftindex);
    		cv::Vec3b right = rgb_mat.at<cv::Vec3b>(i, rightindex);
    		cv::Vec3b up = rgb_mat.at<cv::Vec3b>(upindex, j);
    		cv::Vec3b down = rgb_mat.at<cv::Vec3b>(downindex, j);
    		
    		int red_x = abs(static_cast<int>(up[2]) - static_cast<int>(down[2]));
    		int green_x = abs(static_cast<int>(up[1]) - static_cast<int>(down[1]));
    		int blue_x = abs(static_cast<int>(up[0]) - static_cast<int>(down[0]));
    		int red_y = abs(static_cast<int>(left[2]) - static_cast<int>(right[2]));
    		int green_y = abs(static_cast<int>(left[1]) - static_cast<int>(right[1]));
    		int blue_y = abs(static_cast<int>(left[0]) - static_cast<int>(right[0]));
    		
    		int del_x = red_x*red_x + green_x*green_x + blue_x*blue_x;
    		int del_y = red_y*red_y + green_y*green_y + blue_y*blue_y;
    		
    		imp_mat[i][j] = sqrt(del_x+del_y);
    	}
    }
    
    for(int i=1;i<row;i++)
    {
    	for(int j=0;j<col;j++)
    	{
    		int val = INT_MAX;
    		
    		if(j-1>=0 && j-1<col)
    		{
    			if(val>imp_mat[i-1][j-1])
    			{
    				val = imp_mat[i-1][j-1];
    			}
    		}
    		
    		if(j>=0 && j<col)
    		{
    			if(val>imp_mat[i-1][j])
    			{
    				val = imp_mat[i-1][j];
    			}
    		}
    		
    		if(j+1>=0 && j+1<col)
    		{
    			if(val>imp_mat[i-1][j+1])
    			{
    				val = imp_mat[i-1][j+1];
    			}
    		}
    		
    		imp_mat[i][j] = imp_mat[i][j]+val;
    	}
    }
    
    cout<<"The current width is :"<<col<<endl;
    cout<<"Enter the new width: ";
    int width;
    cin>>width;
    
    int no_of_ver_line = (col-width);
    while(no_of_ver_line--)
    {
    	int min_val = INT_MAX;
    	int index_i;
    	int index_j;
    	
    	for(int j=0;j<col;j++)
    	{
    		if(imp_mat[row-1][j] < min_val)
    		{
    			min_val = imp_mat[row-1][j];
    			index_i = row-1;
    			index_j = j;
    		}
    	}
    	
    	int i=index_i;
    	int j=index_j;
    	
    	while(i>=0 && j>=0)
    	{
    		min_val = INT_MAX;
    		index_i = -1;
    		index_j=-1;
    		
    		if(i-1>=0 && i-1<row && j-1>=0 && j-1<col)
    		{
    			if(min_val>imp_mat[i-1][j-1])
    			{
    				index_i = i-1;
    				index_j = j-1;
    				min_val = imp_mat[i-1][j-1];
    			}
    		}
    		
    		if(i-1>=0 && i-1<row && j>=0 && j<col)
    		{
    			if(min_val>imp_mat[i-1][j])
    			{
    				index_i = i-1;
    				index_j = j;
    				min_val = imp_mat[i-1][j];
    			}
    		}
    		
    		if(i-1>=0 && i-1<row && j+1>=0 && j+1<col)
    		{
    			if(min_val>imp_mat[i-1][j+1])
    			{
    				index_i = i-1;
    				index_j = j+1;
    				min_val = imp_mat[i-1][j+1];
    			}
    		}
    		
    		for(int k=j;k<col-1;k++)
    		{
    			imp_mat[i][k] = imp_mat[i][k+1];
    			rgb_mat.at<cv::Vec3b>(i, k) = rgb_mat.at<cv::Vec3b>(i, k+1);
        	}
        	
        	i=index_i;
        	j=index_j;

    	}
    	
    	col--;
    }
    
    cv::Mat new_rgb_matrix(row, width, CV_8UC3);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < width; j++) {
            cv::Vec3b pixel = rgb_mat.at<cv::Vec3b>(i, j);

            new_rgb_matrix.at<cv::Vec3b>(i, j) = pixel;
        }
    }
    
    
    cv::imshow("Result Image", new_rgb_matrix);
    cv::waitKey(0);

    return 0;
}
