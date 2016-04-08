# ConnectedComponent
A simple In-memory implementation for finding connected component in undirected graphs
# Table of Content
- [Compile and Installation](#compile-and-installation)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Input Format Convertion](#convert-input)

##Compile and Installation
In order to compile, we need the g++ compiler pre-installed. With g++ comiler, just type:

``g++ Graphstat.cpp -O3 -o [programe name]``,


it will automatically generate an executable program with name ``programe name". 
Note that [programe name] is a place holder that can be replaced with any name you want

## Input Format

  - The first line is number of nodes n
  - Starting from the second line: 
  Assume u denotes a node id and d denotes its degree, the format of neighborhood representation format is :
  u,d:v_1:v_2:...:v_d. And also v_1, v_2, ..., v_d are in an order.
  - Each node id is an integer or long withine the range [0,n-1], where n is number of nodes
  - Each node is sorted by its id, that is, line (i+2) is for node with id i (i=0 to n-1).
  
  ### An example of the Input format
    A graph with 10 nodes is represented as follows, where node 0 has out-link neighbors 1, 2, 3, 6, 7 and 8.
    
    
    10
    
    0,6:1:2:3:6:7:8
    
    1,4:3:4:6:7
    
    2,5:3:4:6:7:8
    
    3,2:4:9
    
    4,3:3:5:8
    
    5,2:4:8
    
    6,1:7
    
    7,2:0:1
    
    8,0
    
    9,1:3
    
## Output Format
Each line contains two integers that are separated by ''\t", where the first integer is the id of nodes, 
and the second integer is the id of connected components.

An example of output is available [here](https://github.com/linhongseba/ConnectedComponent/blob/master/Connected_Comp/EG.txt_cc.txt)

## Convert Input
In many real case, the id for each node is a string, rather than an integer. Please look [here](https://github.com/linhongseba/MaximumClique/blob/master/README.md) 
for a walkthrough example and related codes about how to convert the edge table format with string id to the required input format.




    
