all: connectcomp TSV2wstandard randomwalk
connectcomp: 
	g++ Graphstat.cpp -O2 -o connectcomp

randomwalk: 
	g++ randomwalk.cpp -o2 -o randomwalk
	
TSV2wstandard: 
	g++ tsv2weightstandard.cpp -O2 -o TSV2wstandard

clean:
	rm -f $(all)