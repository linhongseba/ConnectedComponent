# Connected Component and Random Walk
A simple In-memory implementation for 
   - finding connected component in undirected graphs
   - rooted random walk starting from each node
   - A fast approximate rooted ranom walk starting from each node

# Table of Content
- [Compile and Installation](#compile-and-installation)
- [Input Format](#input-format)
- [Output Format](#output-format)
- [Input Format Convertion](#convert-input)

##Compile and Installation
   - In order to compile, we need the g++ compiler pre-installed. With g++ comiler, just go into the src folder and type make to compile, it will automatically generate three executable programs with name: connectcomp, TSV2wstandard, randomwalk.
   - To execute the connected component program, type:
   
      [./connectcomp] [input-unweighted-graph-file-name] [output-graph-file-name]

      The format of input graph file, is specified as unweighted Graph in [Input Format](#input-format).

      As an example, we could type
      ./connectedcomp EG.txt EG.txt_cc.txt

      It will compute the connected component of the graph specified in [EG.txt] (https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph/EG.txt) and output results into [EG.txt_cc.txt](https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph/EG.txt_cc.txt).
      
   - To executate the random walk program, type:
   
     [./randomwalk] [input-weighted-graph-file-name] [file-of-node-name] [file-phone-to-uri][output-file-name] [threshold]
     
      The format of input graph file, is specified as weighted graph in [Input Format](#input-format).
      
      As an example, we could type
      ./randomwalk EG3.txt_new.txt name.txt phone2url.txt toycluster-0.2.txt 0.2
      
      For each node u, it will return the set of nodes that has hightest probability to reach from u. The graph is a weighted graph specified in [EG3.txt_new.txt] (https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph-random-walk/EG3.txt_new.txt), and the outputs are saved in file [toycluster-0.2.txt] (https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph-random-walk/toycluster-0.2.txt).
      
  - The executation of the fast random walk shares the same input parameter with random walk.
  

## Input Format
### Unweighted Graph (For connected component)
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

More examples of input are available [here](https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph/EG.txt)

### Weighted Graph
The first line is number of nodes, and starting from the second line is the adjacence list of each node formated as follows:

node_id,degree_d:neighboreid1,weight1:neighborid2,weight2:...neighboridd,weightd

Note that the node_id is within the range [0,n-1], where n is number of nodes, and the list of neighbors are sorted in ascending order too.

An example of input graph file is as follows:

3

0,2:1,1.0:2,1.0

1,2:0,1.0:2,1.0

2,2:0,1.0:1,1.0

where this graph is a triangle with three vertices, and each edge is with weight 1.0.

Another example of input graph is as follows:

7

0,2:1,5.000000:2,10.000000

1,2:0,5.000000:3,2.000000

2,1:0,10.000000

3,1:1,2.000000

4,2:5,1.000000:6,6.000000

5,1:4,1.000000

6,1:4,6.000000


## Output Format
### Connected Component
Each line contains two integers that are separated by ''\t", where the first integer is the id of nodes, 
and the second integer is the id of connected components.

An example of output is available [here](https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/example-graph/EG.txt_cc.txt)

### Random Walk
Each line is formated as follows:
{"cluster-id": "str", "centroid_phone": "str", "phones": [{"name": "str", "weight": double}..., {"name": "str", "weight": double}], "CDRIDs": [{"uri": "str", "weight": double}, ..., {"uri": "str", "weight": double}]}

An example of output is available [here](https://github.com/linhongseba/ConnectedComponent/blob/master/Connected_Comp/example-graph-random-walk/toycluster-0.2.txt)

## Convert Input
In many real case, the id for each node is a string, rather than an integer. Please look [here](https://github.com/linhongseba/MaximumClique/blob/master/README.md) 
for a walkthrough example and related codes about how to convert the unweighted edge table format with string id to the required input format.

For the weighted edge table format, please use the TSV2wstandard to convert into the required input format.

In the HT domain, an example of converting Java script is in [here](https://github.com/linhongseba/ConnectedComponent/tree/master/Connected_Comp/pre-process/src/phonegraph)


Usage: raw_json_file output_prefix_string k (integer)
The input is the raw Json line file, while the outputs are three files:

1) phone/email string to integer mapping file

2) phone/email integer to CDR uri id mapping file

3) phone/emmail co-occurrence graph in the weighted edge table format (i.e, source(integer)[\t]target(integer)[\t]weights

## Walk Through Pipeline

1) Run the pre-processing script (the Java code phonegraph) to obtain three files: phone_mapID.txt, phone2urlID.txt, phone_graphID.txt, where ID denotes the output_prefix_string given by the user

2) Run the TSV2wstandard to convert the phone_graphID.txt in weighted edge table format to the required input format phone_graphID.txt_new.txt

3) Run the randomwalk code using the outputs of 1) and 2)





    
